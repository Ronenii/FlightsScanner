#include "Flight_Service_Funcs.h"

// This is the Parent process part of the task.
// 1) Get opcode input from user and write it to pipe
// 2) Once written, wait until child process writes back the result
// 3) prints said result
void FlightServiceTask(int namedPipeFsToDbFD, int namedPipeDbToFsFD) {
    int opCode, prevOpCode = 0;
    bool sendCleanMe = true, skipFlush = false;

    do {
        PrintMenu();
        try {
            SendStringToDbCleanPipe(namedPipeFsToDbFD, sendCleanMe, prevOpCode);
            FlushRestartOrOkSeq(namedPipeDbToFsFD, skipFlush);

            GetParamsFromUserAndSendToDbService(namedPipeFsToDbFD,opCode);

            prevOpCode = opCode;

            ReadStringFromDbService(namedPipeDbToFsFD);

            if (opCode == ZIP_DATABASE) // Clear buffer after the use of some of the functions
                BufferFlush(); // needed to clear the buffer in order to make the getchar() to wait for the user's input
        }
        catch (exception &e) {
            cout << e.what() << endl;
            ChangeSendCleanMe(e.what(), sendCleanMe, skipFlush);
        }
        if (static_cast<MenuOptions>(opCode) != MenuOptions::GracefulExit) {
            WaitForUserInputToContinue();
        }
    } while (static_cast<MenuOptions>(opCode) != MenuOptions::GracefulExit);
}

// Creates a string with the opcode and additional parameters if necessary and sends to dbService
void GetParamsFromUserAndSendToDbService(int namedPipeFsToDbFD, int &opCode) {
    string query;
    opCode = GetUserOpcodeInput(); // step 3
    string inputParameters;

    if (opCode >= 1 && opCode <= 4) {
        inputParameters = GetInputParamsFromUserAndFormat(opCode);
    }

    query = BuildQuery(opCode, inputParameters);
    WriteStringToDbService(query, namedPipeFsToDbFD);
}

// Gets the string list from user, if the string list is airport calltags then formats them to uppercase to avoid
// multiple folders being generated of the same airport with different capitalization (llBG, LLBG, LLbg, etc...)
string GetInputParamsFromUserAndFormat(int opCode)
{
    string ret;
    ret = GetStringsListFromUser(opCode);
    if (opCode >= 1 && opCode <=
                       3) //If input parameters are airport names convert them to upper so that the db will always have 1 folder of an airport
    {
        transform(ret.begin(), ret.end(), ret.begin(), ::toupper);
    }

    return ret;
}

// Builds a query to the dbService with the given opcode and input parameters
string BuildQuery(int opcode, string inputParameters)
{
    string ret;
    ret += to_string(opcode * (-1));
    ret += " ";
    ret += inputParameters;

    return ret;
}


// Sends a string to the fbService signaling to clean the pipe
void SendStringToDbCleanPipe(int pipeFD, bool &sendCleanMe, int prevOpCode) {
    if (sendCleanMe || prevOpCode == 1) {
        string garbage = "Clean me";
        WriteStringToDbService(garbage, pipeFD);
        if (prevOpCode == 1)
            sendCleanMe = true;
    } else {
        sendCleanMe = true;
    }
}

// If a clean me msg was sent to db, wil lchange the flags so that no other clean me will be sent afterwards
void ChangeSendCleanMe(const char *message, bool &sendCleanMe, bool &skipFlush) {
    if (strcmp(message, dbCrushMessage.c_str()) == 0) {
        sendCleanMe = false;
        skipFlush = true;
    }
}

// Will flush the first restart response the DB Service returns. Afterwards will catch restart messages only when waiting for responses.
void FlushRestartOrOkSeq(int namedPipeDbToFs, bool &skipFlush) {
    int garbage;
    char buffer[1024];
    string message;
    if (!skipFlush) {
        if (read(namedPipeDbToFs, &garbage, sizeof(int)) != -1)//get the len of message
        {
            if (read(namedPipeDbToFs, buffer, sizeof(buffer)) != -1) {
                message += buffer;
                if (strstr(message.c_str(), restartSeq.c_str()) != nullptr &&
                    strstr(message.c_str(), okSeq.c_str()) != nullptr) {
                    throw runtime_error("Runtime error: something is weird...\n");
                }
            }
        }
    } else {
        skipFlush = false;
    }
}

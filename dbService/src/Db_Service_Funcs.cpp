#include "Db_Service_Funcs.h"


// This is the flight service main runtime loop
// 1) If the service is started, notify Flight Service
// 2) If DB zip filed is not unzipped, unzip it
// 3) Receieve query from Flight service and process it to opCode and input parameters
// 4) Handle the given opcode and input parameters in the corresponding functions
// 5) Write a response back to Flight service with either the program output or the exception that happened
void DbServiceProcessTask(int namedPipeFsToDbFD, int namedPipeDbToFsFD) {
    int opCode;
    string res;
    vector<string> inputParams;
    bool loopFlag = true, receiveCleanPipe = true, okSeqFlag = false;

    SendRestarted(namedPipeDbToFsFD);
    // Will run in this loop, until the main process sends a signal to stop
    while(loopFlag) {
        try
        {
            unzipDB(); // Unzip called here in order to catch the exception. Only unzips once.
            CleanPipe(namedPipeFsToDbFD, receiveCleanPipe);
            SendOkSeq(namedPipeDbToFsFD, okSeqFlag);
            inputParams = GetQueryFromFlightService(namedPipeFsToDbFD, opCode);
            MenuOptions userInput = static_cast<MenuOptions>(opCode); // Cast opCode to enum

            switch (userInput) { // step 4
                case MenuOptions::UpdateDatabase:
                    res = updateDatabase(inputParams);
                    break;
                case MenuOptions::FetchAirportsIncomingFlights:
                    res = fetchAirportsIncomingFlights(inputParams);
                    break;
                case MenuOptions::FetchAirportsFullFlightSchedule:
                    res = fetchAirportsFullFlightSchedule(inputParams);
                    break;
                case MenuOptions::FetchAircraftsFullFlightSchedule:
                    res = fetchAircraftsFullFlightSchedule(inputParams);
                    break;
                case MenuOptions::ZipDatabaseFiles:
                    res = zipDatabaseFiles();
                    break;
                case MenuOptions::GracefulExit:
                    res = DbServiceGracefulExit();
                    loopFlag = false; // stops the loop
                    break;
                default:
                    res = "Invalid operation code.\n";
                    break;
            }
            WriteResponseToFlightService(namedPipeDbToFsFD, res);

        }
        catch (exception& e) {
            ChangeReceiveCleanPipeAndRestarted(e.what(), receiveCleanPipe, okSeqFlag);
            if(receiveCleanPipe)
            {
                res += e.what();
                res += '\n';
                WriteResponseToFlightService(namedPipeDbToFsFD, res);
            }
        }
    }
}


// Waits until receiving a query from Flight Service. Breaks it down to opcode and input parameters
vector<string> GetQueryFromFlightService(int namedPipeFsToDbFD, int &opCode) {
    vector<string> inputParams;

    // Convert start of query to opcode, save a pointer to the index in the string after the opcode
    string FlightServiceQuery = ReadStringFromFs(namedPipeFsToDbFD, 2);
    string::size_type paramLocation;
    opCode = stoi(FlightServiceQuery, &paramLocation);
    opCode = opCode * (-1);

    // Throw if opcode is invalid
    if(opCode > 6 || opCode < 1)
    {
        throw runtime_error("DB RESPONSE: \"Runtime error: Invalid opcode\"");
    }
    else if(opCode >= 1 || opCode <= 4)
    {
        inputParams = ConvertInputParamsToVector(FlightServiceQuery.substr(paramLocation));
    }

    return inputParams;
}


// Receiving a string with delimiters and put each string in a string vector.
vector<string> ConvertInputParamsToVector(string inputParams) {
    vector<string> res;

    char *cInputParams = new char[inputParams.length() + 1]; // convert string to char* in order to use strtok.
    strcpy(cInputParams, inputParams.c_str());
    const char *delimiter = " ";
    char *token = strtok(cInputParams, delimiter);

    while (token != nullptr) {
        res.push_back(token); // put the token in the res vector.
        token = strtok(nullptr, delimiter);
    }

    delete[] cInputParams;
    return res;
}

// If the program has restarted, write the restart magic sequence to the named pipe.
void SendRestarted(int namedPipeDbToFs)
{
    int size = dbRestartSeq.length();
    write(namedPipeDbToFs, &size, sizeof(size));
    write(namedPipeDbToFs, dbRestartSeq.c_str(), dbRestartSeq.length());
}

// Write the "ok" magic sequence to the named pipe in order to run properly the system.
// On initialize the flag set to false, the program send the restart magic sequence instead of the "ok" magic sequence.
// If the flag set to false, next run it will be true.
void SendOkSeq(int namedPipeDbToFs, bool &okSeqFlag)
{
    if(okSeqFlag)
    {
        int size = okSeq.length();
        write(namedPipeDbToFs, &size, sizeof(size));
        write(namedPipeDbToFs, okSeq.c_str(), okSeq.length());
    }
    else
        okSeqFlag = true;
}

// Handle DbService's graceful exit (zip DB)
string DbServiceGracefulExit()
{
    string message;

    // zip the current database files
    zipDatabaseFiles();

    message = "Closing program, good bye\n";
    return message;
}

// Clean the flight service to DB service named pipe in order to let the program run properly after crush.
// In some cases, the program skips the clean (receiveCleanPipe flag).
void CleanPipe(int pipeFD, bool &receiveCleanPipe)
{
    if(receiveCleanPipe)
    {
        string garbage = ReadStringFromFs(pipeFD, 1);
    }
    else
    {
        receiveCleanPipe = true;
    }
}

// Check if the flight service crushed and change the program's flags.
void ChangeReceiveCleanPipeAndRestarted(const char* message, bool &receiveCleanPipe, bool &restarted)
{
    if(strcmp(message, fsCrushMessage.c_str()) == 0)
    {
        receiveCleanPipe = false;
        restarted = true;
    }
}






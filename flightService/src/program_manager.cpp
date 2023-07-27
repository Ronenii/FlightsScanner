#include "program_manager.h"

void PrintMenu() {
    ClearScreen();
    cout << endl << "**************************" << endl <<
         "1. Fetch airports data" << endl <<
         "2. Print airports incoming flights" << endl <<
         "3. Print airports full flight schedule" << endl <<
         "4. Print aircraft full flight schedule" << endl <<
         "5. Zip Database Files" << endl <<
         "6. Shutdown" << endl <<
         "Please make your choice: " << flush;
}


// ANSI escape sequence to clear the screen
void ClearScreen() {
    cout << "\033[2J\033[1;1H" << flush;
}

// User inputs multiple string in the line and the func combine them together with a delimiter and return the string.
string GetStringsListFromUser(int opCode) {
    string s;
    BufferFlush();

    PrintMessageByOpCode(opCode);
    getline(cin, s);

    return s;
}

// Prints out a prompt based on the given opcode
void PrintMessageByOpCode(int opCode) {
    MenuOptions userInput = static_cast<MenuOptions>(opCode); // Cast opCode to enum

    switch (userInput) {
        case MenuOptions::UpdateDatabase:
            cout << "Please enter airport ICOA names: " << flush;
            break;
        case MenuOptions::FetchAirportsIncomingFlights:
            cout << "Please enter airport ICOA names: " << flush;
            break;
        case MenuOptions::FetchAirportsFullFlightSchedule:
            cout << "Please enter airport ICOA name: " << flush;
            break;
        case MenuOptions::FetchAircraftsFullFlightSchedule:
            cout << "Please enter all required aircraft Icoa24: " << flush;
            break;
    }
}

// Creates a path to the named pipe designated for communication from flight service to dbService
string GetNamedPipeMainToChildPath()
{
    string namedPipeMainToChild = "/tmp/flights_pipes/namedPipeMainToChild";
    return namedPipeMainToChild;
}

// Creates a path to the named pipe designated for communication from dbService to flight service
string GetNamedPipeChildToMainPath()
{
    string namedPipeChildToMain = "/tmp/flights_pipes/namedPipeChildToMain";
    return namedPipeChildToMain;
}

// Waits for any key to be entered to continue with the program
void WaitForUserInputToContinue() {
    cout << "Press any key to continue..." << flush;
    getchar();
}


// Read the result from the child process and print it
void ReadStringFromDbService(int pipefd) {
    int bytesRead, size;
    char buffer[4096];

    if(read(pipefd, &size, sizeof(size)) != -1)
    {
        while (size > 0) {
            bytesRead = read(pipefd, buffer, sizeof(buffer)-1);
            buffer[bytesRead] = '\0';
            if(bytesRead == -1)
            {
                throw(runtime_error("Error while reading from pipe\n"));
            }

            size -= bytesRead;

            // If we recieve the magic sequence signaling restart then the dbService has restarted
            if(strcmp(buffer, restartSeq.c_str()) == 0)
            {
                throw runtime_error("Runtime error: DB service restarted\n");
            }

            cout << buffer << flush;
        }
    }
}

void WriteStringToDbService(string str, int namedPipeFsToDbFD)
{
    int inputLength = str.length();
    write(namedPipeFsToDbFD, &inputLength, sizeof(inputLength));
    write(namedPipeFsToDbFD, str.c_str(), str.length());
}

// Gets user opcode input, throws exception if input invalid
int GetUserOpcodeInput() {

    string input;
    int opCode;
    cin >> input;

    try {
        opCode = stoi(input);
        if (opCode > 6 || opCode < 1)
            throw std::runtime_error("Invalid input: opCode invalid.");

        return opCode;
    }
    catch (const exception &e) {
        throw std::runtime_error("Invalid input: Must be numbers\n");
    }
}


// Discard remaining characters in the input buffer
void BufferFlush() {
    int c;
    while (((c = getchar()) != '\n') && (c != EOF)) {}
}

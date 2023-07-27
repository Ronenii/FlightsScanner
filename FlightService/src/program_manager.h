#ifndef PROGRAM_MANAGER_H
#define PROGRAM_MANAGER_H

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "exception"
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <sys/types.h>
#include <unistd.h>
#include <filesystem>

using namespace std;
using namespace std::filesystem;

enum class MenuOptions {
    UpdateDatabase = 1,
    FetchAirportsIncomingFlights,
    FetchAirportsFullFlightSchedule,
    FetchAircraftsFullFlightSchedule,
    ZipDatabaseFiles,
    GracefulExit
};

const string restartSeq = "#$$31**";

const string okSeq = "#$$32**";

const string dbCrushMessage = "Runtime error: DB service restarted\n";

void PrintMenu();

void ClearScreen();

void WaitForUserInputToContinue();

void WriteStringToDbService(string str, int namedPipeFsToDbFD);

void ReadStringFromDbService(int pipeFd);

int GetUserOpcodeInput();

void BufferFlush();

string GetNamedPipeChildToMainPath();

string GetNamedPipeMainToChildPath();

void PrintMessageByOpCode(int opCode);

string GetStringsListFromUser(int opCode);

#endif // PROGRAM_MANAGER_H
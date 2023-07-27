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
#include <filesystem>
#include "programs.h"
#include "exception"

#define STOUT_FILENO 1
#define OPTION_NUM_FIVE 5

enum class MenuOptions {
    UpdateDatabase = 1,
    FetchAirportsIncomingFlights,
    FetchAirportsFullFlightSchedule,
    FetchAircraftsFullFlightSchedule,
    ZipDatabaseFiles,
    GracefulExit
};

const string fsRestartSeq = "Clean me";

void WriteResponseToFlightService(int namedPipeDbToFsFD, string response);

string ReadStringFromFs(int pipefd, int stepNum);

string GetNamedPipeDbToFsPath();

string GetNamedPipeFsToDbPath();

string GetDirectoryPath();

void unzipDB();

#endif // PROGRAM_MANAGER_H
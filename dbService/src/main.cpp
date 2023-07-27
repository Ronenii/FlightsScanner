#include "Db_Service_Funcs.h"

int main() {
    // TODO: make only named pipe child to main, communicate through it with the parent.
    string directoryPath = GetDirectoryPath();
    mkdir(directoryPath.c_str(), 0777);
    string namedPipeFsToDb = GetNamedPipeFsToDbPath();
    string namedPipeDbToFs = GetNamedPipeDbToFsPath();

    mkfifo(namedPipeFsToDb.c_str(), 0666);
    mkfifo(namedPipeDbToFs.c_str(), 0666);

    int namedPipeFsToDbFD, namedPipeDbToFsFD;
    namedPipeFsToDbFD = open(namedPipeFsToDb.c_str(), O_RDWR);
    namedPipeDbToFsFD = open(namedPipeDbToFs.c_str(), O_RDWR);


    DbServiceProcessTask(namedPipeFsToDbFD, namedPipeDbToFsFD);

    // close write and read pipes which were in use
    close(namedPipeDbToFsFD);
    close(namedPipeFsToDbFD);

    unlink(namedPipeFsToDb.c_str());
    unlink(namedPipeDbToFs.c_str());

    exit(0);
}


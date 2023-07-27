#include "Flight_Service_Funcs.h"

int main() {
    string namedPipeFsToDb = GetNamedPipeMainToChildPath();
    string namedPipeDbToFs = GetNamedPipeChildToMainPath();

    // Child process creates the named pipes.

    int namedPipeFsToDbFD, namedPipeDbToFsFD;
    namedPipeFsToDbFD = open(namedPipeFsToDb.c_str(), O_RDWR);
    namedPipeDbToFsFD = open(namedPipeDbToFs.c_str(), O_RDWR);

    try {
        FlightServiceTask(namedPipeFsToDbFD, namedPipeDbToFsFD);

        // close write and read pipes which were in use
        close(namedPipeDbToFsFD);
        close(namedPipeFsToDbFD);

        unlink(namedPipeFsToDb.c_str());
        unlink(namedPipeDbToFs.c_str());
        exit(0);
    }
    catch (exception& e) {
        cout << e.what() << endl;

        close(namedPipeDbToFsFD);
        close(namedPipeFsToDbFD);

        unlink(namedPipeFsToDb.c_str());
        unlink(namedPipeDbToFs.c_str());
        exit(0);
    }
}


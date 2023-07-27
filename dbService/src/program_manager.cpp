#include "program_manager.h"


// Read the received string from parent. This function doesn't use while loop in order to read
// the string from the main process.
string ReadStringFromFs(int pipefd, int stepNum)
{
    int bytesRead, size;
    char buffer[1024];
    string result;

    if(read(pipefd, &size, sizeof(size)) != -1)
    {
        while (size > 0) {
            bytesRead = read(pipefd, buffer, sizeof(buffer)-1);
            if(bytesRead == -1)
            {
                throw(runtime_error("Error while reading from pipe"));
            }
            buffer[bytesRead] = '\0';
            size -= bytesRead;
            result += buffer;
        }

        // If we recieve the magic sequence signaling restart then the dbService has restarted
        if(strcmp(buffer, fsRestartSeq.c_str()) == 0 && stepNum == 2)
        {
            throw runtime_error("Runtime error: flight service restarted\n");
        }
    }

    return result;
}

void WriteResponseToFlightService(int namedPipeDbToFsFD, string response)
{
    int size;
    size = response.length()+1;
    write(namedPipeDbToFsFD, &size, sizeof(size));
    write(namedPipeDbToFsFD, response.c_str(), size);
}

// Checks if a DB zip file exists. If so, unzips it. Can only unzip once in a program's runtime
void unzipDB()
{
    static bool unzipped = false;

    if(std::filesystem::exists("/tmp/DB.zip") && !unzipped)
    {
        unzipped = true;
        int status = std::system("unzip -o /tmp/DB.zip -d /tmp");

        if(status != 0)
        {
            throw runtime_error("unzip failed with exit status: " + to_string(status));
        }
    }
}

// Creates a path to the named pipe designated for communication from flight service to dbService
string GetNamedPipeFsToDbPath()
{
    string namedPipeMainToChild ="/tmp/flights_pipes/namedPipeMainToChild";
    return namedPipeMainToChild;
}

// Creates a path to the named pipe designated for communication from dbService to flight service
string GetNamedPipeDbToFsPath()
{
    string namedPipeChildToMain = "/tmp/flights_pipes/namedPipeChildToMain";
    return namedPipeChildToMain;
}

string GetDirectoryPath()
{
    string directoryPath ="/tmp/flights_pipes";
    return directoryPath;
}
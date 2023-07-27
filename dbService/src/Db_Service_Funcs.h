#ifndef CHILD_PROCESS_H
#define CHILD_PROCESS_H

#include "program_manager.h"

const string dbRestartSeq = "#$$31**";

const string okSeq = "#$$32**";

const string fsCrushMessage = "Runtime error: flight service restarted\n";

vector<string> ConvertInputParamsToVector(string inputParams);

void DbServiceProcessTask(int namedPipeFsToDbFD, int namedPipeDbToFsFD);

vector<string> GetQueryFromFlightService(int namedPipeFsToDbFD, int &opCode);

string DbServiceGracefulExit();

void SendRestarted(int namedPipeDbToFs);

void CleanPipe(int pipeFD, bool &receiveCleanPipe);

void ChangeReceiveCleanPipeAndRestarted(const char* message, bool &receiveCleanPipe, bool &restarted);

void SendOkSeq(int namedPipeDbToFs, bool &okSeqFlag);

#endif // CHILD_PROCESS_H
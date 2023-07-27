#ifndef PARENT_PROCESS_H
#define PARENT_PROCESS_H

#include "program_manager.h"

const int ZIP_DATABASE = 5;

void FlightServiceTask(int namedPipeFsToDbFD, int namedPipeDbToFsFD);

void GetParamsFromUserAndSendToDbService(int namedPipeFsToDbFD, int &opCode);

string GetInputParamsFromUserAndFormat(int opCode);

void FlushRestartOrOkSeq(int namedPipeDbToFs, bool &skipFlush);

void SendStringToDbCleanPipe(int pipeFD, bool &sendCleanMe, int prevOpCode);

void ChangeSendCleanMe(const char* message, bool &sendCleanMe, bool &skipFlush);

string BuildQuery(int opcode, string inputParameters);

#endif // PARENT_PROCESS_H
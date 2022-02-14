#ifndef DATATYPES_H
#define DATATYPES_H
#include <stdio.h>

typedef struct ConfigDataType
{
  double version;
  char metaDataFileName[100];
  int cpuSchedCode;
  int quantumCycles;
  int memAvailable;
  int procCycleRate;
  int ioCycleRate;
  int logToCode;
  char logToFileName[100];
} ConfigDataType;

typedef struct OpCodeType
{
  int pid;
  char command[5];
  char inOutArg[5];
  char strArg1[15];
  int intArg2;
  int intArg3;
  double opEndTime;
  struct OpCodeType *nextNode;
} OpCodeType;

#endif
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
  int pid;          // ID for PCB
  char command[5];  // Three letter command
  char inOutArg[5]; // Device I/O
  char strArg1[15]; // Descriptor or arg 1
  int intArg2;      // Memory or cycles: 4 byte int
  int intArg3;      // Memory: 4 byte int (and non or premptive indicator?)
  double opEndTime; // Size of the time string returned from accessTimer
  struct OpCodeType *nextNode; // Ptr to the next node, linked list
} OpCodeType;

// The PCB contains information associated with a process
typedef struct PCB
{
  // Shows the number of a particular process
  int id;

  int remainingTime;

  // Specifies the process state: new, ready, running, terminated / exiting
  char *state;
  // struct action *PC;
} PCB;

#endif
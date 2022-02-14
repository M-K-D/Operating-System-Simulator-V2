#ifndef METADATAOPS_H
#define METADATAOPS_H

#include "datatypes.h"
#include "StringUtils.h"
#include <stdlib.h>
#include <stdio.h>

typedef enum {MD_FILE_ACCESS_ERR = 3,
              MD_CORRUPT_DESCRIPTOR_ERR,
              OPCMD_ACCESS_ERR,
              CORRUPT_OPCMD_ERR,
              CORRUPT_OPCMD_ARG_ERR,
              UNBALANCED_START_END_ERR,
              COMPLETE_OPCMD_FOUND_MSG,
              LAST_OPCMD_FOUND_MSG} OpCodeMessages;

extern const int BAD_ARG_VAL;

OpCodeType *addNode(OpCodeType *localPtr, OpCodeType *newNode);
int calcOpTime(ConfigDataType *configPtr, OpCodeType *opCode);
OpCodeType *clearMetaDataList(OpCodeType *localPtr);
void copyOpCode(OpCodeType *destination, OpCodeType *source);
OpCodeType *createInitOpCode();
void displayMetaData(OpCodeType *localPtr);
int getCommand(char *cmd, char *inputStr, int index);
int getOpCommand(FILE *filePtr, OpCodeType *inData);
bool getMetaData(char *fileName, OpCodeType **opCodeDataHead, char *endStateMsg);
int getNumberArg(int *number, char *inputStr, int index);
int getStringArg(int *strArg, char *inputStr, int index);
bool isDigit(char testChar);
bool verifyFirstStringArg(char *strArg);
bool verifyValidCommand(char *testCmd);
int updateEndCount(int count, char *opString);
int updateStartCount(int count, char *opString);

#endif
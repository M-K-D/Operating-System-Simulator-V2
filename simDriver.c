#include "simDriver.h"
#include <stdbool.h>
#include <stdlib.h>
#include "metadataops.h"
#include "configops.h"
#include "datatypes.h"

int main(int argc, char **argv)
{
  bool programRunFlag = false;
  bool configDisplayFlag = false;
  bool configUploadFlag = false;
  bool mdDisplayFlag = false;
  bool runSimFlag = false;
  bool infoFlag = false;
  int argIndex = 1;
  int lastFourLetters = 4;
  int fileStrLen, fileStrSubLoc;
  char fileName[STD_STR_LEN];
  char errorMessage[MAX_STR_LEN];
  ConfigDataType *configDataPtr = NULL;
  OpCodeType *metaDataPtr = NULL;

  printf("\nSimulator Program\n");
  printf("==================\n\n");

  if(argc < 2)
  {
    showProgramFormat();

    programRunFlag = false;

    infoFlag = true;

  }

  fileName[0] = NULL_CHAR;

  while(!programRunFlag && argIndex < argc)
  {
    fileStrLen = getStringLength(argv[argIndex]);
    fileStrSubLoc = findSubString(argv[argIndex], ".cnf");

    if(compareString(argv[argIndex], "-dc") == STR_EQ)
    {
      configUploadFlag = true;

      configDisplayFlag = true;
    }

    else if(compareString(argv[argIndex], "-dm") == STR_EQ)
    {
      configUploadFlag = true;

      mdDisplayFlag = true;
    }

    else if(compareString(argv[argIndex], "-rs") == STR_EQ)
    {
      configUploadFlag = true;

      runSimFlag = true;
    }

    else if(fileStrSubLoc != SUBSTRING_NOT_FOUND 
            && fileStrSubLoc == fileStrLen - lastFourLetters)
    {
      copyString(fileName, argv[argIndex]);

      programRunFlag = true;
    }

    argIndex++;
  }

  // Check for command line failure 
  if(!programRunFlag && !infoFlag)
  {
    printf("Incorrect argument line format, program aborted\n\n");

    showProgramFormat();
  }

  if(programRunFlag && configUploadFlag)
  {
    if(getStringLength(fileName) > 0 
      && getConfigData(fileName, &configDataPtr, errorMessage))
    {
      if(configDisplayFlag)
      {
        displayConfigData(configDataPtr);
      }
    }

    else
    {
      printf("\nConfig Upload Error: %s, program aborted\n\n", errorMessage);

      programRunFlag = false;
    }  
  }

  if(programRunFlag
    && (mdDisplayFlag || runSimFlag))
  {
    if(getMetaData(configDataPtr->metaDataFileName,
      &metaDataPtr, errorMessage))
    {
      if(mdDisplayFlag)
      {
        displayMetaData(metaDataPtr);
      }

      if(runSimFlag)
      {
        //runSim(configDataPtr, metaDataPtr);
        runSim();
      }
    }

    else
    {
      printf("\nMetadata Upload Error: %s, program aborted\n", errorMessage);
    }
  }

  configDataPtr = clearConfigData(configDataPtr);
  metaDataPtr = clearMetaDataList(metaDataPtr);

  printf("\nSimulator Program End.\n\n");

  return 0;
}


void showProgramFormat()
{
  printf( "Program Format:\n" );
  printf( "     sim_0x [-dc] [-dm] [-rs] <config file name>\n" );
  printf( "     -ds [optional] displays configuration data\n" );
  printf( "     -dm [optional] displays meta data\n" );
  printf( "     -rs [optional] runs simulator\n" );
  printf( "     config file name is required\n" );
}
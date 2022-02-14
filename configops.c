#include "stdbool.h"
#include "configops.h"
#include "StringUtils.h"

ConfigDataType *clearConfigData(ConfigDataType *configData)
{
  printf("calling clearConfigData()\n");
  return NULL;
}

void configCodeToString(int code, char *outString)
{
  char displayString[8][10] = {"SJF-N", "SRTF-P", "FCFS-P",
                               "RR-P", "FCFS-N", "Monitor",
                               "File", "Both"};

  copyString(outString, displayString[code]);
}

void displayConfigData(ConfigDataType *configData)
{
  char displayString[STD_STR_LEN];
  printf("Config File Display\n");
  printf("-------------------\n");
  printf("Version                 : %3.2f\n", configData->version);
  printf("Program file name       : %s\n", configData->metaDataFileName);
  configCodeToString(configData->cpuSchedCode, displayString);
  printf("CPU Schedule selection  : %s\n", displayString);
  printf("Quantum time            : %d\n", configData->quantumCycles);
  printf("Memory Available        : %d\n", configData->memAvailable);
  printf("Process cycle rate      : %d\n", configData->procCycleRate);
  printf("IO cycle rate           : %d\n", configData->ioCycleRate);
  configCodeToString(configData->logToCode, displayString);
  printf("Log to selection        : %s\n", displayString);
  printf("Log file name           : %s\n", configData->logToFileName);
}

bool getConfigData(char *fileName, ConfigDataType **configData, char *endStateMsg)
{
  const int NUM_DATA_LINES = 9;
  const char READ_ONLY_FLAG[] = "r";
  ConfigDataType *tempData;

  FILE *fileAccessPtr;
  char dataBuffer[MAX_STR_LEN];
  int intData, dataLineCode, lineCtr = 0;
  double doubleData;
  bool dontStopAtNonPrintable = false;

  copyString(endStateMsg, "Configuration file upload successful");

  *configData = NULL;

  fileAccessPtr = fopen(fileName, READ_ONLY_FLAG);

  if(fileAccessPtr == NULL)
  {
    copyString(endStateMsg, "Configuration file access error");
    return false;
  }

  if(!getStringToDelimiter(fileAccessPtr, COLON, dataBuffer)
     || compareString(dataBuffer, "Start Simulator Configuration File") != STR_EQ)
  {
    fclose(fileAccessPtr);

    copyString(endStateMsg, "Corrupt configuration leader line error");

    return false;
  }

  tempData = (ConfigDataType *) malloc(sizeof(ConfigDataType));

  while(lineCtr < NUM_DATA_LINES)
  {
    if(!getStringToDelimiter(fileAccessPtr, COLON, dataBuffer))
    {
      free(tempData);

      fclose(fileAccessPtr);

      copyString(endStateMsg, "Configuration start line capture error");
      return false;
    }

    dataLineCode = getDataLineCode(dataBuffer); // MAybe an issue, databuffer

    if(dataLineCode != CFG_CORRUPT_PROMPT_ERR)
    {
      if(dataLineCode == CFG_VERSION_CODE)
      {
        fscanf(fileAccessPtr, "%lf", &doubleData);
      }

      else if (dataLineCode == CFG_MD_FILE_NAME_CODE
              || dataLineCode == CFG_LOG_FILE_NAME_CODE
              || dataLineCode == CFG_CPU_SCHED_CODE
              || dataLineCode == CFG_LOG_TO_CODE)
      {
        fscanf(fileAccessPtr, "%s", &dataBuffer);
      }
      else
      {
        fscanf(fileAccessPtr, "%d", &intData);
      }
       
      if(valueInRange(dataLineCode, intData, doubleData, dataBuffer))
      {
        switch(dataLineCode)
        {
          case CFG_VERSION_CODE:
            tempData->version = doubleData;
            break;

          case CFG_MD_FILE_NAME_CODE:
            copyString(tempData->metaDataFileName, dataBuffer);
            break;

          case CFG_CPU_SCHED_CODE:
            tempData->cpuSchedCode = getCpuSchedCode(dataBuffer);
            break;

          case CFG_QUANT_CYCLES_CODE:
            tempData->quantumCycles = intData;
            break;

          case CFG_MEM_AVAILABLE_CODE:
            tempData->memAvailable = intData;
            break;

          case CFG_PROC_CYCLES_CODE:
            tempData->procCycleRate = intData;
            break;

          case CFG_IO_CYCLES_CODE:
            tempData->ioCycleRate = intData;
            break;

          case CFG_LOG_TO_CODE:
            tempData->logToCode = getLogToCode(dataBuffer);
            break;

          case CFG_LOG_FILE_NAME_CODE:
            copyString(tempData->logToFileName, dataBuffer);
            break;
        }
      }

      else
      {
        free(tempData);
        fclose(fileAccessPtr);
        copyString(endStateMsg, "Configuration item out of range");
        return false; 
      }   
    }

    else
    {
      free(tempData);
      fclose(fileAccessPtr);
      copyString(endStateMsg, "Corrupted configuration prompt");
      return false;
    }

    lineCtr++;  
  }

  if(!getStringToDelimiter(fileAccessPtr, PERIOD, dataBuffer) 
    || compareString(dataBuffer, "End Simulator Configuration File") != STR_EQ)
  {
    free(tempData);
    fclose(fileAccessPtr);
    copyString(endStateMsg, "Configuration end line capture error");

    return false;
  }

  *configData = tempData;

  fclose(fileAccessPtr);

  return true;
}
// 12:50 configops s6

ConfigDataCodes getCpuSchedCode(char *codeStr)
{
  int strLen = getStringLength(codeStr);
  char *tempStr = (char * )malloc(strLen + 1);
  int returnVal = CPU_SCHED_FCFS_N_CODE;

  setStrToLowerCase(tempStr, codeStr);

  if(compareString(tempStr, "sjf-n") == STR_EQ)
  {
    returnVal = CPU_SCHED_SJF_N_CODE;
  }

  if(compareString(tempStr, "srtf-p") == STR_EQ)
  {
    returnVal = CPU_SCHED_SRTF_P_CODE;
  }

  if(compareString(tempStr, "fcfs-p") == STR_EQ)
  {
    returnVal = CPU_SCHED_FCFS_P_CODE;
  }

  if(compareString(tempStr, "rr-p") == STR_EQ)
  {
    returnVal = CPU_SCHED_RR_P_CODE;
  }

  free(tempStr);
  return returnVal;
}

int getDataLineCode(char *dataBuffer)
{
  if(compareString(dataBuffer, "Version/Phase") == STR_EQ)
  {
    return CFG_VERSION_CODE;
  }

  if(compareString(dataBuffer, "File Path") == STR_EQ)
  {
    return CFG_MD_FILE_NAME_CODE;
  }

  if(compareString(dataBuffer, "CPU Schedulung Code") == STR_EQ)
  {
    return CFG_CPU_SCHED_CODE;
  }

  if(compareString(dataBuffer, "Quantum Time (cycles)") == STR_EQ)
  {
    return CFG_QUANT_CYCLES_CODE;
  }
  
  if(compareString(dataBuffer, "Memory Available (KB)") == STR_EQ)
  {
    return CFG_MEM_AVAILABLE_CODE;
  }

  if(compareString(dataBuffer, "Processor Cycle Time (msec)") == STR_EQ)
  {
    return CFG_PROC_CYCLES_CODE;
  }

  if(compareString(dataBuffer, "I/O Cycle Time (msec)") == STR_EQ)
  {
    return CFG_IO_CYCLES_CODE;
  }

  if(compareString(dataBuffer, "Log To") == STR_EQ)
  {
    return CFG_LOG_TO_CODE;
  }
  
  if(compareString(dataBuffer, "Log File Path") == STR_EQ)
  {
    return CFG_LOG_FILE_NAME_CODE;
  }

  return CFG_CORRUPT_PROMPT_ERR;
}

ConfigDataCodes getLogToCode(char *logToStr)
{
  int strLen = getStringLength(logToStr);
  char *tempStr = (char *)malloc(strLen + 1);
  int returnVal = LOGTO_MONITOR_CODE;

  setStrToLowerCase(tempStr, logToStr);

  if(compareString(tempStr, "both") == STR_EQ)
  {
    returnVal = LOGTO_BOTH_CODE;
  }
  
  if(compareString(tempStr, "file") == STR_EQ)
  {
    returnVal = LOGTO_FILE_CODE;
  }
  
  free(tempStr);
  return returnVal;
}

bool valueInRange(int lineCode, int intVal, double doubleVal, char *stringVal)
{
  bool result = true;
  char *tempStr;
  int strLen;

  switch (lineCode)
  {
    case CFG_VERSION_CODE:
      if(doubleVal < 0.00 || doubleVal > 10.00)
      {
        result = false;
      } 
      break;
    
    case CFG_CPU_SCHED_CODE:
      strLen = getStringLength(stringVal);
      tempStr = (char *)malloc(strLen + 1);
      setStrToLowerCase(tempStr, stringVal);

      if(compareString(tempStr, "fcfs-n") != STR_EQ &&
         compareString(tempStr, "sjf-n") != STR_EQ &&
         compareString(tempStr, "srtf-p") != STR_EQ &&
         compareString(tempStr, "fcfs-p") != STR_EQ &&
         compareString(tempStr, "rr-p") != STR_EQ )
      {
        result = false;
      }

      free(tempStr);
      break;
    
    case CFG_QUANT_CYCLES_CODE:
      if(intVal < 0 || intVal > 100)
      {
        result = false;
      }
      break;
    
    case CFG_MEM_AVAILABLE_CODE:
      if(intVal < 1024 || intVal > 102400)
      {
        result = false;
      }
      break;

    case CFG_PROC_CYCLES_CODE:
      if(intVal < 1 || intVal > 100)
      {
        result = false;
      }
      break;
    
    case CFG_IO_CYCLES_CODE:
      if(intVal < 1 || intVal > 1000)
      {
        result = false;
      }
      break;

    case CFG_LOG_TO_CODE:
      strLen = getStringLength(stringVal);
      tempStr = (char *)malloc(strLen + 1);
      setStrToLowerCase(tempStr, stringVal);

      if(compareString(tempStr,"both") != STR_EQ &&
         compareString(tempStr,"monitor") != STR_EQ &&
         compareString(tempStr,"file") != STR_EQ)
      {
        result = false;
      }

      free(tempStr);
      break;
  }

  return result;
}
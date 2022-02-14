#include "simulator.h"
#include "configops.h"
#include "metadataops.h"

void runSim()
{
  printf("calling runSim\n");
}

void createPCBs(state, PC, metadata)
{
  // loop across the metadata

    // Check for app start and okay flag
        // Create pcb node
        // Set data, pid, state
        // set PC(program counter) to the app start
        // set time remaining to zero
    // Check for not app end
        // do math, add the time for a given op code to your time remaining 
    // Otherwise, assume app end
        // set some kind of flag to support app start (may not need?)
  // end loop across meta data            

}


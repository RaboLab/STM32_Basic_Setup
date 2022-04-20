#ifndef __DEFINATION_
#define __DEFINATION_




///******* Hardware Define *******///
///******* Hardware Define *******///

#define HARDWARE_V1_0 0010		//  00 - 1 - 0   release - version - debug
#define HARDWARE_RUN HARDWARE_V1_0

#if(HARDWARE_RUN == HARDWARE_V1_0)
    #include "HardwareDefV1_0.h"
#elif(HARDWARE_RUN == HARDWARE_V1_1)
    #include "??"
#else
    #error "No Hardware Define"
#endif

#endif

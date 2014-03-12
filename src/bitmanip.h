#ifndef __2048_H
#define __2048_H

#define ClearLow(x) ((x) &= (x) - 1)

/* Headers for optimized versions of LSB, MSB and PopCnt */
#if defined(__LP64__)
    #include "inline64.h"
#else
    #include "inline32.h"
#endif

#endif

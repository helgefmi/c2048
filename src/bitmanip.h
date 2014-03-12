#ifndef __2048_H
#define __2048_H

#define ClearLow(x) ((x) &= (x) - 1)

#define PopCnt(x) (__builtin_popcount((x)) + 0)
#define LSB(x) (__builtin_ffs((x)) - 1)
#define MSB(x) (31 - __builtin_clz((x)))

#endif

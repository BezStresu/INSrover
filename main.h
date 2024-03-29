#include "MKL05Z4.h"
#ifndef MAIN_H
#define MAIN_H


// Global definitions

#define FRDM_KL05Z       										

#ifndef TRUE
# define TRUE        (1)
#endif

#ifndef	FALSE
# define FALSE       (0)
#endif

// Global macros

#define MASK(x)		 (uint32_t)(1UL << (x))    /* turn bit number into 32b mask */
#define DELAY(x)   for(uint32_t i=0;i<(x*10000);i++)__nop(); 					/* wait */

#ifdef FRDM_KL05Z
# include "MKL05Z4.h"                        /* header with CMSIS */
#endif /* FRDM_KL05Z */

#endif /* MAIN_H */

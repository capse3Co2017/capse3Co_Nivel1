#include "sapi.h"

//#define CATODO_COMUN 1
#define ANODO_COMUN 1

/*
 * @Brief   Initialize Timer peripheral
 * @param   timerNumber:   Timer number, 0 to 3
 * @param   ticks:   Number of ticks required to finish the cycle.
 * @param   voidFunctionPointer:   function to be executed at the end of the timer cycle
 * @return   nothing
 * @note   For the 'ticks' parameter, see function Timer_microsecondsToTicks
 */
void display7SegmentWrite_(uint8_t);


void display7SegmentPinConfig_( void );

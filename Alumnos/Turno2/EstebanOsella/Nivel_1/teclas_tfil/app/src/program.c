/*============================================================================
 * License: 
 * Author: 
 * Date: 
 *===========================================================================*/

/*==================[inclusions]=============================================*/

#include "program.h"   // <= Own header
#include "sapi.h"      // <= sAPI library

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[main function]==========================================*/

// MAIN FUNCTION, ENTRY PINT TO THE PROGRAM AFTER POR OR RESET.
int main( void ){

   // ---------- SETUP ----------------------------------------
   // Initialize an configurate the board
   boardConfig();   
   gpioConfig(T_FIL1, GPIO_INPUT);//_PULLUP);
   // ---------- REPEAT FOR EVER ------------------------------
   while( TRUE )
   {      
      // If TEC1 is pressed, turn on the LEDR
      gpioWrite( LEDR, !(gpioRead(T_FIL1)) );
      
      // If TEC2 is pressed, turn on the LED1
      gpioWrite( LED1, !(gpioRead(TEC2)) );
      
      // If TEC3 is pressed, turn on the LED2
      gpioWrite( LED2, !(gpioRead(TEC3)) );
      
      // If TEC4 is pressed, turn on the LED3
      gpioWrite( LED3, !(gpioRead(TEC4)) );

      // Toggle LEDB
      gpioToggle( LEDB );
      
      // Blocking delay for 100ms
      delay( 100 );
   } 

   // IT SHOULD NEVER COME HERE, because this program runs directly on a
   // microcontroller and is not called by any Operating System, as in
   // case of a PC program.
   return 0;
}

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/*==================[end of file]============================================*/

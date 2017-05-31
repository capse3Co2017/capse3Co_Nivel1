/*============================================================================
 * Licencia: 
 * Autor: 
 * Fecha: 
 *===========================================================================*/

/*==================[inlcusiones]============================================*/

#include "programa.h"   // <= su propio archivo de cabecera
#include "sapi.h"       // <= Biblioteca sAPI

/*==================[definiciones y macros]==================================*/

/*==================[definiciones de datos internos]=========================*/

/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void ){

   // ---------- CONFIGURACIONES ------------------------------
   // Inicializar y configurar la plataforma
   boardConfig();
 delay_t delayBlink;   
    delayConfig(&delayBlink,200);
   uint8_t led = 0;
   // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE )
   {  
      if(delayRead(&delayBlink) )
          led = 1;
      else
          led = 0;
      gpioWrite(LEDB,led);
      // Si se presiona TEC1, enciende el LEDR
      gpioWrite( LED1, ON );
      delay(500);
      gpioWrite( LED1, OFF );
      delay(500);
      // Si se presiona TEC2, enciende el LED1
      
   } 

   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta 
   // directamenteno sobre un microcontroladore y no es llamado/ por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
}

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

/*==================[fin del archivo]========================================*/

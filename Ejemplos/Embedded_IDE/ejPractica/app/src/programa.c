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

uint8_t ledsVector[4] = {
   LEDB,    // Column 0
   LED1,    // C1
   LED2,    // C2
   LED3     // C3
};



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
   uint8_t estadoLed=0;
   
   delayConfig(&delayBlink,500);
   
   uint8_t led=0;
   
   // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE )
   {      
      if(!(gpioRead(TEC1))){
         gpioWrite( ledsVector[led], OFF );
         led=0;
      }
      if(!(gpioRead(TEC2))){
         gpioWrite( ledsVector[led], OFF );
         led=1;
      }
      if(!(gpioRead(TEC3))) {
         gpioWrite( ledsVector[led], OFF );
         led=2;
      }
      if(!(gpioRead(TEC4))) {
         gpioWrite( ledsVector[led], OFF );
         led=3;
      }
      
      if (delayRead(&delayBlink)) {
         gpioWrite( ledsVector[led], estadoLed );
         estadoLed ^= 1;
      }

   } 

   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta 
   // directamenteno sobre un microcontroladore y no es llamado/ por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
}

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

/*==================[fin del archivo]========================================*/

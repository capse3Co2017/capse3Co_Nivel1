/*============================================================================
 * Licencia: GBSD
 * Autor: Esteban Osella
 * Fecha: 2017-05-06
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
/*  ---------> Enunciado:

Escribir un programa que haga parpadear un led de la placa.
A través de la UART se define cual de los LEDs parpadea 
-->'a': azul, 
-->'1': LED1, 
-->'2': LED2 
-->'3': LED3

Con los botones se define el período 

--> TEC1: 50 ms
--> TEC2: 100 ms
--> TEC3: 200 ms 
--> TEC4: 500 ms
*/
// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.

void initHardware()
{   
    boardConfig();   
    uartConfig( UART_USB, 115200 );
    }
    
    
int main( void ){
    // ---------- CONFIGURACIONES ------------------------------
   // Inicializar y configurar la plataforma
    initHardware();
    uint16_t delays[4] = {100, 250, 500, 1000 };
    delay_t periodo; 
    uint8_t led = LEDB;
    uint8_t receivedByte;
    
    delayConfig(&periodo,delays[0]);
    uartWriteString( UART_USB, "Prender y apagar leds.\r\n Valores admitidos: a, 1, 2, 3.\r\n" );
    uint8_t k = 0;

   // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE )
   {     
      for  (k = TEC1;k<=TEC4;k++)
      {
          if(!(gpioRead(k)))
          {
              switch (k)
              {
                  case TEC1:
                      delayConfig(&periodo,delays[0]);
                      delayRead(&periodo);
                  break;
                  case TEC2:
                      delayConfig(&periodo,delays[1]);
                      delayRead(&periodo);
                  break;
                  case TEC3:
                      delayConfig(&periodo,delays[2]);
                      delayRead(&periodo);
                  break;
                  case TEC4:
                      delayConfig(&periodo,delays[3]);
                      delayRead(&periodo);
                  break;
                }
            }
      }
      if( uartReadByte( UART_USB, &receivedByte ) )
          {
              gpioWrite( led, 0);
              switch (receivedByte)
              {
                  case 0x61:
                      uartWriteString( UART_USB,"Led Azul ...\r\n");
                      led = LEDB;
                  break;
                  case 0x31:
                      uartWriteString( UART_USB,"Led 1 ...\r\n");
                      led = LED1;
                  break;
                  case 0x32:
                      uartWriteString( UART_USB,"Led 2...\r\n");
                      led = LED2;
                  break;
                  case 0x33:
                      uartWriteString( UART_USB,"Led 3...\r\n");
                      led = LED3;
                  break;
                  default:
                      led = LEDB;
              }
              
              
          }
          if ( delayRead(&periodo) )
                  gpioToggle(led);
   } 

   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta 
   // directamenteno sobre un microcontroladore y no es llamado/ por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
}

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

/*==================[fin del archivo]========================================*/

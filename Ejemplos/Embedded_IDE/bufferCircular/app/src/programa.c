/*============================================================================
 * Licencia: 
 * Autor: 
 * Fecha: 
 *===========================================================================*/

/*==================[inlcusiones]============================================*/

#include "programa.h"   // <= su propio archivo de cabecera
#include "sapi.h"       // <= Biblioteca sAPI

#include "sapi_circularBuffer.h"       // <= Biblioteca sAPI

/*==================[definiciones y macros]==================================*/

/*==================[definiciones de datos internos]=========================*/

/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

/*==================[funcion principal]======================================*/

# define MY_BUFF_SIZE   9 // Recordar ingresar tamaño requerido + 1, porque deja un lugar en la cola
# define UART_DEBUG UART_USB

void emptyBuff( void ){
   uartWriteString( UART_DEBUG, "Buffer vacio.\r\n" );
}

void fullBuff( void ){
   uartWriteString( UART_DEBUG, "Buffer lleno.\r\n" );
}


// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void ){

   // ---------- CONFIGURACIONES ------------------------------
   // Inicializar y configurar la plataforma
   boardConfig();   
   
   // EL BUFFER ES DE ITEMS DE A 1 BYTE

   static uint8_t txData = '1';
   static uint8_t rxData;


   uint8_t myBuffMemory[ MY_BUFF_SIZE ];
   circularBuffer_t myBuff;

   circularBufferConfig(
            &(myBuff),
            myBuffMemory,
            MY_BUFF_SIZE,
            emptyBuff, fullBuff
         );
   
   // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE )
   {      
      if( uartReadByte( UART_DEBUG, &rxData ) ){

         if( rxData == 'w' ){
            if( circularBufferWrite( &myBuff, &txData ) == CIRCULAR_BUFFER_NORMAL ){
               uartWriteString( UART_DEBUG, "guarde un " );
               uartWriteByte( UART_DEBUG, txData );
               uartWriteString( UART_DEBUG, " en el buffer.\r\n" );
               txData++;
            }

         }

         if( rxData == 'r' ){
            if( circularBufferRead( &myBuff, &rxData ) == CIRCULAR_BUFFER_NORMAL ){
               uartWriteString( UART_DEBUG, "Lei un " );
               uartWriteByte( UART_DEBUG, rxData );
               uartWriteString( UART_DEBUG, " del buffer.\r\n" );
            }
         }
      }

      if(txData > '9')
         txData = '1';
   } 

   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta 
   // directamenteno sobre un microcontroladore y no es llamado/ por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
}

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

/*==================[fin del archivo]========================================*/

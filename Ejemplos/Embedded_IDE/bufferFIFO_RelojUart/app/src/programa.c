/*============================================================================
 * Licencia: BSD de 3 clausulas.
 * Autor: Eric Pernia.
 * Fecha: 2017-04-24
 *===========================================================================*/

/*==================[inlcusiones]============================================*/

#include "programa.h"   // <= su propio archivo de cabecera
#include "sapi.h"       // <= Biblioteca sAPI

/*==================[definiciones y macros]==================================*/

#define BUFF_TX_LENGHT 20

/*==================[definiciones de datos internos]=========================*/

/*==================[definiciones de datos externos]=========================*/

uint8_t seg=0, min=15, hs=11;

uint8_t buffTxIndiceLectura = 0, buffTxIndiceEscritura = 0;

uint8_t buffTxMemory[ BUFF_TX_LENGHT];

char CHARMAP[]="0123456789";

char relojString[]="\r\nRELOJ: HH:MM:SS";

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

void actualizarReloj(void);
void actualizarDriverUart(void);
void uartEscribirStringEnBuffer( char* STR_PTR );
void uartEscribirCaracterEnBuffer( char Data );

/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void ){

   // ---------- CONFIGURACIONES ------------------------------
  
   boardConfig(); // Inicializar y configurar la plataforma
   
   uartConfig( UART_USB, 115200 ); // Inicializar la UART
   
   // actualizar el driver de TX por UART cada 5ms
   delay_t delayDriverTx;
   delayConfig( &delayDriverTx, 50 );
   
   // actualizar reloj cada 1s
   delay_t delayReloj;
   delayConfig( &delayReloj, 1000 );
   
   // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE )
   {      
      // ejecutar tareas según corresponda
      
      // Si se cumple el tiempo de ejecutar la tarea del reloj
      // ejecutar tarea del reloj
      if( delayRead( &delayReloj) ){
         actualizarReloj();
      }
            
      // Si se cumple el tiempo de ejecutar la tarea de mandar buffer por UART
      // ejecutar tarea de mandar buffer por UART
      if( delayRead( &delayDriverTx) ){
         actualizarDriverUart();
      }
   } 

   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta 
   // directamenteno sobre un microcontroladore y no es llamado/ por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
}

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

void actualizarReloj( void ){
   // Actualizo hora
   if( ++seg==60 ){   
      seg=0;
      if( ++min==60 ){
         min=0;
         if( ++hs==24 ){
            hs=0;
         }
      }
   }
   // armo mensaje a visualizar
   relojString[9]  = CHARMAP[  hs / 10 ];
   relojString[10] = CHARMAP[  hs % 10 ];
   
   relojString[12] = CHARMAP[ min / 10 ];
   relojString[13] = CHARMAP[ min % 10 ];
   
   relojString[15] = CHARMAP[ seg / 10 ];
   relojString[16] = CHARMAP[ seg % 10 ];
   // Mando el mensaje al buffer
   uartEscribirStringEnBuffer( relojString );
}

void uartEscribirStringEnBuffer( char* STR_PTR ){
   char i = 0;
   while( STR_PTR [ i ] != '\0' ){
      uartEscribirCaracterEnBuffer( STR_PTR [ i ] );
      i++;
   }
}

void uartEscribirCaracterEnBuffer( char Data ){
   // Write to the buffer *only* if there is space
   if( buffTxIndiceEscritura < BUFF_TX_LENGHT ){
      buffTxMemory[ buffTxIndiceEscritura ]= Data;
      buffTxIndiceEscritura ++;
   }
   else{ // Write buffer is full
      uartWriteString( UART_USB, "Error, buffer lleno.\r\n" );
   }
}


void actualizarDriverUart( void ){
   // Hay byte en el buffer Tx para transmitir?
   if( buffTxIndiceLectura < buffTxIndiceEscritura ){
      uartWriteByte( UART_USB, buffTxMemory[buffTxIndiceLectura ] );
      buffTxIndiceLectura++;
   }
   else{ // No hay datos disponibles para enviar
      buffTxIndiceLectura = 0;
      buffTxIndiceEscritura = 0;
   }
}

/*==================[fin del archivo]========================================*/

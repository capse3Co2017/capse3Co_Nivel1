/*============================================================================
 * Licencia: 
 * Autor: 
 * Fecha: 
 *===========================================================================*/

/*==================[inlcusiones]============================================*/

#include "programa.h"   // <= su propio archivo de cabecera
#include "sapi.h"       // <= Biblioteca sAPI
#include "ff.h"
#include "magnetometroHMC588L.h"
#include "rtc.h"
#include "sd_spi.h"
#include <stdlib.h>
#include <string.h>

/*==================[definiciones y macros]==================================*/

#define FILENAME "hola.txt"
#define BAUD_RATE 115200

/*==================[definiciones de datos internos]=========================*/
static int16_t hmc5883l_x_raw;
static int16_t hmc5883l_y_raw;
static int16_t hmc5883l_z_raw;

static char buffer[50];
static delay_t miDelay;

static FIL fp;             // <-- File object needed for each open file


/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/
void putIntToBuf(char *buffer, int16_t i)
{
  itoa(i,buffer,10);
}

/*==================[declaraciones de funciones externas]====================*/

/*==================[funcion principal]======================================*/

int main( void ){

   // ---------- CONFIGURACIONES ------------------------------
   // Inicializar y configurar la plataforma
   boardConfig();   
   /* Inicializar Uart */
   sdInit();
   delay(500);
   uartConfig(UART_USB, BAUD_RATE);
   uartWriteString(UART_USB,"Paso1\r\n");
   magnetometroInit();
   delay(100);
   uartWriteString(UART_USB,"Paso2\r\n");
   rtcInit();
   delay(100);
   uartWriteString(UART_USB,"Paso3\r\n");
   delayConfig( &miDelay, 1000);
   int i=0;
   
   // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE )
   {

     
     if ( delayRead( &miDelay ) ){
         

         // Intercambia el valor del LEDB
         gpioToggle( LED1 );

         // Se debe esperar minimo 67ms entre lecturas su la tasa es de 15Hz
         // para leer un nuevo valor del magnet�metro
         //delay(67);
         hmc5883lRead( &hmc5883l_x_raw, &hmc5883l_y_raw, &hmc5883l_z_raw );
         
         getDateHour(buffer);
         buffer[strlen(buffer)+1]=0;
         buffer[strlen(buffer)]=':';
         putIntToBuf(&buffer[strlen(buffer)],hmc5883l_x_raw);
         buffer[strlen(buffer)+1]=0;
         buffer[strlen(buffer)]=':';
         putIntToBuf(&buffer[strlen(buffer)],hmc5883l_y_raw);
         buffer[strlen(buffer)+1]=0;
         buffer[strlen(buffer)]=':';
         putIntToBuf(&buffer[strlen(buffer)],hmc5883l_z_raw);
         buffer[strlen(buffer)+1]=buffer[strlen(buffer)+2]=0;
         buffer[strlen(buffer)]='\r';
         buffer[strlen(buffer)]='\n';
                  
         uartWriteString(UART_USB,buffer);
         uartWriteString(UART_USB,"\r\n");
         sendHmc5883lToUart( hmc5883l_x_raw, 'x' );
         sendHmc5883lToUart( hmc5883l_y_raw, 'y' );
         sendHmc5883lToUart( hmc5883l_z_raw, 'z' );
         uartWriteString( UART_USB, "\r\n");
         UINT nbytes;
   
         hmc5883l_x_raw=0;
         hmc5883l_y_raw=0;
         hmc5883l_z_raw=0;

         if (i>10) continue;
         
         // Create/open a file, then write a string and close it
         if( f_open( &fp, FILENAME, FA_WRITE | FA_OPEN_APPEND ) == FR_OK ){
           f_write( &fp, buffer, strlen(buffer), &nbytes );
           f_close(&fp);
           if( nbytes == strlen(buffer) ){
             // Turn ON LEDG if the write operation was successful
             gpioWrite( LEDG, ON );
           }
         } else{
           // Turn ON LEDR if the write operation was fail
           gpioWrite( LEDR, ON );
         }
         i++;
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

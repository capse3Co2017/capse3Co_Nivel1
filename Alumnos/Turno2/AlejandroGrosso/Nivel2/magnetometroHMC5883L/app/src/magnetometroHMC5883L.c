/*============================================================================
 * Autor: Eric Pernia.
 * Fecha: 2017/05/18
 * Licencia: BSD de 3 clausulas.
 *===========================================================================*/

/*==================[inlcusiones]============================================*/

#include "programa.h"   // <= su propio archivo de cabecera
#include "sapi.h"       // <= Biblioteca sAPI

#include <stdlib.h>
#include <math.h>       // <= Funciones matematicas


/*==================[definiciones y macros]==================================*/

#define BAUD_RATE 	115200
#define PI		3.14159265

/*==================[definiciones de datos internos]=========================*/

static int16_t hmc5883l_x_raw;
static int16_t hmc5883l_y_raw;
static int16_t hmc5883l_z_raw;

/* Buffers */
static uint8_t uartBuff[10];

/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/
static uint8_t calcularAngulo(int16_t x, int16_t y)
{
  float a=atan2((float) y, (float) x);
  
  if (a < 0)
    a=2*PI+a;
  a=a*255/PI;
  return (uint8_t)a;
}
/*==================[declaraciones de funciones externas]====================*/

void sendHmc5883lToUart( int16_t axis, uint8_t axisName );

/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void ){

   // ---------- CONFIGURACIONES ------------------------------
   // Inicializar y configurar la plataforma
   boardConfig();   
   
   // Inicializar HMC5883L
   HMC5883L_config_t hmc5883L_configValue;

   hmc5883lPrepareDefaultConfig( &hmc5883L_configValue );

   hmc5883L_configValue.mode = HMC5883L_continuous_measurement;
   hmc5883L_configValue.samples = HMC5883L_8_sample;

   hmc5883lConfig( hmc5883L_configValue );
   servoConfig(0,SERVO_ENABLE);
   servoConfig(SERVO0, SERVO_ENABLE_OUTPUT);

   /* Inicializar Uart */
   uartConfig(UART_USB, BAUD_RATE);

   delay_t miDalay;
   delay_t servoDelay;
   uint8_t angulo=0;

   delayConfig( &miDalay, 1000 );
   delayConfig( &servoDelay, 100);
   
   // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE )
   {
     if ( delayRead(&servoDelay) ){
       hmc5883lRead( &hmc5883l_x_raw, &hmc5883l_y_raw, &hmc5883l_z_raw );

       angulo=calcularAngulo(hmc5883l_x_raw, hmc5883l_y_raw);
       servoWrite( SERVO0, angulo );
     }
     
     if ( delayRead( &miDalay ) ){
         
         // Se debe esperar minimo 67ms entre lecturas su la tasa es de 15Hz
         // para leer un nuevo valor del magnet�metro
         //delay(67);
         
         sendHmc5883lToUart( hmc5883l_x_raw, 'x' );
         sendHmc5883lToUart( hmc5883l_y_raw, 'y' );
         sendHmc5883lToUart( hmc5883l_z_raw, 'z' );
         uartWriteString( UART_USB, "Angulo : ");
         itoa( (((int) angulo)*180)/255, uartBuff, 10 );
         uartBuff[3]='\r';uartBuff[4]='\n';uartBuff[5]=0;
         uartWriteString( UART_USB, uartBuff);
         uartWriteString( UART_USB, "\r\n");
         hmc5883l_x_raw=0;
         hmc5883l_y_raw=0;
         hmc5883l_z_raw=0;
         
      }
   } 

   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta 
   // directamenteno sobre un microcontroladore y no es llamado/ por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
}

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

void sendHmc5883lToUart( int16_t axis, uint8_t axisName ){
   
   /* Envio la primer parte dle mensaje a la Uart */
   uartWriteString( UART_USB, "HMC5883L eje ");
   uartWriteByte( UART_USB, axisName );
   uartWriteString( UART_USB, ": ");

   /* Conversion de muestra entera a ascii con base decimal */
   itoa( (int) axis, uartBuff, 10 ); /* 10 significa decimal */
   
   /* Envio el valor del eje */
   uartBuff[4] = 0;    /* NULL */
   uartWriteString( UART_USB, uartBuff );
   
   /* Envio un 'enter' */
   uartWriteString( UART_USB, "\r\n");
}

/*==================[fin del archivo]========================================*/

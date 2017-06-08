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

static   HMC5883L_config_t hmc5883L_configValue;

/* Buffers */
static uint8_t uartBuff[10];

/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

void sendHmc5883lToUart( int16_t axis, uint8_t axisName );

/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.

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

void magnetometroInit(void)
{
   // Inicializar HMC5883L

   hmc5883lPrepareDefaultConfig( &hmc5883L_configValue );

   hmc5883L_configValue.mode = HMC5883L_continuous_measurement;
   hmc5883L_configValue.samples = HMC5883L_8_sample;

   hmc5883lConfig( hmc5883L_configValue );

}
/*==================[fin del archivo]========================================*/

/*============================================================================
 * Autor: Eric Pernia.
 * Fecha: 2017/05/18
 * Licencia: BSD de 3 clausulas.
 *===========================================================================*/

/*==================[inlcusiones]============================================*/

#include "programa.h"   // <= su propio archivo de cabecera
#include "sapi.h"       // <= Biblioteca sAPI

#include <math.h>       // <= Funciones matematicas

/*==================[definiciones y macros]==================================*/

#define BAUD_RATE 115200

/*==================[definiciones de datos internos]=========================*/

static int16_t hmc5883l_x_raw;
static int16_t hmc5883l_y_raw;
static int16_t hmc5883l_z_raw;

static int16_t angle = 0;

/* Buffers */
static uint8_t uartBuff[10];

/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

void sendAngleToUart( void );

void sendHmc5883lToUart( int16_t axis, uint8_t axisName );

int16_t vectorR2ToAngle( int16_t axis1, int16_t axis2 );

uint8_t* angleToDirection( int16_t angle );

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

   /* Inicializar Uart */
   uartConfig(UART_USB, BAUD_RATE);

   /* Configurar Servo */
   servoConfig( 0,      SERVO_ENABLE );
   servoConfig( SERVO0, SERVO_ENABLE_OUTPUT );

   delay_t miDalay;

   delayConfig( &miDalay, 1000 );
   
   // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE )
   {      
      
	   if( delayRead( &miDalay ) ){

         hmc5883lRead( &hmc5883l_x_raw, &hmc5883l_y_raw, &hmc5883l_z_raw );
         // Se debe esperar minimo 67ms entre lecturas su la tasa es de 15Hz
         // para leer un nuevo valor del magnetómetro
         //delay(67);

         angle = vectorR2ToAngle( hmc5883l_x_raw, hmc5883l_y_raw );

         // Usar Servo
         servoWrite( SERVO0, (uint8_t)angle );
         
         sendHmc5883lToUart( hmc5883l_x_raw, 'x' );
         sendHmc5883lToUart( hmc5883l_y_raw, 'y' );
         sendHmc5883lToUart( hmc5883l_z_raw, 'z' );
         
         sendAngleToUart();

         uartWriteString( UART_USB, "Apunta al ");
         uartWriteString( UART_USB,  angleToDirection( angle )  );
         /* Envio 2 'enter' */
         uartWriteString( UART_USB, "\r\n");
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

void sendAngleToUart( void ){
   
   /* Envio la primer parte dle mensaje a la Uart */
   uartWriteString( UART_USB, "Angulo: ");

   /* Conversion de muestra entera a ascii con base decimal */
   itoa( (int) angle, uartBuff, 10 ); /* 10 significa decimal */
   
   /* Envio el valor del eje */
   uartBuff[4] = 0;    /* NULL */
   uartWriteString( UART_USB, uartBuff );
   
   /* Envio un 'enter' */
   uartWriteString( UART_USB, "\r\n");
}

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


int16_t vectorR2ToAngle( int16_t axis1, int16_t axis2 ){

	float angle = 0;
	float heading = 0;

	// Angulo en radianes
	heading = atan2( (float)axis2, (float)axis1 );

	// Angulo en radianes ajustado
	if( heading < 0.0 ){
		heading += 2.0 * 3.1415;
	}

	// Angulo en grados
	angle = (heading * 180.0 / 3.1415); //M_PI

	return (int16_t)angle;
}


uint8_t* angleToDirection( int16_t angle ){

   if (angle >= 360) {
      angle %= 360;
   }

   if (angle >= 337 || angle < 23) {
      return (char*)"Norte";
   } else if (angle < 68 && angle >= 23) {
      return (char*)"Noreste";
   } else if (angle >= 68 && angle < 113) {
      return (char*)"Este";
   } else if (angle >= 113 && angle < 158) {
      return (char*)"Sudeste";
   } else if (angle < 203 && angle >= 158) {
      return (char*)"Sur";
   } else if (angle >= 203 && angle < 248) {
      return (char*)"Suroeste";
   } else if (angle < 293 && angle >= 248) {
      return (char*)"Oeste";
   } else if (angle >= 293 && angle < 337) {
      return (char*)"Noroeste";
   }

}

/* Copyright 2016, Alejandro Permingeat.
 * Copyright 2016, Eric Pernia.
 * All rights reserved.
 *
 * This file is part sAPI library for microcontrollers.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*
 * Date: 2016-07-28
 */

/*==================[inclusions]=============================================*/

#include "programa.h"         /* <= own header */

#include "sAPI.h"         /* <= sAPI header */

#include "math.h"         /* <= Funciones matematicas */

/*==================[macros and definitions]=================================*/

#define BAUD_RATE 115200
//#define M_PI 3.141516

/*==================[internal data declaration]==============================*/

static int16_t hmc5883l_x_raw;
static int16_t hmc5883l_y_raw;
static int16_t hmc5883l_z_raw;

/* Buffers */
static uint8_t uartBuff[10];

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

void sendHmc5883lToUart( int16_t axis, uint8_t axisName ){
   
   /* Envio la primer parte dle mensaje a la Uart */
   uartWriteString( UART_USB, (uint8_t*) "HMC5883L eje ");
   uartWriteByte( UART_USB, axisName );
   uartWriteString( UART_USB, (uint8_t*) ": ");

   /* Conversion de muestra entera a ascii con base decimal */
   itoa( (int) axis, (char*)uartBuff, 10 ); /* 10 significa decimal */
   
   /* Envio el valor del eje */
   uartBuff[4] = 0;    /* NULL */
   uartWriteString( UART_USB, uartBuff );
   
   /* Envio un 'enter' */
   uartWriteString( UART_USB, (uint8_t*) "\r\n");
}


int16_t vectorR2ToAngle( int16_t axis1, int16_t axis2 ){

	int16_t angle = 0;
	double heading = 0;

	// Angulo en radianes
	heading = atan2( axis2, axis1 );

	// Angulo en radianes ajustado
	if( heading < 0 ){
		heading += 2* M_PI;
	}

	// Angulo en grados
	angle = (int16_t)(heading * 180.0 / M_PI);

	return angle;
}


uint8_t* angleToDirection( int16_t angle ){

   if (angle >= 360) {
      angle %= 360;
   }

   if (angle >= 338 || angle < 23) {
      return (uint8_t*)"Norte";
   } else if (angle < 68) {
      return (uint8_t*)"Noreste";
   } else if (angle < 113) {
      return (uint8_t*)"Este";
   } else if (angle < 158) {
      return (uint8_t*)"Sudeste";
   } else if (angle < 203) {
      return (uint8_t*)"Sur";
   } else if (angle < 248) {
      return (uint8_t*)"Suroeste";
   } else if (angle < 293) {
      return (uint8_t*)"Oeste";
   } else {
      return (uint8_t*)"Noroeste";
   }

}


/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void){

   /* ------------- INICIALIZACIONES ------------- */

   /* Inicializar la placa */
   boardConfig();
   tickConfig( 1, 0 );
   /* Inicializar HMC5883L */
   HMC5883L_config_t hmc5883L_configValue;

   hmc5883lPrepareDefaultConfig( &hmc5883L_configValue );

   hmc5883L_configValue.mode = HMC5883L_continuous_measurement;
   hmc5883L_configValue.samples = HMC5883L_8_sample;

   hmc5883lConfig( hmc5883L_configValue );

   /* Inicializar Uart */
   uartConfig(UART_USB, BAUD_RATE);

   int16_t angle = 0;

   /* Configurar Servo */
   servoConfig( 0,      SERVO_ENABLE );
   servoConfig( SERVO0, SERVO_ENABLE_OUTPUT );

   delay_t miDalay;

   delayConfig( &miDalay, 1000 );

   /* ------------- REPETIR POR SIEMPRE ------------- */
   while(1) {

	  hmc5883lRead( &hmc5883l_x_raw, &hmc5883l_y_raw, &hmc5883l_z_raw );
	  /* Se debe esperar minimo 67ms entre lecturas su la tasa es de 15Hz
     	para leer un nuevo valor del magnetómetro */
	  delay(100);

	  angle = vectorR2ToAngle( hmc5883l_x_raw, hmc5883l_y_raw );

	   /* Usar Servo */
	   servoWrite( SERVO0, (uint8_t)angle );

	   if( delayRead( &miDalay ) ){

		  sendHmc5883lToUart( hmc5883l_x_raw, 'x' );
		  sendHmc5883lToUart( hmc5883l_y_raw, 'y' );
		  sendHmc5883lToUart( hmc5883l_z_raw, 'z' );

		  uartWriteString( UART_USB,  "Apunta al ");
		  uartWriteString( UART_USB,  angleToDirection( angle )  );
		  /* Envio 2 'enter' */
		  uartWriteString( UART_USB,  "\r\n");
		  uartWriteString( UART_USB,  "\r\n");

	   }

   }

   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
   return 0 ;
}

/*==================[end of file]============================================*/

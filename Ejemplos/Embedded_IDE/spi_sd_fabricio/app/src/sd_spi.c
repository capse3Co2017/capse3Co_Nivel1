/* Copyright 2014, ChaN
 * Copyright 2016, Matias Marando
 * Copyright 2016, Eric Pernia
 * All rights reserved.
 *
 * This file is part of Workspace.
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
 
/*==================[inlcusiones]============================================*/

#include "sd_spi.h"   // <= su propio archivo de cabecera
#include "sapi.h"     // <= Biblioteca sAPI

#include <math.h>         /* <= Funciones matematicas */

#include "ff.h"       // <= Biblioteca FAT FS

/*==================[definiciones y macros]==================================*/

#define FILENAME "hola.txt"
#define BAUD_RATE 115200

/*==================[definiciones de datos internos]=========================*/

static FATFS fs;           // <-- FatFs work area needed for each volume
static FIL fp;             // <-- File object needed for each open file

static int16_t hmc5883l_x_raw;
static int16_t hmc5883l_y_raw;
static int16_t hmc5883l_z_raw;

static uint8_t uartBuff[10];
/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/
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


// FUNCION que se ejecuta cada vezque ocurre un Tick
bool_t diskTickHook( void *ptr );


/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void ){

   // ---------- CONFIGURACIONES ------------------------------
   // Inicializar y configurar la plataforma
   boardConfig();

   // SPI configuration
   spiConfig(SPI0);
   
   /* Inicializar HMC5883L */
   HMC5883L_config_t hmc5883L_configValue;
   hmc5883lPrepareDefaultConfig( &hmc5883L_configValue );

   hmc5883L_configValue.mode = HMC5883L_continuous_measurement;
   hmc5883L_configValue.samples = HMC5883L_8_sample;

   hmc5883lConfig( hmc5883L_configValue );

   /* Inicializar Uart */
   uartConfig(UART_USB, BAUD_RATE);
   
   int16_t angle = 0;

     // Inicializar el conteo de Ticks con resoluci?n de 10ms, 
   // con tickHook diskTickHook
   tickConfig( 10, diskTickHook );
   
   delay_t miDalay;

   delayConfig( &miDalay, 1000 );
          
   // ------ PROGRAMA QUE ESCRIBE EN LA SD -------

   UINT nbytes;
   
   // Give a work area to the default drive
   if( f_mount( &fs, "", 0 ) != FR_OK ){
      // If this fails, it means that the function could
      // not register a file system object.
      // Check whether the SD card is correctly connected
   }

   // Create/open a file, then write a string and close it
   if( f_open( &fp, FILENAME, FA_WRITE | FA_CREATE_ALWAYS ) == FR_OK ){
      f_write( &fp, "Hola mundo\r\n", 12, &nbytes );

      f_close(&fp);

      if( nbytes == 12 ){
         // Turn ON LEDG if the write operation was successful
         gpioWrite( LEDG, ON );
      }
   } else{
      // Turn ON LEDR if the write operation was fail
      gpioWrite( LEDR, ON );
   }
   
   // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE )
   {      
    //  sleepUntilNextInterrupt();
      hmc5883lRead( &hmc5883l_x_raw, &hmc5883l_y_raw, &hmc5883l_z_raw );
	  /* Se debe esperar minimo 67ms entre lecturas su la tasa es de 15Hz
     	para leer un nuevo valor del magnetómetro */
      delay(100);
      if( delayRead( &miDalay ) ){

		  sendHmc5883lToUart( hmc5883l_x_raw, 'x' );
		  sendHmc5883lToUart( hmc5883l_y_raw, 'y' );
		  sendHmc5883lToUart( hmc5883l_z_raw, 'z' );

		  uartWriteString( UART_USB, (uint8_t*) "Apunta al ");
		  uartWriteString( UART_USB,  angleToDirection( angle )  );
		  /* Envio 2 'enter' */
		  uartWriteString( UART_USB, (uint8_t*) "\r\n");
		  uartWriteString( UART_USB, (uint8_t*) "\r\n");

	   }      

   // if( f_open( &fp, FILENAME, FA_WRITE | FA_OPEN_APPEND ) == FR_OK ){
   //   f_write( &fp, "Hola mundo\r\n", 12, &nbytes );

   //   f_close(&fp);
   //} 

   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta 
   // directamenteno sobre un microcontroladore y no es llamado/ por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
}

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

// FUNCION que se ejecuta cada vezque ocurre un Tick
bool_t diskTickHook( void *ptr ){
   disk_timerproc();   // Disk timer process
   return 1;
}


/*==================[fin del archivo]========================================*/

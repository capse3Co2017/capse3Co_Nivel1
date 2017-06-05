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

#define FILENAME "DATOS.TXT"
#define BAUD_RATE 115200

/*==================[definiciones de datos internos]=========================*/

static FATFS fs;           // <-- FatFs work area needed for each volume
static FIL fp;             // <-- File object needed for each open file

static int16_t hmc5883l_x_raw;
static int16_t hmc5883l_y_raw;
static int16_t hmc5883l_z_raw;

static uint8_t uartBuff[10];

// Estructura RTC
rtc_t rtc;
UINT nbytes;
/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/
/**
 * C++ version 0.4 char* style "itoa":
 * Written by Luk?s Chmela
 * Released under GPLv3.
 */
char* itoa(int value, char* result, int base);

/* Enviar fecha y hora en formato "DD/MM/YYYY, HH:MM:SS" */
void showDateAndTime( rtc_t * rtc );

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

// FUNCION que se ejecuta cada vezque ocurre un Tick
bool_t diskTickHook( void *ptr );


/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
/**
 * @mainpage PROYECTO
 * El presente proyecto graba en la memoria SD los datos obtenidos del reloj RTC (timestamp) y Magnetometro (timeStamp)
 * con el siguiente formato:\n
 * \n
 *      DD/MM/YYYY HH:mm:ss HMC5883L ejes x:   y:   z:
 * \n
 * \n
 * Se introdujo la funcionalidad de encender el led verde de la placa cada vez que se graba un dato
 * en la memoria SD (aproximadamente cada segundo).
 * \n
 * \n
 * Se utilizan las bibliotecas de provistas por al sAPI para el manejo de perifericos como la memoria SD, magnetometro y reloj RTC.
 *
 */
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
   
   rtc.year = 2017;
   rtc.month = 10;
   rtc.mday = 31;
   rtc.wday = 3;
   rtc.hour = 21;
   rtc.min = 50;
   rtc.sec= 12;

   bool_t val = 0;
   // Inicializar RTC
   val = rtcConfig( &rtc );
   
   int16_t angle = 0;

   // Inicializar el conteo de Ticks con resoluci?n de 10ms, 
   // con tickHook diskTickHook
   tickConfig( 10, diskTickHook );
   
   delay_t miDelay;

   delayConfig( &miDelay, 1000 );
          
   // ------ PROGRAMA QUE ESCRIBE EN LA SD -------
 
   // Give a work area to the default drive
   if( f_mount( &fs, "", 0 ) != FR_OK ){
      // If this fails, it means that the function could
      // not register a file system object.
      // Check whether the SD card is correctly connected
   }
  
   // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE )
   {      
          
      hmc5883lRead( &hmc5883l_x_raw, &hmc5883l_y_raw, &hmc5883l_z_raw );
	  /* Se debe esperar minimo 67ms entre lecturas su la tasa es de 15Hz
     	para leer un nuevo valor del magnet?metro */
      delay(100);
      if( delayRead( &miDelay ) ){
		if( f_open( &fp, FILENAME, FA_WRITE | FA_OPEN_APPEND ) == FR_OK ){
                                 gpioWrite( LEDG, ON );
                                 val = rtcRead( &rtc );	
		  	   showDateAndTime( &rtc );
		  	    
			   itoa( (int) hmc5883l_x_raw, (char*)uartBuff, 10 ); /* 10 significa decimal */
		             f_write( &fp, "HMC5883L ejes x:", 16, &nbytes);
		             f_write( &fp, uartBuff, 4, &nbytes );   
                                 
                                 itoa( (int) hmc5883l_y_raw, (char*)uartBuff, 10 );
                                 f_write( &fp, " y:", 3, &nbytes);
		             f_write( &fp, uartBuff, 4, &nbytes );   
                                 
                                 itoa( (int) hmc5883l_z_raw, (char*)uartBuff, 10 );
                                 f_write( &fp, " z:", 3, &nbytes);
		             f_write( &fp, uartBuff, 4, &nbytes );   
                                 
                                 f_write( &fp, "\r\n", 2, &nbytes );
                                 f_close(&fp);
		             gpioWrite( LEDG, OFF );		             
		}
       }      
   } 

   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta 
   // directamenteno sobre un microcontroladore y no es llamado/ por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
}

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/
/**
 * C++ version 0.4 char* style "itoa":
 * Written by Luk?s Chmela
 * Released under GPLv3.
 */
char* itoa(int value, char* result, int base) {
   // check that the base if valid
   if (base < 2 || base > 36) { *result = '\0'; return result; }

   char* ptr = result, *ptr1 = result, tmp_char;
   int tmp_value;

   do {
      tmp_value = value;
      value /= base;
      *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
   } while ( value );

   // Apply negative sign
   if (tmp_value < 0) *ptr++ = '-';
   *ptr-- = '\0';
   while(ptr1 < ptr) {
      tmp_char = *ptr;
      *ptr--= *ptr1;
      *ptr1++ = tmp_char;
   }
   return result;
}

void showDateAndTime( rtc_t * rtc ){
   /* Conversion de entero a ascii con base decimal */
   itoa( (int) (rtc->mday), (char*)uartBuff, 10 );
   if( (rtc->mday)<10 ){
       f_write( &fp, "0", 1, &nbytes );
   }
   f_write( &fp, uartBuff, 2, &nbytes );
   f_write( &fp, "/", 1, &nbytes );
   
   itoa( (int) (rtc->month), (char*)uartBuff, 10 );
   if( (rtc->month)<10 ){
       f_write( &fp, "0", 1, &nbytes );   
   }
   f_write( &fp, uartBuff, 2, &nbytes );
   f_write( &fp, "/", 1, &nbytes );
   
   itoa( (int) (rtc->year), (char*)uartBuff, 10 );
   if( (rtc->year)<10 ){
       f_write( &fp, "0", 1, &nbytes );
   }
   f_write( &fp, uartBuff, 4, &nbytes );
   f_write( &fp, " ", 1, &nbytes );
   
   itoa( (int) (rtc->hour), (char*)uartBuff, 10 );
   if( (rtc->hour)<10 ){
       f_write( &fp, "0", 1, &nbytes );
   }
   f_write( &fp, uartBuff, 2, &nbytes );
   f_write( &fp, ":", 1, &nbytes );
   
   itoa( (int) (rtc->min), (char*)uartBuff, 10 ); 
   if( (rtc->min)<10 ){
       f_write( &fp, "0", 1, &nbytes );
   }
   f_write( &fp, uartBuff, 2, &nbytes );
   f_write( &fp, ":", 1, &nbytes );
   
   itoa( (int) (rtc->sec), (char*)uartBuff, 10 );
   if( (rtc->sec)<10 ){
       f_write( &fp, "0", 1, &nbytes );
   }
   f_write( &fp, uartBuff, 2, &nbytes );
   f_write( &fp, " ", 1, &nbytes );
 
}

// FUNCION que se ejecuta cada vezque ocurre un Tick
bool_t diskTickHook( void *ptr ){
   disk_timerproc();   // Disk timer process
   return 1;
}


/*==================[fin del archivo]========================================*/

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
#include "ff.h"       // <= Biblioteca FAT FS

/*==================[definiciones y macros]==================================*/

#define FILETOREAD "ld_cfg.txt"
#define FILENAME "capse.txt"

/*==================[definiciones de datos internos]=========================*/

static FATFS fs;           // <-- FatFs work area needed for each volume
static FIL fpConfig;             // <-- File object needed for each open file
static FIL fp;
char buff1[100];

/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/


/*==================[declaraciones de funciones externas]====================*/

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
   
   gpioConfig( LEDR, GPIO_OUTPUT );
   gpioConfig( LEDG, GPIO_OUTPUT );
   gpioConfig( LEDB, GPIO_OUTPUT );
   gpioConfig( LED1, GPIO_OUTPUT );
   gpioConfig( LED2, GPIO_OUTPUT );
   gpioConfig( LED3, GPIO_OUTPUT);

   // Inicializar el conteo de Ticks con resolución de 10ms, 
   // con tickHook diskTickHook
   tickConfig( 10, diskTickHook );
             
   UINT bytesRead;
   UINT nbytes;
   
   // Give a work area to the default drive
   if( f_mount( &fs, "", 0 ) != FR_OK ){
	   gpioWrite( LEDR, ON );
	   // If this fails, it means that the function could
	   // not register a file system object.
	   // Check whether the SD card is correctly connected
   }
   FRESULT result;  //prueba de escritura
   result= f_open( &fp, FILENAME, FA_WRITE | FA_OPEN_APPEND );
	if( f_open( &fp, FILENAME, FA_WRITE | FA_OPEN_APPEND ) == FR_OK ){
		f_write( &fp,
				"CAPSE: Usamos SPI y FileSystem para grabar datos en una memoria SD\r\n",
				68,
				&nbytes
		);
		gpioWrite( LED2,ON );
		f_close(&fp);
	} else{
		// Turn on if fails */
		gpioWrite( LED3, ON );
	}
	FRESULT result2;  //prueba de lectura
   result2=f_open(&fpConfig, FILETOREAD, FA_READ | FA_OPEN_ALWAYS);
   if (f_open(&fpConfig, FILETOREAD, FA_READ | FA_OPEN_ALWAYS)== FR_OK){
	   bytesRead = f_size(&fpConfig);
	   f_read(&fpConfig,&buff1,bytesRead,&bytesRead);
	   f_close(&fpConfig);
		gpioWrite( LEDG, ON );
   }
   else {
	   // Turn ON LEDR if the write operation was fail
	   gpioWrite( LEDR, ON );
   }

   // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE )
   {
	      sleepUntilNextInterrupt();

   } 

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

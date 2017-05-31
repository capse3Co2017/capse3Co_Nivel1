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

#define FILENAME "Datos.txt"

/*==================[definiciones de datos internos]=========================*/

static FATFS fs;           // <-- FatFs work area needed for each volume
static FIL fp;             // <-- File object needed for each open file

/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/
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
   
   // Inicializar el conteo de Ticks con resolución de 10ms, 
   // con tickHook diskTickHook
   tickConfig( 10, diskTickHook );
             
   // ------ PROGRAMA QUE ESCRIBE EN LA SD -------

   UINT nbytes;
   
   // Give a work area to the default drive
   if( f_mount( &fs, "", 0 ) != FR_OK ){
      // If this fails, it means that the function could
      // not register a file system object.
      // Check whether the SD card is correctly connected
   }

   delay_t ciclo;
   delayConfig(&ciclo,3000);
   char Buffer[] = "Datos X=    ,Y=    ,Z=    \r\n";
   char itoBuff[10];
   int16_t hmc5883l_x_raw;
   int16_t hmc5883l_y_raw;
   int16_t hmc5883l_z_raw;
   uint8_t i;
   int16_t incr=0;
   // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE )
   {
	   hmc5883l_x_raw=incr;
	   hmc5883l_y_raw=incr/13;
	   hmc5883l_z_raw=incr/200;
	   incr=incr+1;
	   if (incr>9999) incr=0;

    if (delayRead(&ciclo)){
    	gpioWrite( LEDR, OFF );
    	gpioWrite( LEDG, OFF );
 	   itoBuff[10]="";
 	   itoa( hmc5883l_x_raw, itoBuff, 10 ); /* base 10 significa decimal */
 	   for (i=0; i<=3;i++){
 		   if (itoBuff[i]!=0)  Buffer[8+i]= itoBuff[i];
 		   else Buffer[8+i]= 32;
 	   }
 	   itoBuff[10]="";
 	   itoa( hmc5883l_y_raw, itoBuff, 10 ); /* base 10 significa decimal */
 	   for (i=0; i<=3;i++){
 		   if (itoBuff[i]!=0)  Buffer[15+i]= itoBuff[i];
 		   else Buffer[15+i]= 32;
 	   }
 	   itoBuff[10]="";
 	   itoa( hmc5883l_z_raw, itoBuff, 10 ); /* base 10 significa decimal */
 	   for (i=0; i<=3;i++){
 		   if (itoBuff[i]!=0)  Buffer[22+i]= itoBuff[i];
 		   else Buffer[22+i]= 32;
 	   }

    	   // Create/open a file, then write a string and close it
    	   if( f_open( &fp, FILENAME, FA_WRITE | FA_OPEN_APPEND ) == FR_OK ){
    	      f_write( &fp, Buffer, 28, &nbytes );

    	      f_close(&fp);

    	      if( nbytes == 28 ){
    	         // Turn ON LEDG if the write operation was successful
    	         gpioWrite( LEDG, ON );
    	      }
    	   } else{
    	      // Turn ON LEDR if the write operation was fail
    	      gpioWrite( LEDR, ON );
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

// FUNCION que se ejecuta cada vezque ocurre un Tick
bool_t diskTickHook( void *ptr ){
   disk_timerproc();   // Disk timer process
   return 1;
}


/*==================[fin del archivo]========================================*/

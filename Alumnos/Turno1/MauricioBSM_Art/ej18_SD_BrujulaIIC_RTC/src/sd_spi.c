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
#include "rtc.h"
#include "sd_spi.h"   // <= su propio archivo de cabecera
#include "sapi.h"     // <= Biblioteca sAPI
#include "ff.h"       // <= Biblioteca FAT FS

/*==================[definiciones y macros]==================================*/

#define FILENAME "Datos.txt"
#define DATOSX 26
#define DATOSY DATOSX + 7
#define DATOSZ DATOSY + 7
#define DATOSD 0
#define DATOSH 12
#define BUFFERTAM 47

/*==================[definiciones de datos internos]=========================*/

static FATFS fs;           // <-- FatFs work area needed for each volume
static FIL fp;             // <-- File object needed for each open file
            //   012345678901234567890123456789012345678901234567   para calcular posicion y tama�o
char Buffer[] = "DD/MM/YYYY, HH:MM:SS => X=    ,Y=    ,Z=    \r\n";
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
                       //   0123456789  0123456
/* fecha y hora en formato "DD/MM/YYYY, HH:MM:SS" */
void cargarDatosRTC( rtc_t * rtc ){
	   char Buff[]="    ";
	   itoa( (int) (rtc->mday), (char*)Buff, 10 );
	   if( (rtc->mday)<10 ) {
		   Buffer[DATOSD]=  '0';
		   Buffer[DATOSD+1]=  Buff[0];
	   }else{
		   Buffer[DATOSD]=  Buff[0];
		   Buffer[DATOSD+1]=  Buff[1];
	   }

	   itoa( (int) (rtc->month), (char*)Buff, 10 );
	   if( (rtc->month)<10 ) {
		   Buffer[DATOSD+3]=  '0';
		   Buffer[DATOSD+4]=  Buff[0];
	   }else{
		   Buffer[DATOSD+3]=  Buff[0];
		   Buffer[DATOSD+4]=  Buff[1];
	   }

	   itoa( (int) (rtc->year), (char*)Buff, 10 );
		   Buffer[DATOSD+6]=  Buff[0];
		   Buffer[DATOSD+7]=  Buff[1];
		   Buffer[DATOSD+8]=  Buff[2];
		   Buffer[DATOSD+9]=  Buff[3];

	   itoa( (int) (rtc->hour), (char*)Buff, 10 );
	   if( (rtc->hour)<10 ) {
		   Buffer[DATOSH]=  '0';
		   Buffer[DATOSH+1]=  Buff[0];
	   }else{
		   Buffer[DATOSH]=  Buff[0];
		   Buffer[DATOSH+1]=  Buff[1];
	   }

	   itoa( (int) (rtc->min), (char*)Buff, 10 );
	   if( (rtc->min)<10 ) {
		   Buffer[DATOSH+3]=  '0';
		   Buffer[DATOSH+4]=  Buff[0];
	   }else{
		   Buffer[DATOSH+3]=  Buff[0];
		   Buffer[DATOSH+4]=  Buff[1];
	   }

	   itoa( (int) (rtc->sec), (char*)Buff, 10 );
	   if( (rtc->sec)<10 ) {
		   Buffer[DATOSH+6]=  '0';
		   Buffer[DATOSH+7]=  Buff[0];
	   }else{
		   Buffer[DATOSH+6]=  Buff[0];
		   Buffer[DATOSH+7]=  Buff[1];
	   }
}

void cargarDatosHMC(int16_t ejeRaw,uint8_t Pos){
	  uint8_t i;
	  char itoBuffAux[]="    ";
	  itoa( ejeRaw, itoBuffAux, 10 ); /* base 10 significa decimal */
	  for (i=0; i<=3;i++){
		   if (itoBuffAux[i]!=0)  Buffer[Pos+i]= itoBuffAux[i];
		   else Buffer[8+i]= ' ';
	}
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
   
   // Inicializar el conteo de Ticks con resoluci�n de 10ms, 
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
   int16_t hmc5883l_x_raw;
   int16_t hmc5883l_y_raw;
   int16_t hmc5883l_z_raw;


   HMC5883L_config_t hmc5883L_configValue;
   hmc5883lPrepareDefaultConfig( &hmc5883L_configValue );
   hmc5883L_configValue.mode = HMC5883L_continuous_measurement;
   hmc5883L_configValue.samples = HMC5883L_8_sample;
   hmc5883lConfig( hmc5883L_configValue );

   rtc_t rtc;              /* Estructura RTC */
   rtc.year = 2017;
   rtc.month = 5;
   rtc.mday = 26;
   rtc.wday = 3;
   rtc.hour = 2;
   rtc.min = 30;
   rtc.sec= 0;
   rtcConfig( &rtc );     /* Inicializar RTC y pone en hora  */

   // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE )
   {
		  hmc5883lRead( &hmc5883l_x_raw, &hmc5883l_y_raw, &hmc5883l_z_raw );
		  /* Se debe esperar minimo 67ms entre lecturas su la tasa es de 15Hz
	     	para leer un nuevo valor del magnet�metro */
		  delay(100);

    if (delayRead(&ciclo)){
    	gpioWrite( LEDR, OFF );
    	gpioWrite( LEDG, OFF );

        rtcRead( &rtc );
        cargarDatosRTC( &rtc );
        cargarDatosHMC(hmc5883l_x_raw,DATOSX);
        cargarDatosHMC(hmc5883l_y_raw,DATOSY);
        cargarDatosHMC(hmc5883l_z_raw,DATOSZ);


    	   // Create/open a file, then write a string and close it
    	   if( f_open( &fp, FILENAME, FA_WRITE | FA_OPEN_APPEND ) == FR_OK ){
    	      f_write( &fp, Buffer, BUFFERTAM, &nbytes );

    	      f_close(&fp);

    	      if( nbytes == BUFFERTAM ){
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

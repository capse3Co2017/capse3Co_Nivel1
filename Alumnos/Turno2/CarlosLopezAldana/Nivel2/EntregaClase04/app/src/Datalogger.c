/*============================================================================
 * Copyright 2017 Carlos Eduardo López Aldana.
 * Copyright 2014, ChaN
 * Copyright 2016, Matias Marando
 * Copyright 2016, Eric Pernia
 
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
 
 * Date: 2017/06/02

 *===========================================================================*/

/*==================[inlcusiones]============================================*/

#include "sd_spi.h"   // <= su propio archivo de cabecera
#include "sapi.h"     // <= Biblioteca sAPI
#include <math.h>       // <= Funciones matematicas
#include "sapi_i2c.h"	// <= sAPI I2C header
#include "ff.h"       // <= Biblioteca FAT FS

/*==================[definiciones y macros]==================================*/

#define FILENAME "Datos.txt"
#define POSICION_MDAY 0
#define POSICION_MONTH 3
#define POSICION_YEAR 6
#define POSICION_HOUR 12
#define POSICION_MIN 15
#define POSICION_SEC 18
#define POSICION_X 26
#define POSICION_Y 35
#define POSICION_Z 44
#define POSICION_ANGULO 58
#define POSICION_APUNTA 71
#define TMUESTREO 2000


/*==================[definiciones de datos internos]=========================*/

static FATFS fs;           // <-- FatFs work area needed for each volume
static FIL fp;             // <-- File object needed for each open file
UINT nbytes;

/* Buffer */
static uint8_t dataBuff[10];
static uint8_t datosBuffer[] = "  /  /        :  :   - x=       y=       z=       Angulo=      Apunta=          \r\n";

static uint8_t b1[] = "Norte   ";
static uint8_t b2[] = "Noreste ";
static uint8_t b3[] = "Este    ";
static uint8_t b4[] = "Sudeste ";
static uint8_t b5[] = "Sur     ";
static uint8_t b6[] = "Suroeste";
static uint8_t b7[] = "Oeste   ";
static uint8_t b8[] = "Noroeste";


static int16_t hmc5883l_x_raw;
static int16_t hmc5883l_y_raw;
static int16_t hmc5883l_z_raw;
static int16_t angle = 0;

delay_t delay1s;




/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/
char* itoa(int value, char* result, int base);
void SendArchivoToUart(void);
void ArmaDateAndTime(rtc_t * rtc);
void Mpu6050Bypass(void);
void AlmacenaDatoRango(uint8_t dato, uint8_t x);
void CargaDatosBuffDisco(int16_t dato, uint8_t x);
void CargaBrujuBuffDisco(int16_t angle);
int16_t vectorR2ToAngle(int16_t axis1, int16_t axis2);
// FUNCION que se ejecuta cada vezque ocurre un Tick
bool_t diskTickHook(void *ptr);




/*==================[declaraciones de funciones externas]====================*/






/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main(void)
{
// ---------- CONFIGURACIONES ------------------------------
// Inicializar y configurar la plataforma
boardConfig();

// Se hace un bypass del MPI6050, Acelerómetro y Giroscopio
// acelerómetro digital de 3 ejes, medición hasta ±16g; y giroscopio digital de 3 ejes, medición hasta ±2000 º/s   
Mpu6050Bypass();   

// Inicializar HMC5883L
HMC5883L_config_t hmc5883L_configValue;
hmc5883lPrepareDefaultConfig(&hmc5883L_configValue);
hmc5883L_configValue.mode = HMC5883L_continuous_measurement;
hmc5883L_configValue.samples = HMC5883L_8_sample;
hmc5883lConfig(hmc5883L_configValue);

// Inicializar UART_USB a 115200 baudios
uartConfig(UART_USB, 115200);   

// Estructura RTC
rtc_t rtc;
rtc.year = 2017;
rtc.month = 6;
rtc.mday = 2;
rtc.wday = 0;
rtc.hour = 20;
rtc.min = 58;
rtc.sec= 0;
bool_t val = 0;
// Inicializar RTC
val = rtcConfig(&rtc);



// SPI configuration
spiConfig(SPI0);

// Inicializar el conteo de Ticks con resolución de 10ms, 
// con tickHook diskTickHook
tickConfig(10, diskTickHook);


delayConfig(&delay1s, TMUESTREO);


// ------ PROGRAMA QUE ESCRIBE EN LA SD -------
// Give a work area to the default drive
if (f_mount( &fs, "", 0) != FR_OK)
	{
	// If this fails, it means that the function could
	// not register a file system object.
	// Check whether the SD card is correctly connected
	}

   
   
// ---------- REPETIR POR SIEMPRE --------------------------
while(TRUE)
{      

//sleepUntilNextInterrupt();


if (delayRead(&delay1s))
	{
	gpioToggle(LED3);
	delayRead(&delay1s);


        hmc5883l_x_raw= 0;
        hmc5883l_y_raw= 0;
        hmc5883l_z_raw= 0;	

        hmc5883lRead(&hmc5883l_x_raw, &hmc5883l_y_raw, &hmc5883l_z_raw);
        // Se debe esperar minimo 67ms entre lecturas su la tasa es de 15Hz
        // para leer un nuevo valor del magnetómetro
        //delay(67);
        angle = vectorR2ToAngle(hmc5883l_x_raw, hmc5883l_y_raw);

	// Leer fecha y hora	
	val = rtcRead(&rtc);

	// Carga la fecha y hora en el buffer que se almacenara.
	ArmaDateAndTime(&rtc);

	//Carga los datos X, Y, Z y ANGULO en el buffer que se almacenara.
	CargaDatosBuffDisco(hmc5883l_x_raw, POSICION_X);
	CargaDatosBuffDisco(hmc5883l_y_raw, POSICION_Y);
	CargaDatosBuffDisco(hmc5883l_z_raw, POSICION_Z);
	CargaDatosBuffDisco(angle, POSICION_ANGULO);
	
	//Carga la direccion... Norte, Este, etc.
	CargaBrujuBuffDisco(angle);


	if (f_open(&fp, FILENAME, FA_WRITE | FA_OPEN_APPEND) == FR_OK)
		{
		f_write(&fp, datosBuffer, sizeof(datosBuffer)-1, &nbytes);
		f_close(&fp);
		if(nbytes == sizeof(datosBuffer)-1)
			{
			gpioWrite(LEDG, 1);
			}
		}
	else	{
		gpioWrite(LEDR, 1);
		}

	// Si se Presiona TECLA 1 se procede a enviar el contenido del archisvo "FILENAME".
	if (!gpioRead(TEC1))
		{
		uartWriteString(UART_USB, "Datos del archivo: ");   
		uartWriteString(UART_USB, FILENAME);   
		uartWriteString(UART_USB, "\r\n");    
		SendArchivoToUart();					// Envia archivo "FILENAME".
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





//*********************************************************************************************************************
//
//*********************************************************************************************************************
// FUNCION que se ejecuta cada vezque ocurre un Tick
bool_t diskTickHook(void *ptr)
{
disk_timerproc();   // Disk timer process
return 1;
}
//*********************************************************************************************************************
//*********************************************************************************************************************





//*********************************************************************************************************************
// C++ version 0.4 char* style "itoa":
// Written by Lukás Chmela
// Released under GPLv3.
//*********************************************************************************************************************
char* itoa(int value, char* result, int base)
{
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
//*********************************************************************************************************************
//*********************************************************************************************************************





//*********************************************************************************************************************
//
//*********************************************************************************************************************
void SendArchivoToUart(void)
{
uint8_t res = 0;
char buff[100] = {0};
UINT br = 0;

if (f_open(&fp, FILENAME, FA_READ) == FR_OK)
	{
	do{
	  res = f_read(&fp, buff, 100, &br);
	  if (res == FR_OK) 
		{
		gpioWrite(LED1, 0);
		gpioWrite(LED2, 0);
		buff[br] = 0;
		uartWriteString(UART_USB, buff);  
		}
	  else	{
		gpioWrite(LED1, 1);
		//return			//Salida si dio error!!!
		}
	  } while (br >= 100);
	
	f_close(&fp);
	}
else	{
	// Turn ON LEDR if the write operation was fail
	gpioWrite(LED2, 1);
	}
}
//*********************************************************************************************************************
//*********************************************************************************************************************





//*********************************************************************************************************************
//		Arma string "DD/MM/YYYY, HH:MM:SS"
//*********************************************************************************************************************
void ArmaDateAndTime(rtc_t * rtc)
{
// Dia
itoa((int) (rtc->mday), (char*)dataBuff, 10);
AlmacenaDatoRango((rtc->mday), POSICION_MDAY);

// Mes
itoa((int) (rtc->month), (char*)dataBuff, 10);
AlmacenaDatoRango((rtc->month), POSICION_MONTH);

// Año
itoa((int) (rtc->year), (char*)dataBuff, 10);
datosBuffer[POSICION_YEAR] = dataBuff[0];
datosBuffer[POSICION_YEAR+1] = dataBuff[1];
datosBuffer[POSICION_YEAR+2] = dataBuff[2];
datosBuffer[POSICION_YEAR+3] = dataBuff[3];

// Hora
itoa((int) (rtc->hour), (char*)dataBuff, 10);
AlmacenaDatoRango((rtc->hour), POSICION_HOUR);

// Minutos
itoa((int) (rtc->min), (char*)dataBuff, 10);
AlmacenaDatoRango((rtc->min), POSICION_MIN);

// Segundos
itoa((int) (rtc->sec), (char*)dataBuff, 10);
AlmacenaDatoRango((rtc->sec), POSICION_SEC);

}
//*********************************************************************************************************************
//*********************************************************************************************************************





//*********************************************************************************************************************
//
//*********************************************************************************************************************
void AlmacenaDatoRango(uint8_t dato, uint8_t x)
{
if (dato<10)
	{
	datosBuffer[x] = '0';
	datosBuffer[x+1] = dataBuff[0];
	}
else	{
	datosBuffer[x] = dataBuff[0];
	datosBuffer[x+1] = dataBuff[1];
	}	
}
//*********************************************************************************************************************
//*********************************************************************************************************************





//*********************************************************************************************************************
//
//*********************************************************************************************************************
void Mpu6050Bypass(void)
{
uint8_t transmitDataBuffer[2];	
    
i2cConfig(I2C0, 100000);	


// Register 106 – User Control USER_CTRL
// transmitDataBuffer[0] = 0x6a;
// transmitDataBuffer[1] = 0;
// i2cWrite( I2C0, 0x68, transmitDataBuffer, 2, TRUE );

	
// Register 55 – INT Pin / Bypass Enable Configuration INT_PIN_CFG	
transmitDataBuffer[0] = 0x37;
transmitDataBuffer[1] = 2;
i2cWrite(I2C0, 0x68, transmitDataBuffer, 2, TRUE);


// Register 107 – Power Management 1 PWR_MGMT_1
transmitDataBuffer[0] = 0x6b;
transmitDataBuffer[1] = 0;
i2cWrite(I2C0, 0x68, transmitDataBuffer, 2, TRUE);

}
//*********************************************************************************************************************
//*********************************************************************************************************************





//*********************************************************************************************************************
//		CARGA DATOS X/Y/Z/ANGULO EN BUFFER
//*********************************************************************************************************************
void CargaDatosBuffDisco(int16_t dato, uint8_t x)
{
// Recibe em "dato" el valor de X o Y o Z o ANGULO, en "x" la posicion que ocupa el dato a dentro del buffer que se almacena.
uint8_t buff[10] = {0x20};
uint8_t i = 0;

itoa((int) dato, buff, 10);

for (i=x; i<=(x+3); i++)
	datosBuffer[i] = 0x20;

for (i=0; i<=3; i++)
	{
	if (buff[i] != 0)
		datosBuffer[x+i] = buff[i];
	}
}
//*********************************************************************************************************************
//*********************************************************************************************************************





//*********************************************************************************************************************
//
//*********************************************************************************************************************
void CargaBrujuBuffDisco(int16_t angle)
{
uint8_t i = 0;


if (angle >= 360)
	angle %= 360;

// "Norte   "
if (angle >= 338 || angle < 23)
	{
	for (i=0; i<=7; i++)
	datosBuffer[POSICION_APUNTA+i] = b1[i];
	}
// "Noreste "
else if (angle < 68)
	{
	for (i=0; i<=7; i++)
	datosBuffer[POSICION_APUNTA+i] = b2[i];
	}
// "Este    "
else if (angle < 113)
	{
	for (i=0; i<=7; i++)
	datosBuffer[POSICION_APUNTA+i] = b3[i];
	}
// "Sudeste "
else if (angle < 158)
	{
	for (i=0; i<=7; i++)
	datosBuffer[POSICION_APUNTA+i] = b4[i];
	}
// "Sur     "
else if (angle < 203)
	{
	for (i=0; i<=7; i++)
	datosBuffer[POSICION_APUNTA+i] = b5[i];
	}
// "Suroeste"
else if (angle < 248)
	{
	for (i=0; i<=7; i++)
	datosBuffer[POSICION_APUNTA+i] = b6[i];
	}
// "Oeste   "
else if (angle < 293)
	{
	for (i=0; i<=7; i++)
	datosBuffer[POSICION_APUNTA+i] = b7[i];
	}
// "Noroeste"	
else	{
	for (i=0; i<=7; i++)
	datosBuffer[POSICION_APUNTA+i] = b8[i];
	}
}
//*********************************************************************************************************************
//*********************************************************************************************************************





//*********************************************************************************************************************
//
//*********************************************************************************************************************
int16_t vectorR2ToAngle(int16_t axis1, int16_t axis2)
{
float angle = 0;
float heading = 0;

// Angulo en radianes
heading = atan2((float)axis2, (float)axis1);

// Angulo en radianes ajustado
if (heading < 0.0)
	heading += 2.0 * 3.1415;

// Angulo en grados
angle = (heading * 180.0 / 3.1415); //M_PI

return (int16_t)angle;
}
//*********************************************************************************************************************
//*********************************************************************************************************************


/*==================[fin del archivo]========================================*/

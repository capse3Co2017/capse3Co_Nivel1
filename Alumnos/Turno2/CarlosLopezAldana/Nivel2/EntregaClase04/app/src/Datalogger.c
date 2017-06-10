/** @mainpage Datalogger
   
   Datalogger implementa la obtencion de los datos del HMC5883L que se encuentra en el modulo GY-87,

   estos datos permiten el calculo del angulo y la direccion a la cual apunta el modulo, tambien se
   
   activa el RTC del LPC4337 con el el cual el sistema obtiene la fecha y hora.
   
   Datos obtenidos: Fecha, Hora, Eje X, Eje Y, Eje Z, Angulo, Direccion.
   
   Todos estos datos son almacenados cada 2 segundos en la tarjeta de memoria SD/uSD.

   Archivo principal "Datalogger.c"

 */

/*============================================================================
 * Copyright 2017 Carlos Eduardo Lopez Aldana.
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

#include "Datalogger.h"     ///< Su propio archivo de cabecera.
#include "sapi.h"       ///< Biblioteca sAPI.
#include <math.h>       ///< Funciones matematicas.
#include "sapi_i2c.h"   ///< sAPI I2C header.
#include "ff.h"         ///< Biblioteca FAT FS.

/*==================[definiciones y macros]==================================*/

#define FILENAME "Datos.txt"    ///< Nombre del archivo en donde se guardaran los datos.
#define POSICION_DAY 0          ///< Posicion del dato dia dentro de "datosBuffer".
#define POSICION_MONTH 3        ///< Posicion del dato mes dentro de "datosBuffer".
#define POSICION_YEAR 6         ///< Posiciona del dato año dentro de "datosBuffer".
#define POSICION_HOUR 12        ///< Posicion del dato hora dentro de "datosBuffer".
#define POSICION_MIN 15         ///< Posicion del dato minutos dentro de "datosBuffer".
#define POSICION_SEC 18         ///< Posicion del dato segundos dentro de "datosBuffer".
#define POSICION_X 26           ///< Posicion del dato coordenada X dentro de "datosBuffer".
#define POSICION_Y 35           ///< Posicion del dato coordenada Y dentro de "datosBuffer".
#define POSICION_Z 44           ///< Posicion del dato coordenada Z dentro de "datosBuffer".
#define POSICION_ANGULO 58      ///< Posicion del dato angulo dentro de "datosBuffer".
#define POSICION_APUNTA 71      ///< Posicion del dato dentro de "datosBuffer" de hacia donde apunta el modulo.
#define TMUESTREO 2000          ///< Definicion del tiempo de muestreo.


/*==================[definiciones de datos internos]=========================*/

static FATFS fs;        ///< FatFs work area needed for each volume.
static FIL fp;          ///< File object needed for each open file.
static UINT bw;                ///< Pointer to the variable to return number of bytes written.

/* Buffer */
static uint8_t dataBuff[10];    ///< Buffer utilizado en la conversion de int a ascii y en la funcion que almacena
                                ///< los datos dentro de "datosBuffer".
                                
/**
 *  Buffer donde se cargan los datos obtenidos y calculados para luego este ser almacenado en la tarjeta SD/uSD.                                
 */
static uint8_t datosBuffer[] = "  /  /        :  :   - x=       y=       z=       Angulo=      Apunta=          \r\n";

static uint8_t strNorte[] = "Norte   ";         ///< String "Norte".
static uint8_t strNoreste[] = "Noreste ";       ///< String "Noreste".
static uint8_t strEste[] = "Este    ";          ///< String "Este".
static uint8_t strSudeste[] = "Sudeste ";       ///< String "Sudeste".
static uint8_t strSur[] = "Sur     ";           ///< String "Sur".
static uint8_t strSuroeste[] = "Suroeste";      ///< String "Suroeste".
static uint8_t strOeste[] = "Oeste   ";         ///< String "Oeste".
static uint8_t strNoroeste[] = "Noroeste";      ///< String "Noroeste".


static int16_t hmc5883l_x_raw;  ///< Variable que contiene el valor obtenido del eje X del HMC5883L.
static int16_t hmc5883l_y_raw;  ///< Variable que contiene el valor obtenido del eje Y del HMC5883L.
static int16_t hmc5883l_z_raw;  ///< Variable que contiene el valor obtenido del eje Z del HMC5883L.
static int16_t angle = 0;  ///< Variable que contiene el valor del angulo obtenido del calculo del X y Y.


delay_t delay1s;                ///< Estructura usada para el control del tiempo de muestreo. 




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
// FUNCION que se ejecuta cada vez que ocurre un Tick
bool_t diskTickHook(void *ptr);




/*==================[declaraciones de funciones externas]====================*/






/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main(void)
{
// ---------- CONFIGURACIONES ------------------------------
///< Inicializar y configurar la plataforma.
boardConfig();

/**
 *  Se hace un bypass del MPI6050, Acelerometro y Giroscopio
 *  acelerometro digital de 3 ejes, medicion hasta ±16g; y giroscopio digital de 3 ejes, medicion hasta ±2000 º/s.
 */
Mpu6050Bypass();   

///< Inicializar HMC5883L.
HMC5883L_config_t hmc5883L_configValue;
hmc5883lPrepareDefaultConfig(&hmc5883L_configValue);
hmc5883L_configValue.mode = HMC5883L_continuous_measurement;
hmc5883L_configValue.samples = HMC5883L_8_sample;
hmc5883lConfig(hmc5883L_configValue);


///< Inicializar UART_USB a 115200 baudios.
uartConfig(UART_USB, 115200);   

///< Estructura RTC-
rtc_t rtc;
rtc.year = 2017;
rtc.month = 6;
rtc.mday = 10;
rtc.wday = 0;
rtc.hour = 1;
rtc.min = 0;
rtc.sec= 0;
bool_t val = 0;

///< Inicializar RTC.
val = rtcConfig(&rtc);



///< SPI configuration.
spiConfig(SPI0);

/**
 *  Inicializar el conteo de Ticks con resolucion de 10ms, 
 *  con tickHook diskTickHook.
 */
tickConfig(10, diskTickHook);


///< Configura el delay no bloqueante, el cual se usa para determinar el tiempo de muestreo.
delayConfig(&delay1s, TMUESTREO);


// ------ PROGRAMA QUE ESCRIBE EN LA SD -------
///< Give a work area to the default drive.
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
        // para leer un nuevo valor del magnetometro
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
                f_write(&fp, datosBuffer, sizeof(datosBuffer)-1, &bw);
                f_close(&fp);
                if(bw == sizeof(datosBuffer)-1)
                        {
                        gpioWrite(LEDG, 1);
                        }
                }
        else        {
                gpioWrite(LEDR, 1);
                }

        // Si se Presiona TECLA 1 se procede a enviar el contenido del archivo "FILENAME".
        if (!gpioRead(TEC1))
                {
                uartWriteString(UART_USB, "Datos del archivo: ");   
                uartWriteString(UART_USB, FILENAME);   
                uartWriteString(UART_USB, "\r\n");    
                SendArchivoToUart();                                        // Envia archivo "FILENAME".
                }
        }
} 
// NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta 
// directamente sobre un microcontrolador y no es llamado/ por ningun
// Sistema Operativo, como en el caso de un programa para PC.
return 0;
}

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/





//*********************************************************************************************************************
//*********************************************************************************************************************
/**
 *  \brief Funcion que se ejecuta cada vez que ocurre un Tick.
 *  
 *  \param [in] ptr 
 *  \return 1
 *  
 */
bool_t diskTickHook(void *ptr)
{
disk_timerproc();   // Disk timer process
return 1;
}
//*********************************************************************************************************************
//*********************************************************************************************************************





//*********************************************************************************************************************
// C++ version 0.4 char* style "itoa":
// Written by Lukas Chmela
// Released under GPLv3.
//*********************************************************************************************************************
/**
 *  \brief Funcion que se usa para convertir un entero a un ASCII.
 *  
 *  \param [in] value Valor a convertir.
 *  \param [in] result Puntero donde se encuentra el resultado ASCII.
 *  \param [in] base La base de la conversion ej.10 es decimal.
 *  \return Un puntero, el cual apunta a los ASCII obtenidos.
 */
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
//*********************************************************************************************************************
/**
 *  \brief Envia el archivo con los datos por la UART_USB.
 *  
 *  \return Nada...
 *  
 *  \details La funcion es la encargada de enviar el archivo que contiene los datos obtenidos del HMC5883L y los
 *  procesados, el envio se realiza por la UART_USB, se envian paquetes de 100 bytes consecutivos hasta completar
 *  la totalidad de datos almacenados.
 *  \note Faltaria completar la salida/procesamiento de los errores de archivo.
 */
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
          else        {
                gpioWrite(LED1, 1);
                //return                        //Salida si dio error!!! Faltaria darle la salida correspondiente.
                }
          } while (br >= 100);
        
        f_close(&fp);
        }
else        {
        // Turn ON LEDR if the write operation was fail
        gpioWrite(LED2, 1);
        }
}
//*********************************************************************************************************************
//*********************************************************************************************************************





//*********************************************************************************************************************
//*********************************************************************************************************************
/**
 *  \brief Almacena datos de la fecha y hora en el buffer.
 *  
 *  \param [in] rtc Estructura tipo "rtc_t".
 *  \return Nada...
 *  
 *  \details La funcion es la encargada de almacenar en el buffer "datosBuffer[]" los datos de la fecha y hora,
 *  cada valor es almacenado en una posicion determinada dentro del buffer, estas posiciones estan en los define
 *  "POSICION_xxx".
 */
void ArmaDateAndTime(rtc_t * rtc)
{
// Dia
itoa((int) (rtc->mday), (char*)dataBuff, 10);
AlmacenaDatoRango((rtc->mday), POSICION_DAY);

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
//*********************************************************************************************************************
/**
 *  \brief Almacena un dato en una posicion determinada del buffer.
 *  
 *  \param [in] dato Dato a ser almacenado.
 *  \param [in] posicion La posicion donde sera almacenado el "dato".
 *  \return Nada...
 *  
 *  \details La funcion es usada por "ArmaDateAndTime()", recibe un dato y una posicion, con estos datos almacena
 *  los datos de la hora y fecha dentro de "datosBuffer[]".
 */
void AlmacenaDatoRango(uint8_t dato, uint8_t posicion)
{
if (dato<10)
        {
        datosBuffer[posicion] = '0';
        datosBuffer[posicion+1] = dataBuff[0];
        }
else        {
        datosBuffer[posicion] = dataBuff[0];
        datosBuffer[posicion+1] = dataBuff[1];
        }        
}
//*********************************************************************************************************************
//*********************************************************************************************************************





//*********************************************************************************************************************
//*********************************************************************************************************************
/**
 *  \brief Habilita el Bypass del MPU6050.
 *  
 *  \return Nada...
 *  
 *  \details Debido a que el modulo utilizado para adquirir los datos es el GY-87, en el cual para acceder al HMC5883L
 *  hay que pasar por el MPU6050, esto se logra mediante la habilitacion del Serial Interface Bypass Mux.
 */
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
//*********************************************************************************************************************
/**
 *  \brief Carga X, Y, Z y Angulo en el buffer.
 *  
 *  \param [in] dato Datos obtenidos del HMC5883L X, Y, Z o Angulo calculado.
 *  \param [in] posicion Es la posicion que ocupara "dato" en el buffer.
 *  \return Nada...
 *  
 *  \details La funcion recibira X, Y, Z o Angulo, junto con un dato de la posicion que el dato recibido ocupara en
 *  el buffer, este buffer sera el que se grabe en el archivo.
 */
void CargaDatosBuffDisco(int16_t dato, uint8_t posicion)
{
uint8_t buff[10] = {0x20};
uint8_t i = 0;

itoa((int) dato, buff, 10);

for (i=posicion; i<=(posicion+3); i++)
        datosBuffer[i] = 0x20;

for (i=0; i<=3; i++)
        {
        if (buff[i] != 0)
                datosBuffer[posicion+i] = buff[i];
        }
}
//*********************************************************************************************************************
//*********************************************************************************************************************





//*********************************************************************************************************************
//*********************************************************************************************************************
/**
 *  \brief Carga la direccion hacia donde apunta el modulo en el buffer.
 *  
 *  \param [in] angle Recibe el el angulo calculado por "vectorR2ToAngle()".
 *  \return Nada...
 *  
 *  \details La funcion recibira el angulo, y con el se procede a determinar la direccion (Norte, Este, etc), la direccion
 *  es almacenada en el buffer "datosBuffer[]", el cual es el que se grabara en la memoria SD/uSD.
 */
void CargaBrujuBuffDisco(int16_t angle)
{
uint8_t i = 0;


if (angle >= 360)
        angle %= 360;

// "Norte   "
if (angle >= 338 || angle < 23)
        {
        for (i=0; i<=7; i++)
        datosBuffer[POSICION_APUNTA+i] = strNorte[i];
        }
// "Noreste "
else if (angle < 68)
        {
        for (i=0; i<=7; i++)
        datosBuffer[POSICION_APUNTA+i] = strNoreste[i];
        }
// "Este    "
else if (angle < 113)
        {
        for (i=0; i<=7; i++)
        datosBuffer[POSICION_APUNTA+i] = strEste[i];
        }
// "Sudeste "
else if (angle < 158)
        {
        for (i=0; i<=7; i++)
        datosBuffer[POSICION_APUNTA+i] = strSudeste[i];
        }
// "Sur     "
else if (angle < 203)
        {
        for (i=0; i<=7; i++)
        datosBuffer[POSICION_APUNTA+i] = strSur[i];
        }
// "Suroeste"
else if (angle < 248)
        {
        for (i=0; i<=7; i++)
        datosBuffer[POSICION_APUNTA+i] = strSuroeste[i];
        }
// "Oeste   "
else if (angle < 293)
        {
        for (i=0; i<=7; i++)
        datosBuffer[POSICION_APUNTA+i] = strOeste[i];
        }
// "Noroeste"        
else        {
        for (i=0; i<=7; i++)
        datosBuffer[POSICION_APUNTA+i] = strNoroeste[i];
        }
}
//*********************************************************************************************************************
//*********************************************************************************************************************





//*********************************************************************************************************************
//*********************************************************************************************************************
/**
 *  \brief Calculo del angulo.
 *  
 *  \param [in] axis1 Este dato es el que corresponde a la coordenada X.
 *  \param [in] axis2 Este dato es el que corresponde a la coordenada Y.
 *  \return Retorna el angulo (int16_t).
 *  
 *  \details La funcion es la encargada de calcular el angulo en base a los datos del HMC5883L, ejes X e Y.
 */
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

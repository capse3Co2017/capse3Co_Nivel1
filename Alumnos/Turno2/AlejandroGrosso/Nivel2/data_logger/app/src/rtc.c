/*============================================================================
 * Licencia: 
 * Autor: 
 * Fecha: 
 *===========================================================================*/

/*==================[inlcusiones]============================================*/

#include "programa.h"   // <= su propio archivo de cabecera
#include "sapi.h"       // <= Biblioteca sAPI
#include <stdlib.h>

/*==================[definiciones y macros]==================================*/

/*==================[definiciones de datos internos]=========================*/
static rtc_t rtc;
/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/
// Coloca la fecha y hora leida del RTC en el string buffer
void getDateHour(char *buffer)
{
   rtcRead(&rtc);
   itoa(rtc.year,buffer,10);
   buffer[4]=':';
   if(rtc.month<10)
   {
     buffer[5]='0';
     itoa(rtc.month,&buffer[6],10);
   }
   else
     itoa(rtc.month,&buffer[5],10);
   buffer[7]=':';
   if(rtc.mday<10)
   {
     buffer[8]='0';
     itoa(rtc.mday,&buffer[9],10);
   }
   else
     itoa(rtc.mday,&buffer[8],10);
   buffer[10]=':';
   if(rtc.hour<10)
   {
     buffer[11]='0';
     itoa(rtc.hour,&buffer[12],10);
   }
   else
     itoa(rtc.hour,&buffer[11],10);
   buffer[13]=':';
   if(rtc.min<10)
   {
     buffer[14]='0';
     itoa(rtc.min,&buffer[15],10);
   }
   else
     itoa(rtc.min,&buffer[14],10);
   buffer[16]=':';
   if(rtc.sec<10)
   {
     buffer[17]='0';
     itoa(rtc.sec,&buffer[18],10);
   }
   else
     itoa(rtc.sec,&buffer[17],10);
   buffer[19]=0;
}
/*==================[declaraciones de funciones externas]====================*/

/*==================[funcion principal]======================================*/

/*==================[definiciones de funciones internas]=====================*/
void rtcInit(void)
{
   rtc.year=2017;
   rtc.month=5;
   rtc.mday=29;
   rtc.wday=1;
   rtc.hour=9;
   rtc.min=58;
   rtc.sec=0;
   rtcConfig(&rtc);   

}
/*==================[definiciones de funciones externas]=====================*/

/*==================[fin del archivo]========================================*/

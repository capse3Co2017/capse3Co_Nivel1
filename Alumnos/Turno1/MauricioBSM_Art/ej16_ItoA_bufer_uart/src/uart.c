/* Copyright 2016, Eric Pernia.
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
 * Date: 2016-04-26
 */

/*==================[inclusions]=============================================*/

#include "uart.h"         /* <= own header */

#include "sapi.h"         /* <= sAPI header */

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/


/**
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
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


/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void){

   /* ------------- INICIALIZACIONES ------------- */

   /* Inicializar la placa */
   boardConfig();

   /* Inicializar el conteo de Ticks con resolución de 1ms, sin tickHook */
   tickConfig( 1, 0 );

   /* Inicializar GPIOs */
   gpioConfig( 0, GPIO_ENABLE );

   /* Configuración de pines de entrada para Teclas de la CIAA-NXP */
   gpioConfig( TEC1, GPIO_INPUT );
   gpioConfig( TEC2, GPIO_INPUT );
   gpioConfig( TEC3, GPIO_INPUT );
   gpioConfig( TEC4, GPIO_INPUT );

   /* Configuración de pines de salida para Leds de la CIAA-NXP */
   gpioConfig( LEDR, GPIO_OUTPUT );
   gpioConfig( LEDG, GPIO_OUTPUT );
   gpioConfig( LEDB, GPIO_OUTPUT );
   gpioConfig( LED1, GPIO_OUTPUT );
   gpioConfig( LED2, GPIO_OUTPUT );
   gpioConfig( LED3, GPIO_OUTPUT );

   /* Inicializar UART_USB a 115200 baudios */
   uartConfig( UART_USB, 115200 );


   char Buffer[] = "Datos X=    ,Y=    ,Z=    \r\n";
   char itoBuff[10];
   int16_t hmc5883l_x_raw;
   int16_t hmc5883l_y_raw;
   int16_t hmc5883l_z_raw;
   uint8_t i;
   int16_t incr=0;
   delay_t ciclo;
   delayConfig(&ciclo,3000);

   /* ------------- REPETIR POR SIEMPRE ------------- */
   while(1) {

	   hmc5883l_x_raw=incr;
	   hmc5883l_y_raw=incr+13;
	   hmc5883l_z_raw=incr+200;
	   incr=incr+1;
	   if (incr>9999) incr=0;

      if(  delayRead(&ciclo) ){
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


    	     uartWriteString( UART_USB,Buffer );
      }

   }

   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
   return 0 ;
}

/*==================[end of file]============================================*/

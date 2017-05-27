/* Copyright 2016, Eric Pernia.
 * All rights reserved.
 *
 * This file is part of CIAA Firmware.
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

/** \brief Main example source file
 **
 ** This is a mini example of the CIAA Firmware.
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Examples CIAA Firmware Examples
 ** @{ */
/** \addtogroup Main example source file
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * ENP          Eric Pernia
 *
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 2016-04-26   v0.0.1   First version
 */

/*==================[inclusions]=============================================*/

#include "program.h"         /* <= own header */

#include "sapi.h"         /* <= sAPI header */

/*==================[macros and definitions]=================================*/

typedef enum{ UP, FALLING, DOWN, RISING } estadoMef;

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

estadoMef estadoActual;
   
uint8_t tiempoTranscurrido = 0;
uint8_t led = 0;

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

void buttonPressed(void){
	gpioWrite(LED1, ON);
}

void buttonReleased(void){
	gpioWrite(LED1, OFF);
}

void InicializarMEF( void ) {
   estadoActual = UP;
}

void ActualizarMEF( void ) {
   switch(estadoActual){
      
      case UP:
         // En este caso no hay salida/evento
         // Chequear condiciones para cambiar de estado
         if( !gpioRead(TEC2) ){
            estadoActual = FALLING;
            tiempoTranscurrido = 0;
         }
      break;
      
      case FALLING:
         // Chequear condiciones para cambiar de estado
         if(tiempoTranscurrido == 40){ 
             if(!gpioRead(TEC2)){
        	       estadoActual = DOWN;
        	       // Salida/Evento
                 buttonPressed();
                tiempoTranscurrido= 0;
              } 
              else{
        	 estadoActual = UP;
              }
         }
      break;
      
      case DOWN:
    	  // En este caso no hay salida/evento
         // Chequear condiciones para cambiar de estado
         if( gpioRead(TEC2)  ){
            estadoActual = RISING;
            tiempoTranscurrido= 0;
         }
      break;
      
      case RISING:
         if(tiempoTranscurrido == 40){ 
             if(gpioRead(TEC2)){
        	       estadoActual = UP;
        	       // Salida/Evento
                 buttonReleased();
                 tiempoTranscurrido= 0;
              } 
              else{
        	       estadoActual = DOWN;
              }
         }
         break;
       
      default:
         InicializarMEF();
      break;
   } 
}

int main(void)
{
   boardConfig();

   InicializarMEF();
   
   while(1) {
      ActualizarMEF();
      delay(1);
      tiempoTranscurrido++;
    }

    return 0 ;
}
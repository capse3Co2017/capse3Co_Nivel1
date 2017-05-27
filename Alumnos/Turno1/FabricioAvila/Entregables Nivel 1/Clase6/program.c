/* Copyright (c) 2017, Eric Pernia.
 * All rights reserved.
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
 * ============================================================================
 * Date: 2017-03-15
 * ============================================================================
 */
 


 // Programa que muestra el estado de las teclas/leds y controla los leds de
 // la EDUCIAA a través de la UART
 // Ejercicio entragable - Clase 6
 // Fabricio Ávila
 // Marzo - 2017
 


/*==================[inclusions]=============================================*/

#include "program.h"   // <= Own header
#include "sapi.h"      // <= sAPI library

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[main function]==========================================*/

// MAIN FUNCTION, ENTRY PINT TO THE PROGRAM AFTER POR OR RESET.
int main( void ){

   // ---------- SETUP ----------------------------------------
   // Initialize an configurate the board
    boardConfig();   
    uartConfig(UART_USB, 115200);
    uint8_t dato = 5;
    bool_t nuevoDato;
    bool_t estado_LB = FALSE;
    bool_t estado_LG = FALSE;
    bool_t estado_LR = FALSE;
    bool_t estado_L1 = FALSE;
    bool_t estado_L2 = FALSE;
    bool_t estado_L3 = FALSE;
   uint8_t Texto1[120];
   
   // ---------- REPEAT FOR EVER ------------------------------
   while( TRUE )
   {
          delay(1000);
          
   	nuevoDato=uartReadByte(UART_USB,&dato);
         
         if(nuevoDato){
             switch(dato){
                 case 'a':   
                             estado_LB = !estado_LB;
                             gpioWrite( LEDB, estado_LB );
                             break;
              
                 case 'v':   
                             estado_LG = !estado_LG;
                             gpioWrite( LEDG, estado_LG );
                             break;
              
                 case 'r':   
                             estado_LR = !estado_LR;
                             gpioWrite( LEDR, estado_LR );
                             break;
                 
                 case '1':   
                             estado_L1 = !estado_L1;
                             gpioWrite( LED1, estado_L1 );
                             break;
              
                 case '2':   
                             estado_L2 = !estado_L2;
                             gpioWrite( LED2, estado_L2 );
                             break;
                             
                 case '3':   
                             estado_L3 = !estado_L3;
                             gpioWrite( LED3, estado_L3 );
                             break;
              
               
                 default:   ;
             } 
         }
    sprintf(Texto1, "Estado de LEDB=%d,LEDG=%d,LEDR=%d,LED1=%d,LED2=%d,LED3=%d,TEC1=%d,TEC2=%d,TEC3=%d,TEC4=%d\r\n",
    estado_LB,estado_LG,estado_LR,estado_L1,estado_L2,estado_L3,!(gpioRead(TEC1)),!(gpioRead(TEC2)),!(gpioRead(TEC3)),
    !(gpioRead(TEC4)));

    uartWriteString(UART_USB, Texto1);

      } 

   // IT SHOULD NEVER COME HERE, because this program runs directly on a
   // microcontroller and is not called by any Operating System, as in
   // case of a PC program.
   return 0;
}

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/*==================[end of file]============================================*/

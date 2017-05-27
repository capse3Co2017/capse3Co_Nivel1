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
   uartConfig( UART_USB, 115200 );
   
   uint8_t tecla, i;
   uint8_t controlTEC[4], controlLed[6]={0};
   uint8_t led[6]={LEDB, LEDG, LEDR, LED1, LED2, LED3};
   uint8_t ledString[6]={'B','G','R','1','2','3'};
   uint8_t tec[4]={TEC1, TEC2, TEC3, TEC4};
   bool_t datoNuevo;
   
   // ---------- REPEAT FOR EVER ------------------------------
   while( TRUE )
   {    
        //Para ver el estado de las teclas
        for(i=0;i<4;i++){
          if(i<4){
            if(!(gpioRead(tec[i]))){
               controlTEC[i]=1;
            }else{
               controlTEC[i] = 0;
            }
          }
        }
        //Recibo el dato y veo de que letra se trata, y en funcion a eso el led que se prende.
        datoNuevo = uartReadByte( UART_USB, &tecla );
        
        if(datoNuevo){
          switch(tecla){
              case 'a': 
                        if(controlLed[0])
                            controlLed[0]=0;
                        else
                            controlLed[0]=1;
                        break;
              case 'v': 
                        if(controlLed[1])
                            controlLed[1]=0;
                        else
                            controlLed[1]=1;
                        break;
              case 'r': 
                        if(controlLed[2])
                            controlLed[2]=0;
                        else
                            controlLed[2]=1;
                        break;
              case '1': 
                        if(controlLed[3])
                            controlLed[3]=0;
                        else
                            controlLed[3]=1;
                        break;
              case '2': 
                        if(controlLed[4])
                            controlLed[4]=0;
                        else
                            controlLed[4]=1;
                        break;
              case '3': 
                        if(controlLed[5])
                            controlLed[5]=0;
                        else
                            controlLed[5]=1;
                        break;
          }
          
          for(i=0;i<6;i++){
            if(controlLed[i]){
               gpioWrite( led[i], ON );
            }else{
               gpioWrite( led[i], OFF );
            }
          } 
                 
        }
        
        //Escribo por consola el estado de las teclas y los leds
        for(i=0;i<6;i++){
           uartWriteString( UART_USB, "\n\rLED"); 
           uartWriteByte( UART_USB, ledString[i]);
           uartWriteString( UART_USB,": " );       
           if(controlLed[i])
               uartWriteString( UART_USB, "ON     " );
           else
               uartWriteString( UART_USB, "OFF    " );
           if(i<4){
               uartWriteString( UART_USB, "      TEC");
               uartWriteByte(UART_USB, i+49);            //Para imprimir los caracteres 1 a 4
               uartWriteString(UART_USB,": " );
               if(controlTEC[i])
                   uartWriteString( UART_USB, "ON     " );
               else
                   uartWriteString( UART_USB, "OFF    " );
           }

        }   
        uartWriteString( UART_USB, "\n\r");     
       
        delay ( 1000 );  
   } 

      // IT SHOULD NEVER COME HERE, because this program runs directly on a
   // microcontroller and is not called by any Operating System, as in
   // case of a PC program.
   return 0;
}
 
/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/*==================[end of file]============================================*/

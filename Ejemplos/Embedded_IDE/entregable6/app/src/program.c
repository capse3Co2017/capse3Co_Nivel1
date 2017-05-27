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

void delayInaccurate( tick_t delay_ms );

/*==================[external data definition]===============================*/
#define INACCURATE_TO_MS 20400

/*==================[main function]==========================================*/

// MAIN FUNCTION, ENTRY PINT TO THE PROGRAM AFTER POR OR RESET.


int main( void ){

   // ---------- SETUP ----------------------------------------
   // Initialize an configurate the board
   boardConfig();   
   uartConfig(UART_USB, 115200);
   
   uint8_t dato = 5;
   bool_t nuevoDato;
   
// ---------- ETIQUETAS BOOLEANAS PARA REALIZAR LECTURAS-------------------------
   bool_t S_LEDB;
   bool_t S_LEDG;
   bool_t S_LEDR;
   bool_t S_LED1;
   bool_t S_LED2;
   bool_t S_LED3;
   
   bool_t S_TEC1;
   bool_t S_TEC2;
   bool_t S_TEC3;
   bool_t S_TEC4;

// ---------- ETIQUETAS DE SALIDA-------------------------
   
   uint8_t miTexto[]="Arranca Programa \r\n";
   uint8_t miTexto2[]=" \r\n";
 
   uint8_t ledsOFF[]="Nada en ON \r\n";
   uint8_t ledsON[]=" Todos los Leds ON \r\n";
   
   uint8_t ledB_ON[]="Led Azul encendido \r\n";
   uint8_t ledB_OFF[]="Led Azul apagado \r\n";
   
   uint8_t ledG_ON[]="Led Verde encendido \r\n";
   uint8_t ledG_OFF[]="Led Verde apagado \r\n";

   uint8_t ledR_ON[]="Led Rojo encendido \r\n";
   uint8_t ledR_OFF[]="Led Rojo apagado \r\n";   

   uint8_t led1_ON[]="Led 1 encendido \r\n";
   uint8_t led1_OFF[]="Led 1 apagado \r\n";

   uint8_t led2_ON[]="Led 2 encendido \r\n";
   uint8_t led2_OFF[]="Led 2 apagado \r\n";

   uint8_t led3_ON[]="Led 3 encendido \r\n";
   uint8_t led3_OFF[]="Led 3 apagado \r\n";

   uint8_t TEC1_ON[]="TEC1 ON \r\n";
   uint8_t TEC1_OFF[]="TEC1 OFF \r\n";

   uint8_t TEC2_ON[]="TEC2 ON \r\n";
   uint8_t TEC2_OFF[]="TEC2 OFF \r\n";

   uint8_t TEC3_ON[]="TEC3 ON \r\n";
   uint8_t TEC3_OFF[]="TEC3 OFF \r\n";
   
   uint8_t TEC4_ON[]="TEC4 ON \r\n";
   uint8_t TEC4_OFF[]="TEC4 OFF \r\n";
   
   uint8_t control;

// ---------- BANDERAS DE ESTADOS-------------------------
   uint8_t Flag_LED_B = 0;
   uint8_t Flag_LED_G = 0;
   uint8_t Flag_LED_R = 0;
   uint8_t Flag_LED_1 = 0;
   uint8_t Flag_LED_2 = 0;
   uint8_t Flag_LED_3 = 0;
   uint8_t Flag_TEC_1 = 0;   
   uint8_t Flag_TEC_2 = 0;
   uint8_t Flag_TEC_3 = 0;
   uint8_t Flag_TEC_4 = 0;

   //uartWriteByte(UART_USB, 'a');
   //uartWriteByte(UART_USB, dato+48);
   
   uartWriteString(UART_USB, miTexto);
   uartWriteString(UART_USB, miTexto2);
   
// ---------- REPEAT FOR EVER ------------------------------
   while( TRUE )
   {
    delayInaccurate(1000);
// ---------- LECTURA DE PINES-------------------------
    S_LEDB = gpioRead( LEDB );
    S_LEDG = gpioRead( LEDG );
    S_LEDR = gpioRead( LEDR );
    S_LED1 = gpioRead( LED1 );
    S_LED2 = gpioRead( LED2 );
    S_LED3 = gpioRead( LED3 );
    
    S_TEC1 = gpioRead( TEC1 );
    S_TEC2 = gpioRead( TEC2 );
    S_TEC3 = gpioRead( TEC3 );
    S_TEC4 = gpioRead( TEC4 );
   
    nuevoDato=uartReadByte(UART_USB,&dato);


// ---------- LECTURA DE ESTADO LED B-------------------------
    if((Flag_LED_B == 0))
   	{
     	       uartWriteString(UART_USB, ledB_OFF);
            }else{
            	uartWriteString(UART_USB, ledB_ON);
       	       	}
// ---------- LECTURA DE ESTADO LED G-------------------------       	       	
    if((Flag_LED_G == 0))
   	{
     	       uartWriteString(UART_USB, ledG_OFF);
            }else{
            	uartWriteString(UART_USB, ledG_ON);
       	       	}
// ---------- LECTURA DE ESTADO LED R-------------------------       	       	
    if((Flag_LED_R == 0))
   	{
     	       uartWriteString(UART_USB, ledR_OFF);
            }else{
            	uartWriteString(UART_USB, ledR_ON);
       	       	}
// ---------- LECTURA DE ESTADO LED 1-------------------------       	       	
    if((Flag_LED_1 == 0))
   	{
     	       uartWriteString(UART_USB, led1_OFF);
            }else{
            	uartWriteString(UART_USB, led1_ON);
       	       	}
// ---------- LECTURA DE ESTADO LED 2-------------------------       	       	
    if((Flag_LED_2 == 0))
   	{
     	       uartWriteString(UART_USB, led2_OFF);
            }else{
            	uartWriteString(UART_USB, led2_ON);
       	       	}
// ---------- LECTURA DE ESTADO LED 3-------------------------       	       	
    if((Flag_LED_3 == 0))
   	{
     	       uartWriteString(UART_USB, led3_OFF);
            }else{
            	uartWriteString(UART_USB, led3_ON);
       	       	}
// ---------- LECTURA DE ESTADO TECLA 1-------------------------       	       	
    if(!S_TEC1)
   	{
     	       uartWriteString(UART_USB, TEC1_ON);
            }else{
            	uartWriteString(UART_USB, TEC1_OFF);
       	       	}
// ---------- LECTURA DE ESTADO TECLA 2-------------------------       	       	
    if(!S_TEC2)
   	{
     	       uartWriteString(UART_USB, TEC2_ON);
            }else{
            	uartWriteString(UART_USB, TEC2_OFF);
       	       	}
// ---------- LECTURA DE ESTADO TECLA 3-------------------------       	       	
    if(!S_TEC3)
   	{
     	       uartWriteString(UART_USB, TEC3_ON);
            }else{
            	uartWriteString(UART_USB, TEC3_OFF);
       	       	}
// ---------- LECTURA DE ESTADO TECLA 4-------------------------       	       	
    if(!S_TEC4)
   	{
     	       uartWriteString(UART_USB, TEC4_ON);
            }else{
            	uartWriteString(UART_USB, TEC4_OFF);
       	       	}

// ---------- LOGICA DE CONTROL DE ENCENDIDO Y APAGADO DE LEDS-------------------------
    
    if (nuevoDato){
    	control = dato;
    	switch(control) {
    	   // ---------- LOGICA DE CONTROL DE ENCENDIDO Y APAGADO DE LED AZUL-------------------------
    	   case 'a':
    		if((dato == 'a'))
     			{
     				if((!S_LEDB)&&(Flag_LED_B == 0))
				{	
					uartWriteByte(UART_USB, dato);
   					gpioWrite(LEDB, ON);
   					uartWriteString(UART_USB, ledB_ON);
   					Flag_LED_B = 1;
   				}else{
     					if((!S_LEDB)&&(Flag_LED_B == 1))
   					{
						uartWriteByte(UART_USB, dato);
   						gpioWrite(LEDB, OFF);
   						uartWriteString(UART_USB, ledB_OFF);
   						Flag_LED_B = 0;
   					}
   				     }
      	           		}
      	           break;
	   // ---------- LOGICA DE CONTROL DE ENCENDIDO Y APAGADO DE LED VERDE-------------------------
    	   case 'v':
    		if((dato == 'v'))
     			{
     				if((!S_LEDG)&&(Flag_LED_G == 0))
				{	
					uartWriteByte(UART_USB, dato);
   					gpioWrite(LEDG, ON);
   					uartWriteString(UART_USB, ledG_ON);
   					Flag_LED_G = 1;
   				}else{
     					if((!S_LEDG)&&(Flag_LED_G == 1))
   					{
						uartWriteByte(UART_USB, dato);
   						gpioWrite(LEDG, OFF);
   						uartWriteString(UART_USB, ledG_OFF);
   						Flag_LED_G = 0;
   					}
   				     }
      	           		}
      	           break;
	   // ---------- LOGICA DE CONTROL DE ENCENDIDO Y APAGADO DE LED ROJO-------------------------      	            
	   case 'r':
    		if((dato == 'r'))
     			{
     				if((!S_LEDR)&&(Flag_LED_R == 0))
				{	
					uartWriteByte(UART_USB, dato);
   					gpioWrite(LEDR, ON);
   					uartWriteString(UART_USB, ledR_ON);
   					Flag_LED_R = 1;
   				}else{
     					if((!S_LEDR)&&(Flag_LED_R == 1))
   					{
						uartWriteByte(UART_USB, dato);
   						gpioWrite(LEDR, OFF);
   						uartWriteString(UART_USB, ledR_OFF);
   						Flag_LED_R = 0;
   					}
   				     }
      	           		}
      	           break;	
	   // ---------- LOGICA DE CONTROL DE ENCENDIDO Y APAGADO DE LED 1-------------------------
	   case '1':
    		if((dato == '1'))
     			{
     				if((!S_LED1)&&(Flag_LED_1 == 0))
				{	
					uartWriteByte(UART_USB, dato);
   					gpioWrite(LED1, ON);
   					uartWriteString(UART_USB, led1_ON);
   					Flag_LED_1 = 1;
   				}else{
     					if((!S_LED1)&&(Flag_LED_1 == 1))
   					{
						uartWriteByte(UART_USB, dato);
   						gpioWrite(LED1, OFF);
   						uartWriteString(UART_USB, led1_OFF);
   						Flag_LED_1 = 0;
   					}
   				     }
      	           		}
      	           break;
	   // ---------- LOGICA DE CONTROL DE ENCENDIDO Y APAGADO DE LED 2-------------------------      	           
	   case '2':
    		if((dato == '2'))
     			{
     				if((!S_LED2)&&(Flag_LED_2 == 0))
				{	
					uartWriteByte(UART_USB, dato);
   					gpioWrite(LED2, ON);
   					uartWriteString(UART_USB, led2_ON);
   					Flag_LED_2 = 1;
   				}else{
     					if((!S_LED2)&&(Flag_LED_2 == 1))
   					{
						uartWriteByte(UART_USB, dato);
   						gpioWrite(LED2, OFF);
   						uartWriteString(UART_USB, led2_OFF);
   						Flag_LED_2 = 0;
   					}
   				     }
      	           		}
      	           break;
	   // ---------- LOGICA DE CONTROL DE ENCENDIDO Y APAGADO DE LED 3-------------------------
	   case '3':
    		if((dato == '3'))
     			{
     				if((!S_LED3)&&(Flag_LED_3 == 0))
				{	
					uartWriteByte(UART_USB, dato);
   					gpioWrite(LED3, ON);
   					uartWriteString(UART_USB, led3_ON);
   					Flag_LED_3 = 1;
   				}else{
     					if((!S_LED3)&&(Flag_LED_3 == 1))
   					{
						uartWriteByte(UART_USB, dato);
   						gpioWrite(LED3, OFF);
   						uartWriteString(UART_USB, led3_OFF);
   						Flag_LED_3 = 0;
   					}
   				     }
      	           		}
      	           break;
      	       
      	     //default:	
      	             	       	
      		      }//end switch
                   }
    
      }
      
    // IT SHOULD NEVER COME HERE, because this program runs directly on a
   // microcontroller and is not called by any Operating System, as in
   // case of a PC program.
   
   return 0;
}


/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/*==================[end of file]============================================*/
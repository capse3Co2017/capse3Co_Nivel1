/* Copyright 2016, Ian Olivieri.
 * Copyright 2016, Eric Pernia.
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

/* Diagrama de conexion ESP8266

   VCC ESP8266 <--> +3.3V EDU-CIAA-NXP
   RST ESP8266 <--> (SIN CONEXION)
 CH_PD ESP8266 <--> +3.3V EDU-CIAA-NXP
    TX ESP8266 <--> RX EDU-CIAA-NXP

    RX ESP8266 <--> TX EDU-CIAA-NXP
 GPIO0 ESP8266 <--> (SIN CONEXION)
 GPIO0 ESP8266 <--> (SIN CONEXION)
   GND ESP8266 <--> GND EDU-CIAA-NXP

  AT commands: http://www.pridopia.co.uk/pi-doc/ESP8266ATCommandsSet.pdf
*/

/*==================[inclusions]=============================================*/

#include "main.h"   /* <= own header */

#include "sapi.h"                 /* <= sAPI header */

/*==================[macros and definitions]=================================*/

#define BAUD_RATE 115200 // Baudrate por defecto del ESP8266

/*==================[internal data declaration]==============================*/
   waitForReceiveStringOrTimeout_t waitText;
   waitForReceiveStringOrTimeoutState_t waitTextState;
/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
void reciboESP (void);
void imprimirMensajeDeBienvenida( void ){

   /* Imprimo el mensaje de bienvenida */
   uartWriteString( UART_USB, 
      "Bievenido al asistente de configuracion del modulo ESP8266\r\n" );
   uartWriteString( UART_USB, 
      "Antes de continuar, por favor asegurese que su terminal\r\n" );
   uartWriteString( UART_USB, 
      "serie dispone del terminador CR+LF (enter)\r\n\r\n" );
   uartWriteString( UART_USB, 
      "A continuacion se realiza un listado de algunos de los\r\n" );
   uartWriteString( UART_USB, "comandos AT disponibles:\r\n\r\n" );
   uartWriteString( UART_USB, 
      "> Saber si el modulo responde correctamente: AT\r\n" );
   uartWriteString( UART_USB, 
      "> Version del Firmware: AT+GMR\r\n" );
   uartWriteString( UART_USB, "> Resetear el modulo: AT+RST\r\n" );
   uartWriteString( UART_USB, 
      "> Listar todas las redes disponibles: AT+CWLAP\r\n" );
   uartWriteString( UART_USB, 
      "> Checkear la red actual: AT+CWJAP?\r\n" );
   uartWriteString( UART_USB, 
      "> Unirse a una red: AT+CWJAP=\"nombreRedInalambrica\",\"password\"\r\n" );
   uartWriteString( UART_USB, 
      "  - NOTA: Las comillas dobles son parte del mensaje\r\n" );
   uartWriteString( UART_USB, 
      "> Salir de la red: AT+CWQAP\r\n" );

   delay(100);
}


/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void){

   /* ------------- INICIALIZACIONES ------------- */

   /* Inicializar la placa */
   boardConfig();

   /* Inicializar las UART a 115200 baudios */
   uartConfig( UART_USB, BAUD_RATE );
   uartConfig( UART_232, BAUD_RATE );

   bool_t received = FALSE;

   /* Si presionan TEC1 muestro el mensaje de bienvenida */
   imprimirMensajeDeBienvenida();


   /* ------------- REPETIR POR SIEMPRE ------------- */
   while(1) {
     //INICIA Comando AT
	 uartWriteString( UART_232,"AT\r\n");
	 uartWriteString( UART_USB,"AT\r\n");
     waitText.string =  "OK\r\n";
     reciboESP();
     //FIN Comando AT

     //INICIA Comando Verificar conexion
	 uartWriteString( UART_232,"AT+CWJAP?\r\n");
	 uartWriteString( UART_USB,"AT+CWJAP?\r\n");
     waitText.string =  "+CWJAP:\"TP-LINK_Besama\"";
     reciboESP();
     //FIN Comando Verificar conexion

     //INCIA Comando conexion a Thinkspeak
	 uartWriteString( UART_232,"AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
	 uartWriteString( UART_USB,"AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
     waitText.string =  "CONNECT";
     reciboESP();
	 //FIN Comando conexion a Thinkspeak

     //INICIA Envio cantidad de caracteres de Comando a Thinkspeak
	 uartWriteString( UART_232,"AT+CIPSEND=150\r\n");
	 uartWriteString( UART_USB,"AT+CIPSEND=150\r\n");
     waitText.string =  "OK\r\n>";
     reciboESP();
     //FIN Envio cantidad de caracteres de Comando a Thinkspeak

     //INICIA Envio Comando Carga datos a Thinkspeak
	 uartWriteString( UART_232,"GET /update?key=3EV61E0RD5KV780X&field1=100&field2=200&field3=300&headers=false HTTP/1.1\r\nHost: api.thingspeak.com\r\nConnection: close\r\nAccept: \*/\*\r\n\r\n");
	 uartWriteString( UART_USB,"GET /update?key=3EV61E0RD5KV780X&field1=100&field2=200&field3=300&headers=false HTTP/1.1\r\nHost: api.thingspeak.com\r\nConnection: close\r\nAccept: \*/\*\r\n\r\n");
	 waitText.string =  "+IPD,472:HTTP/1.0 200 OK\r\n";
     reciboESP();
     //FIN Envio Comando Carga datos a Thinkspeak
     delay(20000);
     //Re- INICIA LOOP
  }

   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
   return 0 ;
}
void reciboESP(){
	waitTextState = UART_RECEIVE_STRING_CONFIG;
	waitText.stringSize = sizeof(waitText.string);
	waitText.timeout = 5000;
	while( waitTextState != UART_RECEIVE_STRING_RECEIVED_OK && waitTextState != UART_RECEIVE_STRING_TIMEOUT ){
	      waitTextState = waitForReceiveStringOrTimeout( UART_232, &waitText );
	   }
	if( waitTextState == UART_RECEIVE_STRING_RECEIVED_OK ){
	   uartWriteString( UART_USB, waitText.string );
	   uartWriteString( UART_USB, "--->OK\r\n" );
	}
	if( waitTextState == UART_RECEIVE_STRING_TIMEOUT ){
	   uartWriteString( UART_USB, "\r\nSalio por timeout\r\n" );
	}
}


/*==================[end of file]============================================*/

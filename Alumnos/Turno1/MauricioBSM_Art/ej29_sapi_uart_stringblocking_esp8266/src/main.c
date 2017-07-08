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

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

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
     //Comando AT
	 uartWriteString( UART_232,
		  "AT\r\n");

     received = waitForReceiveStringOrTimeoutBlocking(
                   UART_232,
                   "OK\r\n",
                   sizeof("OK\r\n"),
                   5000
                );

     if( received ){
        uartWriteString( UART_USB, "\r\nRecibí OK del ESP\r\n" );
     }
     // Si no lo recibe indica que salio de la funcion
     // waitForReceiveStringOrTimeoutBlocking  por timeout.
     else{
        uartWriteString( UART_USB, "\r\nError de OK\r\n" );
     }
     //FIN Comando AT
     delay(3000);
     //INICIA Comando Verificar conexion
	 uartWriteString( UART_232,
		  "AT+CWJAP?\r\n");

     received = waitForReceiveStringOrTimeoutBlocking(
                   UART_232,
                   "+CWJAP:\"TP-LINK_Besama\"",
                   sizeof("+CWJAP:\"TP-LINK_Besama\""),
                   5000
                );

     if( received ){
        uartWriteString( UART_USB, "\r\nEstá conectado a la red que corresponde\r\n" );
     }
     // Si no lo recibe indica que salio de la funcion
     // waitForReceiveStringOrTimeoutBlocking  por timeout.
     else{
        uartWriteString( UART_USB, "\r\nError de conexión de red\r\n" );
     }
     //FIN Comando Verificar conexion
     delay(3000);
     //INCIA Comando conexion a Thinkspeak
	 uartWriteString( UART_232,
		  "AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");

     received = waitForReceiveStringOrTimeoutBlocking(
                   UART_232,
                   "CONNECT",
                   sizeof("CONNECT"),
                   5000
                );

     if( received ){
        uartWriteString( UART_USB, "\r\nEstá conectado Thinkspeak\r\n" );
     }
     // Si no lo recibe indica que salio de la funcion
     // waitForReceiveStringOrTimeoutBlocking  por timeout.
     else{
        uartWriteString( UART_USB, "\r\nError de conexión a Thinkspeak\r\n" );
     }
     //FIN Comando conexion a Thinkspeak
     delay(3000);
     //INICIA Envio cantidad de caracteres de Comando a Thinkspeak
	 uartWriteString( UART_232,
		  "AT+CIPSEND=150\r\n");

     received = waitForReceiveStringOrTimeoutBlocking(
                   UART_232,
                   "OK\r\n>",
                   sizeof("OK\r\n>"),
                   5000
                );

     if( received ){
        uartWriteString( UART_USB, "\r\nCargo OK nro de bytes a enviar\r\n" );
     }
     // Si no lo recibe indica que salio de la funcion
     // waitForReceiveStringOrTimeoutBlocking  por timeout.
     else{
        uartWriteString( UART_USB, "\r\nError Carga nro de Bytes\r\n" );
     }
     //FIN Envio cantidad de caracteres de Comando a Thinkspeak
     delay(3000);
     //INICIA Envio Comando Carga datos a Thinkspeak
	 uartWriteString( UART_232,
			 "GET /update?key=3EV61E0RD5KV780X&field1=100&field2=200&field3=300&headers=false HTTP/1.1\r\nHost: api.thingspeak.com\r\nConnection: close\r\nAccept: \*/\*\r\n\r\n");
            //GET /update?key={my_____API_key}&field1=56&headers=false HTTP/1.1{CL}Host: api.thingspeak.com{CL}Connection: close{CL}Accept: */*{CrLf}{CrLf}
     received = waitForReceiveStringOrTimeoutBlocking(
                   UART_232,
                   "+IPD,472:HTTP/1.0 200 OK\r\n",
                   sizeof("+IPD,472:HTTP/1.0 200 OK\r\n"),
                   5000
                );

     if( received ){
        uartWriteString( UART_USB, "\r\nEnvio OK a Thinkspeak \r\n" );
     }
     // Si no lo recibe indica que salio de la funcion
     // waitForReceiveStringOrTimeoutBlocking  por timeout.
     else{
        uartWriteString( UART_USB, "\r\nMAL Thinkspeak\r\n" );
     }
     //FIN Envio Comando Carga datos a Thinkspeak
     delay(20000);
     //Re- INICIA LOOP
  }

   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
   return 0 ;
}

/*==================[end of file]============================================*/

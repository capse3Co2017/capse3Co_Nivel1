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
 * Date: 2016-12-11
 */

/*==================[inclusions]=============================================*/

#include "MEF_ESP.h"  /* <= own header */

#include "sapi.h"                 /* <= sAPI header */

/*==================[macros and definitions]=================================*/
   //parametros constantes
#define ESP_PAUSA 1000
#define ESP_TMO 10000
#define ESP_BAUDRATE 115200
#define ESP_ENTREENVIOSTHINK 20000
#define THINK_WAPIKEY "3EV61E0RD5KV780X"
/*==================[internal data declaration]==============================*/
   //definir textos de cmd
   uint8_t CMD_AT[]="AT\r\n";
   uint8_t CMD_VERCONEXION[]="AT+CWJAP?\r\n";
   uint8_t CMD_CONECTATHINK[]="AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n";
   uint8_t CMD_CANTBYTESENVIO[]="AT+CIPSEND=153\r\n";
   uint8_t CMD_ENVIOATHINK1[]="GET /update?key=";
   //sigue API Write KEY = #define como parametro a ajustar
   uint8_t CMD_ENVIOATHINK2[]="&field1=";
   //sigue DATO 1 = dato externo
   uint8_t CMD_ENVIOATHINK3[]="&field2=";
   //sigue DATO 2 = dato externo
   uint8_t CMD_ENVIOATHINK4[]="&field3=";
   //sigue DATO 3 = dato externo
   uint8_t CMD_ENVIOATHINK5[]="&headers=false HTTP/1.1\r\nHost: api.thingspeak.com\r\nConnection: close\r\nAccept: \*/\*\r\n\r\n";
//"GET /update?key=3EV61E0RD5KV780X&field1=100&field2=200&field3=300&headers=false HTTP/1.1\r\nHost: api.thingspeak.com\r\nConnection: close\r\nAccept: \*/\*\r\n\r\n");

   //definir textos de rpta
   uint8_t RPT_AT_OK[]="OK\r\n";
   uint8_t RPT_VERCONEXION_OK[]="+CWJAP:\"TP-LINK_Besama\"";
   uint8_t RPT_CONECTATHINK_OK[]="CONNECT";
   uint8_t RPT_CANTBYTESENVIO_OK[]="OK\r\n>";
   uint8_t RPT_ENVIOATHINK_OK[]="+IPD,472:HTTP/1.0 200 OK\r\n";

   //1 retardo de time out
   delay_t ESPdelay;

   //  defino estados
   typedef enum {
		ESP_INICIAL,
		ESP_ENVIO_CMD_AT,
		ESP_ESPERO_RPT_AT_OK,
		ESP_ENVIO_CMD_VERCONEXION,
		ESP_ESPERO_RPT_VERCONEXION_OK,
		ESP_ENVIO_CMD_CONECTATHINK,
		ESP_ESPERO_RPT_CONECTATHINK_OK,
		ESP_ENVIO_CMD_CANTBYTESENVIO,
		ESP_ESPERO_RPT_CANTBYTESENVIO_OK,
		ESP_ENVIO_CMD_ENVIOATHINK,
		ESP_ESPERO_RPT_ENVIOATHINK_OK,
		ESP_ESPERO_ENTREENVIOSTHINK,
   }ESP_Estados;
   uint8_t * rpta;
/*==================[internal functions declaration]=========================*/
bool_t ESP_Rpta(void);
void MEF_ESP(void);
/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/
uint8_t HMC_X[]="0010";
uint8_t HMC_Y[]="2000";
uint8_t HMC_Z[]="0300";
/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/



/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void){

   /* ------------- INICIALIZACIONES ------------- */

   /* Inicializar la placa */
   boardConfig();

   /* Inicializar el conteo de Ticks con resolución de 1ms, sin tickHook */
   tickConfig( 1, 0 );

   /* Inicializar UART_USB a 115200 baudios */
   uartConfig( UART_USB, 115200 );

   /* ------------- REPETIR POR SIEMPRE ------------- */
   while(1) {
       MEF_ESP();
   }
   return 0 ;
   }

void MEF_ESP(void){
	   static ESP_Estados estadoESP=ESP_INICIAL;

	   switch (estadoESP) {
		case ESP_INICIAL:
			uartConfig(UART_232, ESP_BAUDRATE);
			delayConfig(&ESPdelay,ESP_PAUSA);
			estadoESP=ESP_ENVIO_CMD_AT;
			break;
		//-----------
		case ESP_ENVIO_CMD_AT:
			if(delayRead(&ESPdelay)){
				uartWriteString(UART_232,(char*)CMD_AT);
                uartWriteString(UART_USB, (char*)CMD_AT);       //salida p debug
				delayConfig(&ESPdelay,ESP_TMO);
				rpta=RPT_AT_OK;
				estadoESP=ESP_ESPERO_RPT_AT_OK;
			}
			break;
		case ESP_ESPERO_RPT_AT_OK:
			if(delayRead(&ESPdelay)){
				delayConfig(&ESPdelay,ESP_PAUSA);
				estadoESP=ESP_ENVIO_CMD_AT;
			}
			if(ESP_Rpta()){
				delayConfig(&ESPdelay,ESP_PAUSA);
				estadoESP=ESP_ENVIO_CMD_VERCONEXION;
			}
			break;
			//-----------
		case ESP_ENVIO_CMD_VERCONEXION:
			if(delayRead(&ESPdelay)){
				uartWriteString(UART_232,(char*)CMD_VERCONEXION);
                uartWriteString(UART_USB, (char*)CMD_VERCONEXION);       //salida p debug
				delayConfig(&ESPdelay,ESP_TMO);
				rpta=RPT_VERCONEXION_OK;
				estadoESP=ESP_ESPERO_RPT_VERCONEXION_OK;
			}
			break;
		case ESP_ESPERO_RPT_VERCONEXION_OK:
			if(delayRead(&ESPdelay)){
				delayConfig(&ESPdelay,ESP_PAUSA);
				estadoESP=ESP_ENVIO_CMD_AT;
			}
			if(ESP_Rpta()){
				delayConfig(&ESPdelay,ESP_PAUSA);
				estadoESP=ESP_ENVIO_CMD_CONECTATHINK;
			}
			break;
			//-----------
		case ESP_ENVIO_CMD_CONECTATHINK:
			if(delayRead(&ESPdelay)){
				uartWriteString(UART_232,(char*)CMD_CONECTATHINK);
                uartWriteString(UART_USB, (char*)CMD_CONECTATHINK);       //salida p debug
				delayConfig(&ESPdelay,ESP_TMO);
				rpta=RPT_CONECTATHINK_OK;
				estadoESP=ESP_ESPERO_RPT_CONECTATHINK_OK;
			}
			break;
		case ESP_ESPERO_RPT_CONECTATHINK_OK :
			if(delayRead(&ESPdelay)){
				delayConfig(&ESPdelay,ESP_PAUSA);
				estadoESP=ESP_ENVIO_CMD_AT;
			}
			if(ESP_Rpta()){
				delayConfig(&ESPdelay,ESP_PAUSA);
				estadoESP=ESP_ENVIO_CMD_CANTBYTESENVIO;
			}
			break;
			//-----------
		case ESP_ENVIO_CMD_CANTBYTESENVIO:
			if(delayRead(&ESPdelay)){
				uartWriteString(UART_232,(char*)CMD_CANTBYTESENVIO);
                uartWriteString(UART_USB, (char*)CMD_CANTBYTESENVIO);       //salida p debug
				delayConfig(&ESPdelay,ESP_TMO);
				rpta=RPT_CANTBYTESENVIO_OK;
				estadoESP=ESP_ESPERO_RPT_CANTBYTESENVIO_OK;
			}
			break;
		case ESP_ESPERO_RPT_CANTBYTESENVIO_OK:
			if(delayRead(&ESPdelay)){
				delayConfig(&ESPdelay,ESP_PAUSA);
				estadoESP=ESP_ENVIO_CMD_CONECTATHINK;
			}
			if(ESP_Rpta()){
				delayConfig(&ESPdelay,ESP_PAUSA);
				estadoESP=ESP_ENVIO_CMD_ENVIOATHINK;
			}
			break;
			//-----------
		case ESP_ENVIO_CMD_ENVIOATHINK:
			if(delayRead(&ESPdelay)){
				uartWriteString(UART_232,(char*)CMD_ENVIOATHINK1);
                uartWriteString(UART_USB, (char*)CMD_ENVIOATHINK1);       //salida p debug
				uartWriteString(UART_232,(char*)THINK_WAPIKEY);
                uartWriteString(UART_USB, (char*)THINK_WAPIKEY);          //salida p debug
				uartWriteString(UART_232,(char*)CMD_ENVIOATHINK2);
                uartWriteString(UART_USB, (char*)CMD_ENVIOATHINK2);       //salida p debug
				uartWriteString(UART_232,(char*)HMC_X);
                uartWriteString(UART_USB, (char*)HMC_X);                   //salida p debug
				uartWriteString(UART_232,(char*)CMD_ENVIOATHINK3);
                uartWriteString(UART_USB, (char*)CMD_ENVIOATHINK3);       //salida p debug
				uartWriteString(UART_232,(char*)HMC_Y);
                uartWriteString(UART_USB, (char*)HMC_Y);                  //salida p debug
				uartWriteString(UART_232,(char*)CMD_ENVIOATHINK4);
                uartWriteString(UART_USB, (char*)CMD_ENVIOATHINK4);       //salida p debug
				uartWriteString(UART_232,(char*)HMC_Z);
                uartWriteString(UART_USB, (char*)HMC_Z);                  //salida p debug
				uartWriteString(UART_232,(char*)CMD_ENVIOATHINK5);
                uartWriteString(UART_USB, (char*)CMD_ENVIOATHINK5);       //salida p debug
				delayConfig(&ESPdelay,ESP_TMO);
				rpta=RPT_ENVIOATHINK_OK;
				estadoESP=ESP_ESPERO_RPT_ENVIOATHINK_OK;
			}
			break;
		case ESP_ESPERO_RPT_ENVIOATHINK_OK:
			if(delayRead(&ESPdelay)){
				delayConfig(&ESPdelay,ESP_PAUSA);
				estadoESP=ESP_ENVIO_CMD_CONECTATHINK;
			}
			if(ESP_Rpta()){
				delayConfig(&ESPdelay,20000);
				estadoESP=ESP_ENVIO_CMD_CONECTATHINK;
			}
			break;
			//-----------

		default:
			estadoESP=ESP_INICIAL;
			break;
	   }
}

bool_t ESP_Rpta(void){
	static uint8_t i = 0;                   //indice interno usado para recorrar arreglo, es estatico asi que se conserva entre llamadas
	uint8_t rx_byte;                        //almacena lo leido del puerto
	bool_t rta = FALSE;                     //rta va a ser SIEMPRE false salvo que finalice el arreglo (0) con TODOS ==

	if (uartReadByte(UART_232, &rx_byte)) {    //uartReadByte saca de a 1 byte cada vez que es llamada
        uartWriteByte(UART_USB, rx_byte);       //eco de lo que entra por el puerto p debug
		if (rx_byte == rpta[i]) {              //se lo compara con el arreglo pasado por puntero
			i++;                               //incrementa indice interno
			if (!rpta[i]) {                    //testea final del arreglo (0) entonces el if !0 da 1 y entra
				i = 0;                        //reset indice interno, debe hacerlo porque es estatico
				rta = TRUE;
                uartWriteString(UART_USB, "\r\n--->Rpta OK\r\n");       //salida p debug
			}
		} else {                             //sale al primer distinto porque los caracteres son distintos
			i = 0;                           //reset indice interno, debe hacerlo porque es estatico
		}
	}
	return rta;

}

   //parametros constantes
   //definir textos de cmd
   //definir textos de rpta
   //1 retardo de time out, espera, etc
   // rpta= puntero a respuesta actual


   //  defino estados
   //maquina de estado:
   //  estado TXx
   //  if Timer(tESPERA)
   //   	tiro comando
   // 	 	echo a andar el timer con tiempo acorde=tmOUT
   //    	set rpta
   // 	 	set estado RXx
   //  estado RXx
   //  if RecivoRpta--->paso al proximo:
   //       echo a andar el timer con tiempo acorde=tESPERA
   //       avanzo estado TXx+1
   //  if Timer(tmOUT)--> reintento:
   //       echo a andar el timer con tiempo acorde=tESPERA
   //       avanzo estado TXx
   //
   // bool_t ReciboRpta funcion rueda libre (1=OK=recibidoOK)
   // 	static indice
   //   comparo rpta[indice] con Rx232[indice] hasta que
   // 	completo el string (llego el /0)=> OK, reset indice,return 1
   //   salgo si al primer distinto  => Nok, reset indice, return 0
   //   avanzo indice
   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */



/*==================[end of file]============================================*/

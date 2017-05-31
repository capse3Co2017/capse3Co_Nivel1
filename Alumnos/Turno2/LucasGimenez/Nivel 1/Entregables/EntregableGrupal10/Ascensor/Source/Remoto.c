/*============================================================================
 * Copyright 2017, Lucas Andres Gimenez, Carlos Eduardo López Aldana.
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
 

 * Date: 2017/04/10

 *===========================================================================*/

/*==================[inlcusiones]============================================*/

#include "sapi.h"		// <= Biblioteca sAPI
#include "Remoto.h"		// <= su propio archivo de cabecera
#include "MEF_ascensorPuertas.h"	// <= Biblioteca MEF ascensor

/*==================[definiciones y macros]==================================*/
//#define ESTADOTEST 1


/*==================[definiciones de datos internos]=========================*/
delay_t timSerial;


/*==================[definiciones de datos globales]=========================*/
//SACAR ESTAS VARIABLES
uint8_t numeroEnString[10];
uint32_t contadorTeclas = 0;





/*==================[declaraciones de datos externos]=========================*/
extern volatile uint32_t flag1DW;
extern int8_t pisoActual;
extern int8_t pisoDestino;
extern estadoMEFASC_t estadoActualAsc;
extern estadoMEFAbreCierraPuerta_t estadoActualPuerta;

extern int almacenarPisos [10];		// Vector para almacenar hasta 10 pisos ingresados correctamente.
extern int indice;				// Indice para recorrer el vector anterior.
extern uint16_t primerDigito; // Variable para almacenar el primer digito ingresado.
extern uint16_t segundoDigito; // Variable para almacenar el segundo digito ingresado.
extern uint32_t indiceTeclaPresionada;



/*==================[declaraciones de funciones externas]====================*/





/*==================[declaraciones de funciones internas]====================*/
char* itoa(int value, char* result, int base);
void uartWriteMiDato(char* str, int value, int base);
void EnviaEstadoInterno(void);
void ActualizaInfoRemoto(void);
void ConfiguraRemoto(void);






//*********************************************************************************************************************
//
//*********************************************************************************************************************
/**
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.

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
//
//*********************************************************************************************************************
void uartWriteMiDato(char* str, int value, int base)
{

uartWriteString(UART_USB, str);
itoa( value, numeroEnString, base);         
uartWriteString( UART_USB, numeroEnString );  
uartWriteString( UART_USB, "\r\n" );  

}
//*********************************************************************************************************************
//*********************************************************************************************************************




//*********************************************************************************************************************
//
//*********************************************************************************************************************
void EnviaEstadoInterno(void)
{
// Esta linea permite el borrado de la terminal en la PC usando RealTerm.
uartWriteString( UART_USB, "\x1b[2J\x1b[H" );  	


// Variables y banderas para el debbuger remoto, se muestran si esta definido ESTADOTEST.
#ifdef ESTADOTEST
uartWriteMiDato("Numero de teclas presionadas = ", contadorTeclas, 10);
// uartWriteMiDato("PrimerDigito = ", primerDigito, 10);
// uartWriteMiDato("SegundoDigito = ", segundoDigito, 10);
#endif   	




uartWriteMiDato("Buf[0]= ", almacenarPisos[0], 10);
uartWriteMiDato("Buf[1]= ", almacenarPisos[1], 10);
uartWriteMiDato("Buf[2]= ", almacenarPisos[2], 10);
uartWriteMiDato("Buf[3]= ", almacenarPisos[3], 10);
uartWriteMiDato("Buf[4]= ", almacenarPisos[4], 10);
uartWriteMiDato("Buf[5]= ", almacenarPisos[5], 10);
uartWriteMiDato("Buf[6]= ", almacenarPisos[6], 10);
uartWriteMiDato("Buf[7]= ", almacenarPisos[7], 10);
uartWriteMiDato("Buf[8]= ", almacenarPisos[8], 10);
uartWriteMiDato("Buf[9]= ", almacenarPisos[9], 10);


uartWriteMiDato("IndiceBuffer = ", indice, 10);

uartWriteString(UART_USB, "Estado del Ascensor: ");
switch(estadoActualAsc)
	{
	case EN_PLANTA_BAJA:
		uartWriteString(UART_USB, "En Planta Baja y ");
		break;
			
	case SUBIENDO:
		uartWriteString(UART_USB, "Subiendo y ");
		break;

	case BAJANDO:
		uartWriteString(UART_USB, "Bajando y ");	
		break;

	case PARADO:
		uartWriteString(UART_USB, "Parado y ");	
		break;

	case YENDO_A_PLANTA_BAJA:
		uartWriteString(UART_USB, "Yendo a Planta Baja y ");
		break;

	case MODO_CONFIGURACION:
		uartWriteString(UART_USB, "Modo Configuracion y ");
		break;

	default:
		break;
 	}      	
	
	
switch(estadoActualPuerta)
	{
	case PUERTA_CERRADA:
		uartWriteString(UART_USB, "Puertas Cerradas\r\n");
		break;

	case ABRIENDO_PUERTA:
		uartWriteString(UART_USB, "Abriendo Puertas\r\n");
		break;

	case PUERTA_ABIERTA:
		uartWriteString(UART_USB, "Puertas Abiertas\r\n");
		break;

	case INTENTANDO_CERRAR_PUERTAS:
		uartWriteString(UART_USB, "Intentado Cerrar Puertas\r\n");
		break;
	
	case CERRANDO_PUERTA:
		uartWriteString(UART_USB, "Cerrando Puertas\r\n");
		break;

	case ALARMA_PUERTA_ABIERTA:
		uartWriteString(UART_USB, "Alarma Puertas Abiertas\r\n");
		break;

	default:
		break;
 	}      

if (Ask_PideNuevoPisoFlag)
	uartWriteString(UART_USB, "Pide nuevo piso = SI\r\n");
else	uartWriteString(UART_USB, "Pide nuevo piso = NO\r\n");

uartWriteMiDato ("Piso Destino = ", pisoDestino, 10);

uartWriteMiDato ("Piso Actual = ", pisoActual, 10);
	
}
//*********************************************************************************************************************
//*********************************************************************************************************************




//*********************************************************************************************************************
//
//*********************************************************************************************************************
void ConfiguraRemoto(void)
{
// UART_USB a 115200 baudios.
uartConfig( UART_USB, 115200 );
delayConfig(&timSerial, 200);   
}
//*********************************************************************************************************************
//*********************************************************************************************************************




//*********************************************************************************************************************
//
//*********************************************************************************************************************
void ActualizaInfoRemoto(void)
{
if (delayRead(&timSerial))
	{
	if (estadoActualAsc != MODO_CONFIGURACION)
		EnviaEstadoInterno();
// Codigo de prueba, presionando la tecla 4 se cargan valores en las 10 posiciones del buffer de pisos
// y de fija indice a 10.	
	if (!gpioRead(TEC4) && (estadoActualAsc != MODO_CONFIGURACION))
		{
		almacenarPisos[0] = 6;
		almacenarPisos[1] = 9;
		almacenarPisos[2] = 12;
		almacenarPisos[3] = 17;
		almacenarPisos[4] = 20;
		almacenarPisos[5] = 10;
		almacenarPisos[6] = -5;
		almacenarPisos[7] = 5;
		almacenarPisos[8] = -3;
		almacenarPisos[9] = 8;
		indice = 10;
		}
	}
}
//*********************************************************************************************************************
//*********************************************************************************************************************




/*==================[fin del archivo]========================================*/
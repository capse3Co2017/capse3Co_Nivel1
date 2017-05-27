/* Copyright 2017, Eric Pernia.
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
 */
/*
 * Date: 2017-04-14 
 */

/*==================[inlcusiones]============================================*/

#include "mefPuertas.h"   // <= su propio archivo de cabecera
#include "sapi.h"         // <= Biblioteca sAPI

/*==================[definiciones y macros]==================================*/

#define INDICADOR_PUERTA_ABIERTA      LEDG // puerta abierta.
#define INDICADOR_ALARMA              LEDR // alarma puerta abierta.
#define INDICADOR_PUERTA_ABRIENDOSE   LED1 // puerta abriéndose.
#define INDICADOR_PUERTA_CERRANDOSE   LED2 // puerta cerrándose.

/*==================[definiciones de datos internos]=========================*/

/*==================[definiciones de datos externos]=========================*/

extern bool_t modoConfiguracion;

extern uint32_t velocidadPuertas;   // En segundos, de 1 en adelante

estadoMefPuertas_t estadoMefPuertas;

/*==================[declaraciones de funciones internas]====================*/

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

#define INDICADOR_PUERTA_ABIERTA      LEDG // puerta abierta.
#define INDICADOR_ALARMA              LEDR // alarma puerta abierta.
#define INDICADOR_PUERTA_ABRIENDOSE   LED1 // puerta abriéndose.
#define INDICADOR_PUERTA_CERRANDOSE   LED2 // puerta cerrándose.

void secuenciaDeAperturaDePuertas( void ){/*
   //gpioWrite( INDICADOR_PUERTA_ABRIENDOSE, ON );
   gpioWrite( LEDG, ON );
   delay( velocidadPuertas*1000 );
   //gpioWrite( INDICADOR_PUERTA_ABIERTA, ON );
   //gpioWrite( INDICADOR_PUERTA_ABRIENDOSE, OFF );
   gpioWrite( LEDG, OFF );*/
   uartWriteString( UART_USB, "Puertas abiertas\r\n" ); 
}

void secuenciaDeCerradoDePuertas( void ){/*
   gpioWrite( LEDR, ON );
   //gpioWrite( INDICADOR_PUERTA_ABIERTA, OFF );
   //gpioWrite( INDICADOR_PUERTA_CERRANDOSE, ON );
   delay( velocidadPuertas*1000 );
   //gpioWrite( INDICADOR_PUERTA_CERRANDOSE, OFF );   
   gpioWrite( LEDR, OFF );*/
   uartWriteString( UART_USB, "Puertas cerradas\r\n" ); 
}

// Inicializar la MEF de puertas
void puertasInicializarMEF( estadoMefPuertas_t estadoInicial ){
   estadoMefPuertas = estadoInicial;
}

// Actualizar la MEF de puertas
void puertasActualizarMEF( void ){
   estadoMefPuertas = 0;
}

/*==================[fin del archivo]========================================*/

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

#include "mefIngresoPiso.h"   // <= su propio archivo de cabecera
#include "driverDisplays7Segmentos.h"
#include "driverTecladoMatricial.h"
#include "sapi.h"             // <= Biblioteca sAPI

/*==================[definiciones y macros]==================================*/

/*==================[definiciones de datos internos]=========================*/

/*==================[definiciones de datos externos]=========================*/

extern bool_t modoConfiguracion;
extern int8_t pisoActual;
extern int8_t pisoDestino;

int8_t pisosPendientes[10] = { -128, -128, -128, -128, -128,
                               -128, -128, -128, -128, -128 };
                               
int8_t pisoPendiente = -128;

/*==================[declaraciones de funciones internas]====================*/

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/


bool_t cancelarPeticionPendiente( int8_t pisoActual ){
   pisoPendiente = -128;
   return FALSE;
}

// Funcion de test: Devuelve TRUE si hay peticion de subir pendiente
bool_t hayPeticionDeSubirPendiente( int8_t pisoActual ){
   if( ( pisoPendiente != -128) && ( pisoPendiente > pisoActual) ){
      pisoDestino = pisoPendiente;
      pisoPendiente = -128;
      uartWriteString( UART_USB, "Hay peticion de subr pendiente\r\n" ); 
      return TRUE;
   }
   return FALSE;
}

// Funcion de test: Devuelve TRUE si hay peticion de bajar pendiente
bool_t hayPeticionDeBajarPendiente( int8_t pisoActual ){
   if( ( pisoPendiente != -128) && ( pisoPendiente < pisoActual) ){
      pisoDestino = pisoPendiente;
      pisoPendiente = -128;
      uartWriteString( UART_USB, "Hay peticion de bajar pendiente\r\n" ); 
      return TRUE;
   }
   return FALSE;
}


// Inicializar la MEF de ingreso de piso
void ingresoPisoAInicializarMEF( void ){
   //estadoMefAscensor = EN_PLANTA_BAJA;
}

// Actualizar la MEF de ingreso de piso
void ingresoPisoActualizarMEF( void ){
   if( !gpioRead( TEC1 ) ){
      if( pisoActual != -1 )
         pisoPendiente = -1;
   }
   if( !gpioRead( TEC2 ) ){
      if( pisoActual != 0 )
         pisoPendiente = 0;
   }
   if( !gpioRead( TEC3 ) ){
      if( pisoActual != 1 )
         pisoPendiente = 1;
   }
   if( !gpioRead( TEC4 ) ){
      if( pisoActual != 2 )
         pisoPendiente = 2;
   }
}

/*==================[fin del archivo]========================================*/

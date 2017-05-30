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
typedef enum{
  MODO_NORMAL, 
  MODO_CONFIGURACION}
estadoMefIngresoPiso_t

typedef enum{
  LEER_DIGITO_O_SIGNO,
  LEER_DIGITO_O_LETRA,
  LEER_LETRA}
estadoMefEstadoNormal_t;

/*==================[definiciones de datos internos]=========================*/
static estadoMefIngresoPiso_t estadoMefIngresoPiso;
static estadoMefEstadoNormal_t estadoMefEstadoNormal;

static flagEstadoModoNormal=FALSE; 
static flagEstadoModoConfiguracion=FALSE;
static flagEstadoLeerDigitoOSigno=FALSE;
static flagEstadoLeerDigitoOLetra=FALSE;
static flagEstadoLeerLetra=FALSE;


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
void inicializarMefIngresoPiso( void ){
   //estadoMefAscensor = EN_PLANTA_BAJA;
   estadoMefIngresoPiso=ESTADO_NORMAL;
}

// Actualizar la MEF de ingreso de piso
void actualizarMefIngresoPiso( void ){
  switch (estadoMefIngresoPiso){
  case ESTADO_NORMAL:
    if(flagEstadoNormal==FALSE){
      //Entry
      flagEstadoNormal==TRUE;
      inicializarMefEstadoNormal();
    }
    //Internal
    if (!modoConfiguracion)
      actualizarMefEstadoNormal();
    else
      estadoMefIngresoPiso=ESTADO_CONFIGURACION;
    //exit
    if(estadoMefIngresoPiso != MODO_NORMAL)
      flagEstadoEstadoNormal=FALSE;
  break;
  case ESTADO_CONFIGURACION:
    if(!modoConfiguracion)
      estadoMefIngresoPiso=ESTADO_NORMAL;
  break;
  default:
    inicializarMefIngresoPiso();
  }
}

void inicializarMefEstadoNormal( void ){
   //estadoMefAscensor = EN_PLANTA_BAJA;
   estadoMefEstadoNormal=LEER_DIGITO_O_SIGNO;
}

// Actualizar la MEF de ingreso de piso
void actualizarMefEstadoNormal( void ){
  bool_t eDigitoValido=FALSE;
  
  switch(estadoMefEstadoNormal){
  case LEER_DIGITO_O_SIGNO:
    if(flagEstadoLeerDigitoOSigno==FALSE){
      flagEstadoLeerDigitoOSigno==TRUE;
      tecla=255;
    }
    //Internal
    tecla=readkey();
    if(tecla>='0' && tecla<='9'){
      piso=tecla-'0';
      signo=0;
      driverDisplaySh(Display,tecla-'0');
      driverDisplayShow();
      eDigitoValido=TRUE;
    }
    if(tecla=='-'){
      piso=0;
      signo=1;
      driverDisplaySh(Display,CHAR_MENOS);
      driverDisplayShow();
      eDigitoValido=TRUE;
    }
    //Evaluacion transiciones
    if(eDigitoValido)
      estadoMefEstadoNormal=LEER_DIGITO_O_LETRA;
    //Exit
    if(estadoMefEstadoNormal != LEER_DIGITO_O_SIGNO){
      flagEstadoLeerDigitoOSigno=FALSE;
    }  
  break;
  case LEER_DIGITO_O_LETRA:
    if(flagEstadoLeerDigitoOLetra==FALSE){
      flagEstadoLeerDigitoOLetra==TRUE;
      delayConfig(&delayBase1seg,1000);
      temporizador=cDelayIngresoTecla;
    }
    if(delayRead(&delayBase1seg && temporizador>0)
      temporizador--;
    tecla=readkey;
    if(tecla>='0' && tecla<='9' && signo){
      piso=-(tecla-'0');
      eDigitoValido=TRUE;
      driverDisplaySh(Display,tecla-'0');
      driverDisplayShow(Display);
    }
    if(tecla>='0' && tecla<='9' && !signo){
      piso=piso*10+tecla-'0';
      eDigitoValido=TRUE;
      driverDisplaySh(Display,tecla-'0');
      driverDisplayShow(Display);
    }
    // Evaluar Transiciones
    if(DigitoValido)
      estadoMefEstadoNormal=LEER_LETRA;
    if(!DigitoValido && (temporizador==0 || tecla='B')){
      driverDisplayErase(Display);
      driverDisplayShow(Display);
      piso=0;
      signo=0;
      estadoMefEstadoNormal=LEER_DIGITO_O_SIGNO;
    }
    //Exit
    if(estadoMefEstadoNormal != LEER_DIGITO_O_LETRA)
      flagEstadoLeerDigitoOLetra=FALSE;
  break;
  case LEER_LETRA:
    if(flagEstadoLeerLetra==FALSE){
      flagEstadoLeerLetra==TRUE;
      delayConfig(&delayBase1seg,1000);
      temporizador=cDelayIngresoTecla;
    }
    
    if(delayRead(&delayBase1seg && temporizador>0)
      temporizador--;
      
    tecla=readkey();
    //Evaluar Transiciones
    if(tecla='A'){
      guardarPiso(piso);
      estadoMefEstadoNormal=LEER_DIGITO_O_SIGNO;
    }
    if(tecla='B' || temporizador==0){
      driverDisplayErase(Display);
      driverDisplayShow(Display);
      piso=0;
      signo=0;
      estadoMefEstadoNormal=LEER_DIGITO_O_SIGNO;
    }
    //Exit
    if(estadoMefEstadoNormal != LEER_LETRA)
      flagEstadoLeerLetra=FALSE;      
  break;
  default:
    inicializarMefEstadoNormal();
  }
}

/*==================[fin del archivo]========================================*/

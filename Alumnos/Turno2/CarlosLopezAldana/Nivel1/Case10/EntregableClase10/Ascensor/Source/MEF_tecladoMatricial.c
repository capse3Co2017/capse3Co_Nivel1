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

#include "sapi.h"                     // <= Biblioteca sAPI
#include "MEF_tecladoMatricial.h"   // <= su propio archivo de cabecera
#include "MEF_ascensorPuertas.h"	// <= Biblioteca MEF ascensor

/*==================[definiciones y macros]==================================*/

typedef enum{
    EN_ESPERA_DE_DIGITO_1,
	EN_ESPERA_DE_DIGITO_2,
	EN_ESPERA_DE_LETRA,
	GUARDAR_PISO
	
} mefTecladoMatricial_t;

typedef enum{
   ESCANEANDO_TECLADO,
   APRETANDO_TECLA,
   IDENTIFICAR_TECLA_Y_ESCRIBIR
} mefScanTeclado_t;


/*==================[definiciones de datos internos]=========================*/
static delay_t delayAntirebote;


static uint16_t primerDigito  = 0; // Variable para almacenar el primer digito ingresado.
static uint16_t segundoDigito = 0; // Variable para almacenar el segundo digito ingresado.
static uint16_t confirmar = 0;     // Variable para almacenar si se confirma o cancela en ingreso de los datos.

static uint32_t indiceTeclaPresionada = 0; // Variable que me permite saber cuantas veces fue presionado el teclado y asi saber que tecla se espera.
static uint32_t indiceTeclaGuardar = 0;    // Variable que me permite conocer que digito fue ingresado para su posterior almacenamiento. (Similar a la anterior pero se usa en main).


// Guarda la ultima tecla apretada
static uint16_t key = 0;


static mefTecladoMatricial_t estadoMefTecladoMatricial;

static bool_t hayTeclaDisponible = 0;
static uint16_t teclaPresionada = 0xff;

// Variable de estado de la MEF captura estodo de tecla.
static mefScanTeclado_t estadoMefScanTeclado;

/* Vector para almacenar los pines que van a estar conectados a las filas del teclado matricial. */
static uint8_t pinesFila[4] = {
    RS232_TXD,
    CAN_RD,
    CAN_TD,
    T_COL1
};

/* Vector para almacenar los pines que van a estar conectados a las columnas del teclado matricial. */
static uint8_t pinesColumna[4] = {
    T_FIL0,
    T_FIL3,
    T_FIL2,
    T_COL0
};

static uint16_t pinesTeclado[16] = {
                                1, 2, 3, 'A',
                                4, 5, 6, 'B',
                                7, 8, 9, 'C',
                                '*', 0, '#', 'D'
};

/*==================[definiciones de datos globales]=========================*/
int almacenarPisos [10];    // Vector para almacenar hasta 10 pisos ingresados correctamente.
int indice = 0;             // Indice para recorrer el vector anterior.



/*==================[declaraciones de datos externos]=========================*/
extern bool_t flagConfiguracion;
extern uint32_t contadorTeclas;
extern int8_t pisoActual;
extern estadoMEFASC_t estadoActualAsc;
extern uint8_t maximoDePisos;
extern uint8_t maximoDeSubsuelos;



/*==================[declaraciones de funciones externas]====================*/
extern estadoMEFAbreCierraPuerta_t estadoActualPuerta;



/*==================[declaraciones de funciones internas]====================*/
void EnviaEstadoInterno(void);
void InicializarMEF_tecladoMatrical(void);
void ActualizarMEF_tecladoMatricial (void);
void cancelar (void);
void configurarTecladoMatricial(void);
void guardarPisoSimple(int primerDigito);
void guardarPisoDoble (int primerDigito, int segundoDigito);
bool_t ingresarDigito(void);
bool_t ScanTeclas(void);
void ActualizaMEFScanTecla(void);
bool_t VerificaExclusionPisoSimple(int carga);

//*********************************************************************************************************************
//
//*********************************************************************************************************************
void configurarTecladoMatricial (void){
    
uint8_t i = 0;  // Variable para recorrer el vector de filas y columnas.
    
/* Se recorre todo el vector de filas para configurar los pines como salida.*/
    for ( i = 0; i < 4; i++){
        gpioConfig(pinesFila[i], GPIO_OUTPUT);
    } 

/* Se recorre todo el vector de columnas para configurar los pines como entrada en PULLUP (resistencia interna).*/
    for ( i = 0; i < 4; i++){
        gpioConfig(pinesColumna[i], GPIO_INPUT_PULLUP);
    }
}
//*********************************************************************************************************************
//*********************************************************************************************************************




//*********************************************************************************************************************
//
//*********************************************************************************************************************
void InicializarMEF_tecladoMatrical(void) {

estadoMefTecladoMatricial = EN_ESPERA_DE_DIGITO_1;
configurarTecladoMatricial();
estadoMefScanTeclado = ESCANEANDO_TECLADO;
delayConfig(&delayAntirebote, 40);
}
//*********************************************************************************************************************
//*********************************************************************************************************************




//*********************************************************************************************************************
//
//*********************************************************************************************************************
bool_t ScanTeclas(void)
{
// Funcion devuelve 1 si hay por lo menos una presionada.
// Carga en "key" el indice de la tecla presionada.
	
bool_t ret = 0;

uint8_t f = 0; // Variable para recorrer los vectores de filas.
uint8_t c = 0; // Variable para recorrer los vectores de columnas.

key = 0xff;

// Lo primero que hacemos es poner todas las filas en estado "bajo".
for (f=0; f<4; f++)
	gpioWrite(pinesFila[f], 0);
for (c=0; c<4; c++)
	{
	// Si leo un estado BAJO en una columna entonces puede haber una tecla presionada-
        if (!gpioRead(pinesColumna[c]))
		{
		//Se ponen todas las filas en "alto", excepto la primera que se deja en "bajo".
		for (f=1; f<4 ; f++)
			gpioWrite(pinesFila[f], 1);
		// Se procede a buscar la tecla presionada, para ello se recorren las filas. Acá lo que buscamos es la coordenada de la fila.
		for (f=0; f<4; f++)
			{
			// El siguiente if es para evitar un índice negativo en el vector.
			if (f > 0)
				gpioWrite(pinesFila[f-1], 1);
			gpioWrite(pinesFila[f], 0);

			//Se testea la columna actual con la fila actual, si detectamos un estado "BAJO"
			//en alguna columna, significa que hemos encontrado la coordenada de la fila 
			//correspondiente. Con esta coordenada ya podemos detectar que tecla fue presionada.
			if (!gpioRead(pinesColumna[c]))
				{
				ret = 1;
				key = f * 4 + c;
				return ret;
				/*
				  Formula de teclas para Teclado Matricial (Keypad)
				  de 4 filas (rows) * 4 columnas (columns)

				      c0 c1 c2 c3    Si se presiona la tecla row[f] column[c]:
				  f0  0  1  2  3    
				  f1  4  5  6  7    valor = (f) * cantidadDeColumnas + (c)
				  f2  8  9 10 11    
				  f3 12 13 14 15 
				*/
				}
			}
		}
	}
return ret;
}
//*********************************************************************************************************************
//*********************************************************************************************************************



//*********************************************************************************************************************
//
//*********************************************************************************************************************
void ActualizaMEFScanTecla(void)
{
// Esta rutina hace la captura de una tecla con anti-bouncing.
// Al haber confirmado el proceso de una tecla pone "hayTeclaDisponible" en 1;
// Y la tecla confirmada en "teclaPresionada".

switch(estadoMefScanTeclado)
	{
	case ESCANEANDO_TECLADO:
		// Chequear condiciones de transicion de estado.
		// Hay una tecla presionada?
		if (ScanTeclas())
			{
			teclaPresionada = key;
			estadoMefScanTeclado = APRETANDO_TECLA;
			}
	break;


	case APRETANDO_TECLA:
		// Salida en el estado.
		// El delay se incrementa solo!
		// Chequear condiciones de transicion de estado.
		if (delayRead(&delayAntirebote))
			{
			ScanTeclas();
			// Esta la misma tecla presionada?
			if (teclaPresionada == key)
				estadoMefScanTeclado = IDENTIFICAR_TECLA_Y_ESCRIBIR;
			// No esta presionada la misma tecla o ninguna.
			else	{
				estadoMefScanTeclado = ESCANEANDO_TECLADO;
				teclaPresionada = 0xff;
				}
			}      
	break;


	case IDENTIFICAR_TECLA_Y_ESCRIBIR:
		// Se solto la tecla?
		if (!(ScanTeclas()))
			{
			estadoMefScanTeclado = ESCANEANDO_TECLADO;  
			hayTeclaDisponible = 1;
			contadorTeclas++;
			}
	break;

	
	default:
	InicializarMEF_tecladoMatrical();
	break;
   }
}
//*********************************************************************************************************************
//*********************************************************************************************************************




//*********************************************************************************************************************
//
//*********************************************************************************************************************
void ActualizarMEF_tecladoMatricial (void)
{ 
if (estadoActualAsc != MODO_CONFIGURACION)
	{
	ActualizaMEFScanTecla();
		
	switch (estadoMefTecladoMatricial) {

		case EN_ESPERA_DE_DIGITO_1:
			if( ingresarDigito() ) {
					estadoMefTecladoMatricial = EN_ESPERA_DE_DIGITO_2;
			}
		break;
			
		case EN_ESPERA_DE_DIGITO_2:
				if( ingresarDigito() ) {
						estadoMefTecladoMatricial = EN_ESPERA_DE_LETRA;
					

					if (pinesTeclado[segundoDigito] == 'A') {
						guardarPisoSimple (primerDigito);
						estadoMefTecladoMatricial = EN_ESPERA_DE_DIGITO_1;

					}
					else if (pinesTeclado[confirmar] == 'B') {
						estadoMefTecladoMatricial = EN_ESPERA_DE_DIGITO_1;
						cancelar();
					}
				}
			break;

		case EN_ESPERA_DE_LETRA:
				if ( ingresarDigito() ) {
						estadoMefTecladoMatricial = EN_ESPERA_DE_LETRA;
					
					if (pinesTeclado[confirmar] == 'A' ) {
						estadoMefTecladoMatricial = GUARDAR_PISO;
						guardarPisoDoble (primerDigito, segundoDigito);
					}

					else if (pinesTeclado[confirmar] == 'B') {
						estadoMefTecladoMatricial = EN_ESPERA_DE_DIGITO_1;
						cancelar();
					}
				}
			break;

		default:	
			InicializarMEF_tecladoMatrical();
		break;
		}		
	}
}
//*********************************************************************************************************************
//*********************************************************************************************************************




//*********************************************************************************************************************
//
//*********************************************************************************************************************
bool_t ingresarDigito (void)
{
bool_t ret = FALSE;	

/* Se testea la columna actual con la fila actual, si detectamos un estado "BAJO"
    en alguna columna, significa que hemos encontrado la coordenada de la fila 
    correspondiente. Con esta coordenada ya podemos detectar que tecla fue presionada.*/
if (hayTeclaDisponible)
	{
	hayTeclaDisponible = 0;
	ret = TRUE;
                
	/* Etapa en la cual se evalua que digito se ingreso, basicamente llevando la cuenta de las veces que se presionó una tecla*/
	switch (indiceTeclaPresionada)
		{
		case 0:  primerDigito = teclaPresionada;
			   indiceTeclaPresionada = 1;
		break;

		case 1:  segundoDigito = teclaPresionada;
			    if ((pinesTeclado[segundoDigito] == 'A') || (pinesTeclado[segundoDigito] == 'B')) {
				indiceTeclaPresionada = 0;
				indiceTeclaGuardar = 0;
			    }
			    else {
				indiceTeclaPresionada = 2;
			    }
		break;
		    
		case 2:  confirmar = teclaPresionada;
			   indiceTeclaPresionada= 0;
			   indiceTeclaGuardar = 1;
		break;
		}
	}
return ret;
}    
//*********************************************************************************************************************
//*********************************************************************************************************************




//*********************************************************************************************************************
//
//*********************************************************************************************************************
bool_t VerificaExclusionPisoSimple(int carga)
{
// Si la funcion retorna 1 permite guardar.

bool_t salida = 1;

// Si el ascensor se esta moviendo se permite cargar el piso, aunque sea el mismo piso.
// El piso que se pide guardar es el mismo al que se encuentra actualmente el asccensor?
if ((carga == pisoActual) && (estadoActualAsc != SUBIENDO) && (estadoActualAsc != BAJANDO))
	salida = 0;

if ((carga == almacenarPisos[0]) || (carga == almacenarPisos[1]) || (carga == almacenarPisos[2]) || (carga == almacenarPisos[3]) ||
    (carga == almacenarPisos[4]) || (carga == almacenarPisos[5]) || (carga == almacenarPisos[6]) || (carga == almacenarPisos[7]) ||
    (carga == almacenarPisos[8]) || (carga == almacenarPisos[9]))
	salida = 0;
    
return salida;
}
//*********************************************************************************************************************
//*********************************************************************************************************************




//*********************************************************************************************************************
//
//*********************************************************************************************************************
void guardarPisoSimple (int primerDigito)
{
/*======= Funcion que almacena el piso ingresado, de un solo digito, en el vector =======*/
int carga = 0;

carga = pinesTeclado[primerDigito];
// Se verifica si el piso no debe ser almacenado.
if (VerificaExclusionPisoSimple(carga))
	{
	/*======= Etapa de guardar un subsuelo (numero negativo) en el vector de almacenamiento =======*/
	if (indice < 10)
		{
		if (indice > 0)
			almacenarPisos[indice] = carga;
		else
			almacenarPisos[0] = carga;
		indice ++;
		}
	}
/*======= Etapa para reinicializar las variables utilizadas y prepararlas para el proximo ingreso =======*/
primerDigito  = 0;
segundoDigito = 0;
indiceTeclaGuardar = 0;
}
//*********************************************************************************************************************
//*********************************************************************************************************************




//*********************************************************************************************************************
//
//*********************************************************************************************************************
void guardarPisoDoble (int primerDigito, int segundoDigito) {
	
/*======= Funcion que almacena el piso ingresado, de dos digitos, en el vector =======*/

int carga = 0xff;

/*======= Etapa de guardar un subsuelo (numero negativo) en el vector de almacenamiento =======*/
if ((pinesTeclado[primerDigito] == '#') && (pinesTeclado[segundoDigito] <= maximoDeSubsuelos))
	carga = 0 - pinesTeclado[segundoDigito];
else if (!(pinesTeclado[primerDigito] == '#'))
	carga = (pinesTeclado[primerDigito] * 10) + pinesTeclado[segundoDigito];

/*======= Etapa para verificar si se desea entrar al modo configuracion =======*/

if ((carga == 99) && (pisoActual == 0) && (estadoActualPuerta == PUERTA_ABIERTA))
	flagConfiguracion = TRUE;


// Se verifica si el piso no debe ser almacenado.
if (VerificaExclusionPisoSimple(carga) && (carga != 0xff))
	{
	/*======= Etapa de carga en el vector que almacena los pisos ingresados =======*/	
	if (indice < 10)
		{
		if (carga <= maximoDePisos)
			{
			if (indice > 0)
				almacenarPisos[indice] = carga;
			else 
				almacenarPisos[0] = carga;
			indice++;
			}
		}
	}
/*======= Etapa para reinicializar las variables utilizadas y prepararlas para el proximo ingreso =======*/
primerDigito  = 0;
segundoDigito = 0;
indiceTeclaGuardar = 0;
}
//*********************************************************************************************************************
//*********************************************************************************************************************




//*********************************************************************************************************************
//
//*********************************************************************************************************************
void cancelar (void) {
    
/*======= Para cancelar la operacion le seteamos un valor no valido a las siguientes variables =======*/
uint16_t primerDigito  = 0; 
uint16_t segundoDigito = 0; 
uint16_t confirmar = 0;     

uint32_t indiceTeclaPresionada = 0; 
uint32_t indiceTeclaGuardar = 0;
    
}
//*********************************************************************************************************************
//*********************************************************************************************************************




/*==================[fin del archivo]========================================*/

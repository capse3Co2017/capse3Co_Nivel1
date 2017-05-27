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

#include "driverTecladoMatricial.h"   // <= su propio archivo de cabecera
#include "sapi.h"                     // <= Biblioteca sAPI

/*==================[definiciones y macros]==================================*/

typedef enum{
   ESCANEANDO_TECLADO,
   APRETANDO_TECLA,
   IDENTIFICAR_TECLA_Y_ESCRIBIR
} mefTecladoMatricial_t;

/*==================[definiciones de datos internos]=========================*/

/*==================[definiciones de datos externos]=========================*/

// Guarda la ultima tecla apretada
uint16_t key = 0;

/* Pines del teclado matricial */

// Pines conectados a las Filas --> Salidas (MODO = OUTPUT)
uint8_t keypadRowPins[4] = {
   RS232_TXD, // Row 0
   CAN_RD,    // R1
   CAN_TD,    // R2
   T_COL1     // R3
};

// Pines conectados a las Columnas --> Entradas con pull-up (MODO = INPUT_PULLUP)
uint8_t keypadColPins[4] = {
   T_FIL0,    // Column 0
   T_FIL3,    // C1
   T_FIL2,    // C2
   T_COL0     // C3
};


// Vector para mostrar tecla presionada por UART
uint16_t asciiKeypadKeys[16] = {
                                '1', '2', '3', 'A',
                                '4', '5', '6', 'B',
                                '7', '8', '9', 'C',
                                '*', '0', '#', 'D'
                               };

// Vector para mostrar tecla presionada en el display 7 segmentos
uint16_t keypadKeys[16] = {
                               1,    2,    3, 0x0a,
                               4,    5,    6, 0x0b,
                               7,    8,    9, 0x0c,
                            0x0e,    0, 0x0f, 0x0d
                          };


// Variable de estado de la MEF
mefTecladoMatricial_t estadoMefTecladoMatricial;
                        
                          
/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

void configurarTecladoMatricial( void );
void inicializarTecladoMatrical( void );
void actualizarTecladoMatrical( void );

/*
// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void ){

   // ---------- CONFIGURACIONES ------------------------------
   boardConfig(); // Inicializar y configurar la plataforma  
   uartConfig( UART_USB, 115200 ); // Configurar UART a 115200 
  
   configurarTecladoMatricial(); // Configurar teclado matricial
      
   // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE )
   {
      if( leerTecladoMatricial() ){
         uartWriteByte( UART_USB, asciiKeypadKeys[key] );
         delay(500);
      }
   } 

   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta 
   // directamenteno sobre un microcontroladore y no es llamado/ por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
}
*/

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

void configurarTecladoMatricial( void ){
   
   uint8_t i = 0;
   
   // Configure Rows as Outputs
   for( i=0; i<4; i++ ){
      gpioConfig( keypadRowPins[i], GPIO_OUTPUT );
   }

   // Configure Columns as Inputs with pull-up resistors enable
   for( i=0; i<4; i++ ){
      gpioConfig( keypadColPins[i], GPIO_INPUT_PULLUP );
   }
}



void guardarEnBuffer( uint8_t tecla )
{
uartWriteString( UART_USB, "\x1b[2J\x1b[H" );  	
uartWriteString( UART_USB, "La tecla presionada fue: " );  	
uartWriteByte( UART_USB, asciiKeypadKeys[tecla]); 
uartWriteString( UART_USB, "\r\n" );  
}


void inicializarTecladoMatrical( void ){
configurarTecladoMatricial();
estadoMefTecladoMatricial = ESCANEANDO_TECLADO;
}



//*********************************************************************************************************************
//
//*********************************************************************************************************************
bool_t ScanTeclas()
{
// Funcion devuelve 1 si hay por lo menos una presionada.
// Carga en "key" el indice de la tecla presionada.
	
bool_t ret = 0;

uint8_t f = 0; // Variable para recorrer los vectores de filas.
uint8_t c = 0; // Variable para recorrer los vectores de columnas.

// Lo primero que hacemos es poner todas las filas en estado "bajo".
for (f=0; f<4; f++)
	gpioWrite(keypadRowPins[f], 0);
for (c=0; c<4; c++)
	{
	// Si leo un estado BAJO en una columna entonces puede haber una tecla presionada-
        if (!gpioRead(keypadColPins[c]))
		{
		//Se ponen todas las filas en "alto", excepto la primera que se deja en "bajo".
		for (f=1; f<4 ; f++)
			gpioWrite(keypadRowPins[f], 1);
		// Se procede a buscar la tecla presionada, para ello se recorren las filas. Acá lo que buscamos es la coordenada de la fila.
		for (f=0; f<4; f++)
			{
			// El siguiente if es para evitar un índice negativo en el vector.
			if (f > 0)
				gpioWrite(keypadRowPins[f-1], 1);
			gpioWrite(keypadRowPins[f], 0);

			//Se testea la columna actual con la fila actual, si detectamos un estado "BAJO"
			//en alguna columna, significa que hemos encontrado la coordenada de la fila 
			//correspondiente. Con esta coordenada ya podemos detectar que tecla fue presionada.
			if (!gpioRead(keypadColPins[c]))
				{
				ret = 1;
				key = f * 4 + c;
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
void actualizarTecladoMatrical( void )
{
static uint16_t teclaPresionada = 0xff;
static delay_t delayAntirebote;
static bool_t flagEstadoApretandoTecla = 0;


switch(estadoMefTecladoMatricial)
	{
	case ESCANEANDO_TECLADO:
		gpioWrite(LEDR, ON);
		// Chequear condiciones de transicion de estado.
		// Hay una tecla presionada?
		if (ScanTeclas())
			{
			teclaPresionada = key;
			estadoMefTecladoMatricial = APRETANDO_TECLA;
			gpioWrite(LEDR, OFF);
			}
	break;


	case APRETANDO_TECLA:
		gpioWrite(LEDG, ON);
		// Con un flag ejecuto una vez al ingresar al estado.
		if (flagEstadoApretandoTecla == 0)
			{
			flagEstadoApretandoTecla= 1;
			delayConfig(&delayAntirebote, 40);
			}
		// Salida en el estado.
		// El delay se incrementa solo!
		// Chequear condiciones de transicion de estado.
		if (delayRead(&delayAntirebote))
			{
			ScanTeclas();
			// Esta la misma tecla presionada?
			if (teclaPresionada == key)
				{
				estadoMefTecladoMatricial = IDENTIFICAR_TECLA_Y_ESCRIBIR;
				flagEstadoApretandoTecla = 0;
				gpioWrite(LEDG, OFF);
				}
			// No esta presionada la misma tecla.
			else	{
				estadoMefTecladoMatricial = ESCANEANDO_TECLADO;
				teclaPresionada = 0xff;
				flagEstadoApretandoTecla = 0;
				gpioWrite(LEDG, OFF);
				}
			}      


	break;


	case IDENTIFICAR_TECLA_Y_ESCRIBIR:
		// Se solto la tecla?
		if (!(ScanTeclas()))
			{
			estadoMefTecladoMatricial = ESCANEANDO_TECLADO;  
			guardarEnBuffer(teclaPresionada);
			}
	break;

	
	default:
	inicializarTecladoMatrical();
	break;
   }
}

/*==================[fin del archivo]========================================*/
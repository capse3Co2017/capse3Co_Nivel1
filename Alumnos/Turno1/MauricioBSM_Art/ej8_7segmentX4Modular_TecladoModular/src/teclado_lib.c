#include "teclado_lib.h"
#include "sapi.h"
#define CANT_FILAS 4
#define CANT_COLUMNAS 2

// Pines conectados a las Filas --> Salidas (MODO = OUTPUT)
uint8_t keypadRowPins[CANT_FILAS] = {
		   T_FIL0,    // Row 0
		   T_FIL1,    // R1
		   T_FIL2,    // R2
		   T_FIL3     // R3
};
// Pines conectados a las Columnas --> Entradas con pull-up (MODO = INPUT_PULLUP)
uint8_t keypadColPins[CANT_COLUMNAS] = {
		   T_COL0,     // C0
		   T_COL1      // C1
};

// Vector para mostrar tecla presionada en el display 7 segmentos -->numeros
uint16_t keypadKeys[CANT_COLUMNAS*CANT_FILAS] = {
                               0x0a,0x0b,
                               3,    6,
                               2,    5,
                               1,    4
                          };

delay_t teclado_debounce;

void tecladoConfig( void ){

   uint8_t i = 0;

   // Configure Rows as Outputs
   for( i=0; i<CANT_FILAS; i++ ){
      gpioConfig( keypadRowPins[i], GPIO_OUTPUT );
   }

   // Configure Columns as Inputs with pull-up resistors enable
   for( i=0; i<CANT_COLUMNAS; i++ ){
      gpioConfig( keypadColPins[i], GPIO_INPUT_PULLUP );
   }
   delayConfig( &teclado_debounce, 50 );  // delay no bloqueante para debounce
}


/* Devuelve TRUE si hay alguna tecla presionada o FALSE (0) en caso contrario.
 * Si hay tecla presionada guarda el valor en la variable key.
 * El valor es un numero correspondiente al mapa del teclado*/
bool_t tecladoRead( uint16_t * key ){

	   bool_t retVal = FALSE;

	   uint8_t r = 0; // Rows
	   uint8_t c = 0; // Columns

	   uint16_t key_anterior = 0; // memoriza pasada anterior
	   static uint16_t key_actual = -1;   //            pasada actual

	   if ( delayRead( &teclado_debounce ) ){      //escaneo cada delay y de paso debounce
		   for( r=0; r<CANT_FILAS; r++ ){               // bajo de a una fila y leo todas las columnas
			   gpioWrite( keypadRowPins[r], LOW );
			   	   for( c=0; c<CANT_COLUMNAS; c++ ){
			   		   if( !gpioRead( keypadColPins[c] ) ){
			   			   key_anterior=key_actual;
			   			   key_actual = (uint16_t)r * (uint16_t)CANT_COLUMNAS + (uint16_t)c;
			   		   }
			   	   }
			   	gpioWrite( keypadRowPins[r], HIGH );   // suba la fila encuestada
	       }
		   if((key_anterior==key_actual)&(key_actual!=-1)){
	       *key = keypadKeys[key_actual];
	       key_actual = -1;
	       retVal = TRUE;
		   }
	   }
	   return retVal;
	}


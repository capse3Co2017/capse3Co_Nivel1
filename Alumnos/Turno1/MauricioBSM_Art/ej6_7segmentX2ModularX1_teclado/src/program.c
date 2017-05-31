/* Copyright 2016, Alejandro Permingeat.
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
 * Date: 2016-07-28
 */

#include "program.h"   // <= Own header
#include "sapi.h"      // <= sAPI library
#include "display_lib.h"

//#include <math.h>         /* <= Funciones matematicas */
extern uint8_t display7SegmentPins_[];

// Guarda la ultima tecla apretada
uint16_t key = 0;

/* Pines del teclado matricial */

// Pines conectados a las Filas --> Salidas (MODO = OUTPUT)
uint8_t keypadRowPins[4] = {
		   T_FIL0,    // Row 0
		   T_FIL1,    // R1
		   T_FIL2,    // R2
		   T_FIL3     // R3
};

// Pines conectados a las Columnas --> Entradas con pull-up (MODO = INPUT_PULLUP)
uint8_t keypadColPins[2] = {
		   T_COL0,     // C0
		   T_COL1      // C1
};

// Vector para mostrar tecla presionada en el display 7 segmentos
uint16_t keypadKeys[8] = {
                               0x0a,0x0b,
                               3,    6,
                               2,    5,
                               1,    4,
                          };
void configurarTecladoMatricial( void ){

   uint8_t i = 0;

   // Configure Rows as Outputs
   for( i=0; i<4; i++ ){
      gpioConfig( keypadRowPins[i], GPIO_OUTPUT );
   }

   // Configure Columns as Inputs with pull-up resistors enable
   for( i=0; i<2; i++ ){
      gpioConfig( keypadColPins[i], GPIO_INPUT_PULLUP );
   }
}


/* Devuelve TRUE si hay alguna tecla presionada o FALSE (0) en caso contrario.
 * Si hay tecla presionada guarda el valor en la variable key.
 * El valor es un numero de indice entre 0 y 15 */
bool_t leerTecladoMatricial( void ){

   bool_t retVal = FALSE;

   uint16_t r = 0; // Rows
   uint16_t c = 0; // Columns

   // Poner todas las filas en estado BAJO
   for( r=0; r<4; r++ ){
	  gpioWrite( keypadRowPins[r], LOW );
   }

   // Chequear todas las columnas buscando si hay alguna tecla presionada
   for( c=0; c<2; c++ ){

      // Si leo un estado BAJO en una columna entonces puede haber una tecla presionada
      if( !gpioRead( keypadColPins[c] ) ){

         delay( 50 ); // Anti-rebotes de 50 ms

         // Poner todas las filas en estado ALTO excepto la primera
         for( r=1; r<4; r++ ){
            gpioWrite( keypadRowPins[r], HIGH );
         }

         // Buscar que tecla esta presionada
         for( r=0; r<4; r++ ){

            // Poner la Fila[r-1] en estado ALTO y la Fila[r] en estado BAJO
            if( r>0 ){ // Exceptua el indice negativo en el array
               gpioWrite( keypadRowPins[r-1], HIGH );
            }
            gpioWrite( keypadRowPins[r], LOW );

            // Chequear la Columna[c] en Fila[r] para buscar si la tecla esta presionada
            // Si dicha tecla esta oresionada (en estado BAJO) entonces retorna
            // graba la tecla en key y retorna TRUE
            if( !gpioRead( keypadColPins[c] ) ){
               key = r * 2 + c;
               retVal = TRUE;
            }
         }

      }
   }
   return retVal;
}

int main(void){

   boardConfig();
   tickConfig( 1, 0 );
   
    gpioConfig(LEDB,GPIO_OUTPUT);
    gpioConfig(LCD1,GPIO_OUTPUT);
    gpioConfig(LCD2,GPIO_OUTPUT);

   display7SegmentPinConfig_();

   gpioWrite( LCD1, OFF );
   gpioWrite( LCD2, OFF );
   display7SegmentTestPins(display7SegmentPins_,LEDB);
   gpioWrite( LCD1, ON );
   gpioWrite( LCD2, ON );

   configurarTecladoMatricial();

   uint16_t numeroDisplay=0;
   uint16_t teclaActual=0;
   uint16_t teclaAnterior=0;

   while(1) {
	   if (leerTecladoMatricial()){
		   teclaAnterior=teclaActual;
		   teclaActual=keypadKeys[key];
		   if((teclaActual==0x0a)||(teclaActual==0x0b) ){
			   numeroDisplay=0;
		   }
		   else{
			   numeroDisplay=teclaAnterior*10+teclaActual;
		   }
	   }

   gpioWrite( LCD1, OFF );
   display7SegmentWrite_(numeroDisplay/10);
   delay(10);
   gpioWrite( LCD1, ON );
   gpioWrite( LCD2, OFF );
   display7SegmentWrite_(numeroDisplay%10);
   delay(10);
   gpioWrite( LCD2, ON );
   }

   return 0 ;

}

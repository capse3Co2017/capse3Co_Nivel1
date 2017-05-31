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


// Simbolos formados con los segmentos
/*
------------+------+---------
  Segmentos | HEX  | Simbolo
------------+------+---------
   hgfedcba |      |
 0b00111111 | 0x0F |   0
 0b00000110 | 0x00 |   1
 0b01011011 | 0x00 |   2
 0b01001111 | 0x00 |   3
 0b01100110 | 0x00 |   4
 0b01101101 | 0x00 |   5
 0b01111101 | 0x00 |   6
 0b00000111 | 0x00 |   7
 0b01111111 | 0x00 |   8
 0b01101111 | 0x00 |   9

 0b01011111 | 0x00 |   a
 0b01111100 | 0x00 |   b
 0b01011000 | 0x00 |   c
 0b01011110 | 0x00 |   d
 0b01111011 | 0x00 |   e
 0b01110001 | 0x00 |   F

 0b01110111 | 0x00 |   A
 0b00111001 | 0x00 |   C
 0b01111001 | 0x00 |   E
 0b01110110 | 0x00 |   H
 0b00011110 | 0x00 |   J
 0b00111000 | 0x00 |   L
 0b01110011 | 0x00 |   P
 0b00111110 | 0x00 |   U

 0b10000000 | 0x00 |   .

             a
           -----
       f /     / b
        /  g  /
        -----
    e /     / c
     /  d  /
     -----    O h = dp
*/

#include "program.h"   // <= Own header
#include "sapi.h"      // <= sAPI library

#include <math.h>         /* <= Funciones matematicas */
//#define CATODO_COMUN 1
#define ANODO_COMUN 1

// Configuración de pines del display
uint8_t display7SegmentPins_[8] = {
   GPIO5, // a
   GPIO3, // b
   GPIO2, // c
   GPIO8, // d
   GPIO7, // e
   GPIO1, // f
   GPIO4, // g
   GPIO0  // h = dp
};

uint8_t display7SegmentOutputs_[25] = {
   0b00111111, // 0
   0b00000110, // 1
   0b01011011, // 2
   0b01001111, // 3
   0b01100110, // 4
   0b01101101, // 5
   0b01111101, // 6
   0b00000111, // 7
   0b01111111, // 8
   0b01101111, // 9

   0b01011111, // a
   0b01111100, // b
   0b01011000, // c
   0b01011110, // d
   0b01111011, // e
   0b01110001, // f

   0b01110111, // A
   0b00111001, // C
   0b01111001, // E
   0b01110110, // H
   0b00011110, // J
   0b00111000, // L
   0b01110011, // P
   0b00111110, // U

   0b10000000  // .
};

void display7SegmentWrite_( uint8_t symbolIndex ){

   uint8_t i = 0;

   for(i=0;i<=7;i++)
#ifdef ANODO_COMUN
        gpioWrite( display7SegmentPins_[i], !(display7SegmentOutputs_[symbolIndex] & (1<<i)) );
#elif defined(CATODO_COMUN)
        gpioWrite( display7SegmentPins_[i], (display7SegmentOutputs_[symbolIndex] & (1<<i)) );
#endif   
}

void display7SegmentPinConfig_( void ){
   gpioConfig( display7SegmentPins_[0], GPIO_OUTPUT );
   gpioConfig( display7SegmentPins_[1], GPIO_OUTPUT );
   gpioConfig( display7SegmentPins_[2], GPIO_OUTPUT );
   gpioConfig( display7SegmentPins_[3], GPIO_OUTPUT );
   gpioConfig( display7SegmentPins_[4], GPIO_OUTPUT );
   gpioConfig( display7SegmentPins_[5], GPIO_OUTPUT );
   gpioConfig( display7SegmentPins_[6], GPIO_OUTPUT );
   gpioConfig( display7SegmentPins_[7], GPIO_OUTPUT );
}

int main(void){

   boardConfig();

   display7SegmentPinConfig_();

   uint8_t i = 0;

   while(1) {

      for( i=0; i<10; i++ ){
         display7SegmentWrite_(i); // i es el indice de los simbolos del display
         delay( 500 );
      }
   }
   return 0 ;
}

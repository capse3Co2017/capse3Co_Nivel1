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

#include "driverDisplays7Segmentos.h"   // <= su propio archivo de cabecera
#include "sapi.h"                       // <= Biblioteca sAPI

/*==================[definiciones y macros]==================================*/

/*==================[definiciones de datos internos]=========================*/

static uint8_t k=0;
static delay_t delayRefresh;
static delay_t digitChange;

uint8_t chars_[CHARS] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','A','C','E','H','J','L','P','U'};

// Configuraci?n de pines del display
uint8_t display7SegmentPins_[8] = {
GPIO4,
GPIO5,
GPIO6,
GPIO2,
GPIO1,
GPIO3,
GPIO7,
GPIO8
};

// Configuraci?n de pines del display
uint8_t display7DigitPins_[4] = {
LCD3, //D1
LCD2, //D2
LCD1, //D3
GPIO0 //D4
};

uint8_t display7SegmentOutputs_[CHARS] = {
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

   0b10000000,  // .
   0b00000000  // blank
};

/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/

/*==================[definiciones de funciones internas]=====================*/

void display7SegmentWrite_( uint8_t symbolIndex ){

   uint8_t i = 0;

   for(i=0;i<=7;i++)
   #ifdef ANODO_COMUN
        gpioWrite( display7SegmentPins_[i], !(display7SegmentOutputs_[symbolIndex] & (1<<i)) );
   #elif defined(CATODO_COMUN)
        gpioWrite( display7SegmentPins_[i], (display7SegmentOutputs_[symbolIndex] & (1<<i)) );
   #endif   
}

uint8_t char2index_( uint8_t charIndex ){

   uint8_t output = CHARS - 1;
   uint8_t i;
   
   for(i=0;i<CHARS;i++) {
      if (charIndex == chars_[i])
         output = i;
   }
   return output; 
}

void display7DigitWrite_( uint8_t digitIndex ){

   uint8_t i = 0;

   for(i=0;i<=3;i++)
   #ifdef ANODO_COMUN
        gpioWrite( display7DigitPins_[i], OFF );
   gpioWrite( display7DigitPins_[digitIndex], ON);    
   #elif defined(CATODO_COMUN)
        gpioWrite( display7DigitPins_[i], ON );
   gpioWrite( display7DigitPins_[digitIndex], OFF);
   #endif   
}

char* itoaDisplay(int value, char* result, int base) {

  result[3] = (value % 10) + '0';
  result[2] = ((value % 100) / 10) + '0';
  result[1] = ((value % 1000) / 100) + '0';
  result[0] = ((value % 10000) / 1000) + '0';
   
  return result;
}

void display7SegmentPinConfig_( void ){
   uint8_t i = 0;
   
   delayConfig(&delayRefresh, 3);
   delayConfig(&digitChange, 500);
   
   for(i=0;i<=7;i++)
       gpioConfig( display7SegmentPins_[i], GPIO_OUTPUT );
       
   for(i=0;i<=3;i++)
       gpioConfig( display7DigitPins_[i], GPIO_OUTPUT );
}

void display7SegmentWrite4Digits_(uint8_t* digit){

   if (delayRead(&delayRefresh)) {

      display7DigitWrite_(k);
      
      display7SegmentWrite_(char2index_(digit[k]));
      k++;
      if (k>4)
         k=0;
   }
}

/*==================[definiciones de funciones externas]=====================*/

/*==================[fin del archivo]========================================*/

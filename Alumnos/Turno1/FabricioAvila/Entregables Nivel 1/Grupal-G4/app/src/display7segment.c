#define CATODO_COMUN 1
#include "sapi.h"      // <= sAPI library
// Configuraci?n de pines del display
uint8_t k = 0;

//delay_t delayRefresh;
//delay_t digitChange;


uint8_t display7SegmentPins_[8] = {
   GPIO5, // a,
   GPIO3, // b, 
   GPIO2, // c, 
   GPIO8, // d
   GPIO7, // e
   GPIO1, // f, 
   GPIO4, // g, 
   GPIO0  // h = dp
   //GPIO6,  // habilitacion 1er segmento
   //LCD1   //habilitaci?n 2 segmento
};

uint8_t display7DigitPins_[4] = {
GPIO6,
LCD1,
LCD2,
LCD3
};


uint8_t display7SegmentOutputs_[27] = {
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

   0b10000000, // .
   0b01000000, // -
   0b00000000  // nada
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


void display7SegmentPinConfig_( void ){
   gpioConfig( display7SegmentPins_[0], GPIO_OUTPUT );
   gpioConfig( display7SegmentPins_[1], GPIO_OUTPUT );
   gpioConfig( display7SegmentPins_[2], GPIO_OUTPUT );
   gpioConfig( display7SegmentPins_[3], GPIO_OUTPUT );
   gpioConfig( display7SegmentPins_[4], GPIO_OUTPUT );
   gpioConfig( display7SegmentPins_[5], GPIO_OUTPUT );
   gpioConfig( display7SegmentPins_[6], GPIO_OUTPUT );
   gpioConfig( display7SegmentPins_[7], GPIO_OUTPUT );
   gpioConfig(GPIO6, GPIO_OUTPUT);   // catodo comun 1er digito
   gpioConfig(LCD1, GPIO_OUTPUT);    // catodo comun 2do digito
   gpioConfig(LCD2, GPIO_OUTPUT);    // catodo comun 3er digito
   gpioConfig(LCD3, GPIO_OUTPUT);    // catodo comun 4to digito
}

/*void display7SegmentWrite4Digits_(uint8_t i){
   if (delayRead(&delayRefresh)) {

      display7DigitWrite_(k);   // selecci?n del digito
      display7SegmentWrite_(i+k); 
      k++;
      if (k>4)
         k=0;
   }
}
*/
void Imprimir4Digit(uint8_t *datos){
    for(uint8_t j=0;j<4;j++)
            gpioWrite(display7DigitPins_[j], ON);
    gpioWrite(display7DigitPins_[k], OFF);
    display7SegmentWrite_(*(datos+k));
    k++;
    if (k>4)k=0;
}
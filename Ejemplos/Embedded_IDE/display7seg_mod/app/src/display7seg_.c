#include "display7seg_.h"   // <= Own header
#include "sapi.h"      // <= sAPI library

// Configuraci?n de pines del display
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
   uint8_t i = 0;
   for(i=0;i<=7;i++)
       gpioConfig( display7SegmentPins_[i], GPIO_OUTPUT );
}
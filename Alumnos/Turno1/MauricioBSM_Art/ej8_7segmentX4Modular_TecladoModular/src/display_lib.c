#include "display_lib.h"
#include "sapi.h"
#define CHARS 26
#define t_Refresh 5

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

// Configuracion de pines del display 7 segmentos
uint8_t display7SegmentPins_[8] = {
   GPIO5,   //a
   GPIO7,   //b
   GPIO6,   //c
   GPIO1,   //d
   GPIO2,   //e
   GPIO4,   //f
   GPIO3,   //g
   GPIO8    //dp
};

// Configuracion de pines de los digitos 7 segmentos
uint8_t display7DigitPins_[4] = {
		GPIO0, //D4-->menos significativo
		LCD1,  //
		LCD2,  //
		LCD3   //D1-->MAS SIGNIFICATIVO
};

delay_t delayRefresh;                      //declarado para refresco entre digitos

void display7SegmentPinConfig4Digits( void ){
   uint8_t i = 0;
   for(i=0;i<=7;i++)
       gpioConfig( display7SegmentPins_[i], GPIO_OUTPUT );
   for(i=0;i<=3;i++)
       gpioConfig( display7DigitPins_[i], GPIO_OUTPUT );

   delayConfig (&delayRefresh,t_Refresh);   //configurado para refresco entre digitos
}

void display7SegmentPinsTest4Digits( void ){
	uint8_t s,d;
	bool_t v;
	#ifdef ANODO_COMUN
        v=OFF;
	#elif defined(CATODO_COMUN)
        v=ON;
	#endif
	gpioWrite( display7DigitPins_[0], v );             //apago todos los digitos
	gpioWrite( display7DigitPins_[1], v );
	gpioWrite( display7DigitPins_[2], v );
	gpioWrite( display7DigitPins_[3], v );
	for(d=0;d<=3;d++){                                  //ciclo 4 digitos
		gpioWrite( display7DigitPins_[d], !v  );      		//prendo  digito
		for(s=0;s<=7;s++){                             		 	//ciclo 7 segmentos
			gpioWrite( display7SegmentPins_[s], v  );
			delay(200);
			gpioWrite( display7SegmentPins_[s], !v );
		}                                               		//FIN  ciclo 7 segmentos
		gpioWrite( display7DigitPins_[d], v );          	//apago  digito
	}                                                   //FIN ciclo 4 digitos
}

uint8_t chars_[CHARS] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','A','C','E','H','J','L','P','U'};
static uint8_t display7SegmentOutputs_[26] = {
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

   0b01110111, // A 16
   0b00111001, // C 17
   0b01111001, // E 18
   0b01110110, // H 19
   0b00011110, // J 20
   0b00111000, // L 21
   0b01110011, // P 22
   0b00111110, // U 23

   0b10000000,  // . 24
   0b00000000  // apagado 25
};

uint8_t char2index( uint8_t charIndex ){

   uint8_t output = CHARS - 1;
   uint8_t i;

   for(i=0;i<CHARS;i++) {
      if (charIndex == chars_[i])
         output = i;
   }
   return output;
}

void display7SegmentWrite4Digits(uint8_t* digit){
   static uint8_t d=0;
   uint8_t s;
   uint8_t symbolIndex;

	bool_t v;
	#ifdef ANODO_COMUN
       v=OFF;
	#elif defined(CATODO_COMUN)
       v=ON;
	#endif

    	   if (delayRead(&delayRefresh)){                   //ciclo 4 digitos FreeWheeling
    		   if ((d-1)<0)
    				   gpioWrite( display7DigitPins_[3], v ); //apago  digito anterior
    		   else
    			   gpioWrite( display7DigitPins_[d-1], v );

    		   gpioWrite( display7DigitPins_[d], !v  );      		//prendo  digito
    		   symbolIndex = char2index(digit[3-d]);                  //decodifica ascii-->indice7segmento
    		   for(s=0;s<=7;s++){                             		 	//ciclo 7 segmentos
					if (v)
    			   	   gpioWrite( display7SegmentPins_[s], (display7SegmentOutputs_[symbolIndex] & (1<<s)) );
					else
    			   	   gpioWrite( display7SegmentPins_[s], !(display7SegmentOutputs_[symbolIndex] & (1<<s)) );

    		   }                                               		    //FIN  ciclo 7 segmentos
    	       d++;
    	       if (d>3)
    	         d=0;                                         //FIN ciclo 4 digitos
    	   }
}




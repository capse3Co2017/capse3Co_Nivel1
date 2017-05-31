#include "program.h"   // <= Own header
#include "sapi.h"      // <= sAPI library
#include "display7segment.h"
#include "keypad4x4.h"
#include "sapi_circularBuffer.h"
#include <math.h>         /* <= Funciones matematicas */

# define MY_BUFF_SIZE   11 // Recordar ingresar tama?o requerido + 1, porque deja un lugar en la cola
# define UART_DEBUG UART_USB

typedef enum{C_ESPERA, C_DIG} MODO_CONF;

MODO_CONF ESTADO_CONF;

delay_t digitChange;
delay_t delayRefresh;
delay_t blinkTime;

uint8_t var_piso;

uint8_t display4S[5]={26,26,26,26};
uint8_t cF[5]={12,15,26,26};

uint16_t tecl_pres = 0;
uint16_t keypadKeys[16] = {
                               1,    2,    3, 0x0a,
                               4,    5,    6, 0x0b,
                               7,    8,    9, 0x0c,
                            0x0e,    0, 0x0f, 0x0d
                          };     
       


void emptyBuff( void ){
   uartWriteString( UART_DEBUG, "Buffer vacio.\r\n" );
}

void fullBuff( void ){
   uartWriteString( UART_DEBUG, "Buffer lleno.\r\n" );
}

void DESPLEGARcF(void){
    for(uint8_t i=0;i<5;i++)
                      display4S[i]=cF[i];

}

void INI_MEFconf(void){
    ESTADO_CONF = C_ESPERA;
}




int main(void){

   
   boardConfig();
   display7SegmentPinConfig_();
   delayConfig(&delayRefresh, 3);
   delayConfig(&digitChange, 500);
   delayConfig(&blinkTime, 1000); 
   configurarTecladoMatricial(); // Configurar teclado matricial
   
   uint8_t myBuffMemory[ MY_BUFF_SIZE ];
   circularBuffer_t myBuff;

   circularBufferConfig(
            &(myBuff),
            myBuffMemory,
            MY_BUFF_SIZE,
            emptyBuff, fullBuff
         );
    
   uartConfig(UART_DEBUG, 115200);
   
   uint8_t EstadoConf = 0;
   static uint8_t txData = '1';
   static uint8_t rxData;


   
  

   while(1) {
      
      
       if(delayRead(&delayRefresh))
            Imprimir4Digit(&display4S[0]);
   
  
       tecl_pres =leerTecladoMatricial();
       if(tecl_pres!=88){
                                     
            display4S[2]=display4S[3];
            
            display4S[3] = keypadKeys[tecl_pres];
       }
        
       

	switch(ESTADO_CONF){
		case C_ESPERA:
				if((display4S[3]==0x0a) && (display4S[2]==9) && (display4S[1]==9)){
							ESTADO_CONF = C_DIG;
							DESPLEGARcF();
								
				}
		break;
		case C_DIG:
				if(display4S[3]==0x0a){
					var_piso = 10*display4S[1]+display4S[2];
					circularBufferWrite( &myBuff, &var_piso);
					DESPLEGARcF();
					ESTADO_CONF = C_DIG;
				}
				if(display4S[3]==0x0b){
					DESPLEGARcF();
					if(display4S[3]==0x0b){
					ESTADO_CONF = C_ESPERA;
					
				          }
								
								
				}
								
			
		break;
			
			
		default:
				INI_MEFconf();
		
	}
	
       if( delayRead(&blinkTime) ){
       ActualizarMEF();
       }

        
  
   }
   return 0 ;
}

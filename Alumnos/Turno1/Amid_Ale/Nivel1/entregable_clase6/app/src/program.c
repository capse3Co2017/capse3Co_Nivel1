#include "program.h"   // <= Own header
#include "sapi.h"      // <= sAPI library

// MAIN FUNCTION, ENTRY PINT TO THE PROGRAM AFTER POR OR RESET.
int main( void )  {

   // ---------- SETUP ----------------------------------------
   // Initialize an configurate the board
   boardConfig();   
   // Inicializando UART
   uartConfig(UART_USB,115200);
   //variable de control UART
   bool_t nuevodato;
   uint8_t dato;
   
   //Variables de estado de pulsadores y leds
   uint8_t statusleds = 0;
   //Variables de control de estado de leds
   uint8_t control;
   

  
   
 
   
    
   while(true)  {
     delay(1000);
     
     if((statusleds) & 1) {
       uartWriteString(UART_USB,"|LEDB: ON|  ");
     }
     else{
         uartWriteString(UART_USB,"|LEDB: OFF| ");
     }

     if((statusleds >> 1 & 1)){uartWriteString(UART_USB,"|LEDG: ON| ");}
       else{uartWriteString(UART_USB,"|LEDG: OFF|  ");}

     if((statusleds >> 2 & 1)){uartWriteString(UART_USB,"|LEDR: ON| ");}
       else{uartWriteString(UART_USB,"|LEDR: OFF| ");}

     if((statusleds >> 3 & 1)){uartWriteString(UART_USB,"|LED1: ON| ");}
       else{uartWriteString(UART_USB,"|LED1: OFF| ");}

     if((statusleds >> 4 & 1)){uartWriteString(UART_USB,"|LED2: ON| ");}
       else{uartWriteString(UART_USB,"|LED2: OFF| ");}

     if((statusleds >> 5 & 1)){uartWriteString(UART_USB,"|LED3: ON| ");}
       else{uartWriteString(UART_USB,"|LED3: OFF| ");}
     uartWriteString(UART_USB,"\r\n");  
     if(!gpioRead(TEC1)){uartWriteString(UART_USB,"|TEC1: ON| ");}
       else{uartWriteString(UART_USB,"|TEC1: OFF| ");}		 

     if(!gpioRead(TEC2)){uartWriteString(UART_USB,"|TEC2: ON| ");}
       else{uartWriteString(UART_USB,"|TEC2: OFF| ");}		 

     if(!gpioRead(TEC3)){uartWriteString(UART_USB,"|TEC3: ON| ");}
       else{uartWriteString(UART_USB,"|TEC3: OFF| ");}		 

     if(!gpioRead(TEC4)){uartWriteString(UART_USB,"|TEC4: ON| ");}
       else{uartWriteString(UART_USB,"|TEC4: OFF| ");}
      uartWriteString(UART_USB,"\r\n");  
      		 
     
     nuevodato = uartReadByte(UART_USB, &dato);  
   
  
     if(nuevodato)  { 
       control = dato; 
       switch(control)  {
         case 'a':
            statusleds ^= 1;
            gpioWrite( LEDB, (statusleds) & 1);
            break;
         case 'v': 
            statusleds ^= 1 << 1;
            gpioWrite( LEDG, (statusleds >> 1) & 1);  
 	  break;
         case 'r':
            statusleds ^= 1 << 2;
            gpioWrite( LEDR, (statusleds >> 2) & 1);     	
	  break;
         case '1':   
            statusleds ^= 1 << 3;
            gpioWrite( LED1, (statusleds >> 3) & 1);     	     	
	  break;
         case '2':
            statusleds ^= 1 << 4;
            gpioWrite( LED2, (statusleds >> 4) & 1);     	        	
	  break;
         case '3':      
            statusleds ^= 1 << 5;
            gpioWrite( LED3, (statusleds >> 5) & 1);     	  	
	  break;
     }
     }

    
   }
   return 0;
   
   }

   
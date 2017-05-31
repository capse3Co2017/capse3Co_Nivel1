/*============================================================================
 * License: 
 * Author: 
 * Date: 
 *===========================================================================*/

/*==================[inclusions]=============================================*/

#include "program.h"   // <= Own header
#include "sapi.h"      // <= sAPI library

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/
void cambiarEstadoLED(uint8_t puerto){

gpioWrite( puerto, !(gpioRead(puerto)) );

}


void initHardWare()
{
   boardConfig();                  // Inicializar la placa
   uartConfig( UART_USB, 115200 ); // Inicializar la UART2 a 115200 baudios
}

void leerEstadosLed(uint8_t *estados)
{
   uint8_t iter = LEDR;
   for (iter = LEDR;iter++;iter <=LED3)
   {
 	if(gpioRead(iter))
		estados[iter-LEDR] = iter;
	else
		estados[iter-LEDR] = '0';
   }
}
/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[main function]==========================================*/

// MAIN FUNCTION, ENTRY PINT TO THE PROGRAM AFTER POR OR RESET.
int main( void ){
   initHardWare();
   uartWriteString( UART_USB, "Prender y apagar leds.\r\n Valores admitidos: a, v, r, 1, 2, 3.\r\n" );
   uint8_t receivedByte;
   uint8_t puerto;
   uint8_t k;
   uint8_t estados[] = {'0','0','0','0','0','0','0'};
   while(1) 
   {
   	puerto = 0;
    	if( uartReadByte( UART_USB, &receivedByte ) )
    	{
    	    switch( receivedByte )
    		{
    		   case 0x61:
    		         puerto = LEDB;
    		         uartWriteString( UART_USB, "Se apreto una a.\r\n" );
            	         break;
            	   case 0x76:
               	         uartWriteString( UART_USB, "Se apreto una v.\r\n" );
               	         puerto = LEDG;
               	         break;
	         	   case 0x72:
               	         uartWriteString( UART_USB, "Se apreto una r.\r\n" );
               	         puerto = LEDR;
               	         break;
	         	   case 0x31:
               	         uartWriteString( UART_USB, "Se apreto un 1.\r\n" );
               	         puerto = LED1;
               	         break;
	         	   case 0x32:
               	         uartWriteString( UART_USB, "Se apreto un 2.\r\n" );
               	         puerto = LED2;
               	         break;
	         	   case 0x33:
               	         uartWriteString( UART_USB, "Se apreto un 3.\r\n" );
               	         puerto = LED3;
               	         break;
               	 }
              cambiarEstadoLED(puerto);
          }
          uartWriteString( UART_USB,"Estado Teclas: ");
	for (k = TEC1;k<=TEC4;k++)
	{
	   if(!gpioRead(k))
      	   {
      	      switch( k -TEC1)
      	      {
    		case 0:
    		     receivedByte = '1';
            	     break;
            	case 1:
                         receivedByte = '2';
                         break;
	          case 2:
               	     receivedByte = '3';
               	     break;
	         	case 3:
               	     receivedByte = '4';
               	     break;
               	 }
               	 estados[k-TEC1] = receivedByte;
      	     }
	     else
      	   	estados[k-TEC1] = '0';
      	   uartWriteByte( UART_USB, estados[k-TEC1]);
      	   uartWriteString( UART_USB,":");
	}
	uartWriteString( UART_USB,"\r\n");
          uartWriteString( UART_USB,"Estado LEDS: ");

      	for (k = LEDR;k<=LED3;k++)
      	{ 
      	     if(gpioRead(k))
      	     {
      	     	switch( k -LEDR)
      	     	{
    		   case 0:
    		         receivedByte = 'r';
            	         break;
            	   case 1:
               	         receivedByte = 'v';
               	         break;
	         	   case 2:
               	         receivedByte = 'a';
               	         break;
	         	   case 3:
               	         receivedByte = '1';
               	         break;
	         	   case 4:
	         	         receivedByte = '2';
               	         break;
	         	   case 5:
               	         receivedByte = '3';
               	         break;
               	 }
               	 estados[k-LEDR] = receivedByte;
      	     }
      	   	
	     else
      	   	estados[k-LEDR] = '0';
	     uartWriteByte( UART_USB, estados[k-LEDR]);
	     uartWriteString( UART_USB,":");
       	}
	uartWriteString( UART_USB,"\r\n");
      	delay(1000);
      		

   }
   return 0;
}

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/*==================[end of file]============================================*/

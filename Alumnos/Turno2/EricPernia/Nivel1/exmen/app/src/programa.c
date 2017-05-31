/*==================[inlcusiones]=========================*/

#include "programa.h"   // <= su propio archivo de cabecera
#include "sapi.h"       // <= Biblioteca sAPI

int main( void ){

   // ---------- CONFIGURACIONES ------------------------------
   // Inicializar y configurar la plataforma

   boardConfig();
        
   uartConfig( UART_USB, 115200 ); // Inicializar la UART2 a 115200 baudios  
   
   // --------variables del delay---------------
   int led_parpadea;
   delay_t delay_parpadeo;
    delayConfig( &delay_parpadeo,0);

   //------------------variables para el estado de las teclas---------
	int tecla_1; 
	int tecla_2;
	int tecla_3;
	int tecla_4;
	//--------------------variables para el estado de los leds-------------
	int led_B;
	int led_1;
	int led_2;
	int led_3;	
	//--------apago todos los leds para asegurarme que comiencen apagados-----
	gpioWrite( LEDB, OFF );
	gpioWrite( LED1, OFF );
	gpioWrite( LED2, OFF );
	gpioWrite( LED3, OFF );

	//----------------inicio el estado de los leds en cero--------------
	led_B = 0;
	led_1 = 0;
	led_2 = 0;
	led_3 = 0;
	
	uint8_t byteRecibido; // variable donde recibo caracteres de la compu
          
   // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE )
   {   
          
   //-------------Lectura de los estados de las teclas------------
	tecla_1 = !gpioRead(TEC1);
	tecla_2 = !gpioRead(TEC2);
	tecla_3 = !gpioRead(TEC3);
	tecla_4 = !gpioRead(TEC4);
	
		
	//--------Lectura de la UART_USB para saber que led parpadea--------
	if(uartReadByte(UART_USB, & byteRecibido)  ){
	switch(byteRecibido){
	case 'a':
	led_parpadea=led_B;
	break;
		
	case '1':
	led_parpadea=led_1;
	break;
	
	case '2':
	led_parpadea=led_2;
	break;
	
	case '3':
	led_parpadea=led_3;
	break;
	}
	
	}
	//-----------Defino el tiempo de parpadeo segun la tecla de la CIA-----
	
	if(tecla_1==1){
	delayConfig( &delay_parpadeo,50);// el led estara parpadeando cada 50 milisegundos
	}
		
	
	if(tecla_2==1){
	delayConfig( &delay_parpadeo,100);// el led estara parpadeando cada 100 milisegundos
	}
		
	
	if(tecla_3==1){
	delayConfig( &delay_parpadeo,200);// el led estara parpadeando cada 200 milisegundos
	}
		
	if(tecla_4==1){
	delayConfig( &delay_parpadeo,500);// el led estara parpadeando cada 500 milisegundos
	}
	
	//--------------hacemos el parpadeo------------------------
    
	if(delayRead(&delay_parpadeo) ) {
		gpioToggle(led_parpadea);
	}

   } 

   return 0;
}
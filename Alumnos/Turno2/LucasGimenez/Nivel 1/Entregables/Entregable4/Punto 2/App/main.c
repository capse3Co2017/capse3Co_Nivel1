/*
 * =============================================================================
 * Autor: Lucas Gimenez
 * ============================================================================
 * =============================================================================
 * Fecha de creacion: 2017-03-24
 * =============================================================================
 */

/*==================[inlcusiones]============================================*/

#include "programa.h"   // <= su propio archivo de cabecera
#include "sapi.h"       // <= Biblioteca sAPI

/*==================[definiciones y macros]==================================*/

/*==================[definiciones de datos internos]=========================*/

/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main (void){

	// ---------- CONFIGURACIONES ------------------------------
   	// Inicializar y configurar la plataforma (leds, teclas y 
   	// funciones de manejo de tiempos)
   	
	boardConfig();
	
	int tiempo 	    = 0;        //Variable para controlar el tiempo de encendido/apagado de los leds
	int secuencia 	= 0;  	  //Variable para conocer el sentido de la secuencia de los leds
	int baliza	 	= 0;        //Variable para seleccionar entre LEDR o LEDB del LED-RGB en el modo baliza
	int modo 		= 0;	  //Variable para conocer el modo de funcionamiento (modo = 0 baliza; modo = 1 secuencia rotatoria)
	int leds 		= 1;	  //Variable para encender/apagar los leds de la placa en el modo = 1

	//Configurar el periferico Timer para PWM
   	pwmConfig( 0, PWM_ENABLE );
   	
   
   	//Configurar el periferico Conversor Analogico-Digital
   	adcConfig( ADC_ENABLE );

while(1){
	
//Le asignamos el valor del ADC (CH1, donde se encuentra conectado el potenciometro) a nuestra  variable 'tiempo'
	tiempo = adcRead (CH1) * 1000;
	
//Si TEC1 es presionada la variable 'modo' es igual a 0 (modo baliza)
	if (!(gpioRead( (TEC1) )))
	{
		modo = 0; }

//Si TEC2 es presionada la variable 'secuencia' es igual a 0 (de izquierda a derecha) y la variable 'baliza' es LEDB para el modo baliza
	if (!(gpioRead( (TEC2) ))) 
	{
		secuencia = 0;
		baliza = LEDB; }
		
//Si TEC3 es presionada la variable 'secuencia' es igual a 1 (de derecha a izquierda) y la variable 'baliza' es LEDR para el modo baliza
	if (!(gpioRead( (TEC3) )))
	{
		secuencia = 1;
		baliza = LEDR; }

//Si TEC4 es presionada la variable 'modo' es igual a 1 (modo secuencia rotatoria)		
	if (!(gpioRead( (TEC4) )))
	{
		modo = 1; }
			
//Si estamos en el modo '0', se encienden/apagan los leds en modo baliza. El tiempo es controlado por el potenciomentro (variable 'tiempo')
	if (modo == 0)
	{
		gpioWrite( (LED1), (1) );
		gpioWrite( (LED2), (1) );
		gpioWrite( (LED3), (1) );
		gpioWrite( (baliza), (1) );
		delay( (tiempo) / 1000.0 );
		gpioWrite( (LED1), (0) );
		gpioWrite( (LED2), (0) );
		gpioWrite( (LED3), (0) );
		gpioWrite( (baliza), (0) );
		delay( (tiempo) / 1000.0 );
	}
	
//Si estamos en el modo '1', se activa el modo de secuencia rotatoria 
	if (modo == 1)
	{

//Si el sentido de la secuencia es igual a '0', vamos encendiendo los leds con un delay fijado por la variable 'tiempo'
		if (secuencia == 0)
		{
			leds++;	  
		
//Antes de encender los leds, verificamos que la variable 'leds' no desborde (mayor a LED3, limite superior de la placa).
			if (leds >  LED3) {
			
				leds = LEDB; }
		
			gpioWrite( (leds), (1) );
		
			delay( (tiempo) / 1000.0 );
		
			gpioWrite( (leds), (0) );
		}

//Si el sentido de la secuencia es igual a '1'(secuencia invertida), vamos encendiendo los leds con un delay fijado por la variable 'tiempo'

		if (secuencia == 1) 
		{
			leds--;

//Antes de encender los leds, verificamos que la variable 'leds' no desborde (menor a LEDB, limite inferior de la placa).
			if (leds < LEDB) {
		
			 	leds = LED3; }
			
			gpioWrite( (leds), (1) );
		
			delay( (tiempo) / 1000.0 );
	
			gpioWrite( (leds), (0) );
		
		
           	}
           		 
	  }
      }
	
   return 0;
}

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

/*==================[fin del archivo]========================================*/

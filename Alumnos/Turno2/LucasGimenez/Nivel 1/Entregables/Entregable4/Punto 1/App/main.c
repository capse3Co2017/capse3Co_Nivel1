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
	
	int tiempo = 0;        //Variable para controlar el tiempo de encendido/apagado de los leds.
	int interrupcion = 0;  //Variable para conocer el sentido de la secuencia de los leds.
	int leds = 1;          //Variable para seleccionar los leds de la EDU-CIAA.
	

	//Configurar el periferico Timer para PWM.
   	pwmConfig( 0, PWM_ENABLE );
   	
   
   	//Configurar el periferico Conversor Analogico-Digital.
   	adcConfig( ADC_ENABLE );

while(1){
	
//Le asignamos el valor del ADC (CH1, donde se encuentra conectado el potenciometro) a nuestra  variable 'tiempo'.
	tiempo = adcRead (CH1) * 1000;

//Leemos el estado de TEC2 (pin7). Si está presionado, el sentido es igual a '0' (de izquierda a derecha).
	if (!(gpioRead( (TEC2) ))) 
	{
	
		interrupcion = 0; }
		
//Leemos el estado de TEC3 (pin8). Si está presionado, el sentido es igual a '1' (de derecha a izquierda).
	if (!(gpioRead( (TEC3) )))
	{
	
		interrupcion = 1; }
			
//Si el sentido de la secuencia es igual a '0', vamos encendiendo los leds con un delay fijado por la variale 'tiempo'.
//La variable 'tiempo' tiene la lectura del valor del potenciometro.
	if (interrupcion == 0)
	{
		leds++;	  
		
//Antes de encender los leds, verificamos que la variable 'leds' no desborde (mayor a LED3, limite superior de la placa).
		if (leds >  LED3) {
			
			leds = LEDB; }
		
		gpioWrite( (leds), (1) );
		
		delay( (tiempo) / 1000.0 );
		
		gpioWrite( (leds), (0) );
		
		
	}

//Si el sentido de la secuencia es igual a '1' (secuencia invertida), vamos encendiendo los leds con un delay fijado por
//la variable 'tiempo'. La variable 'tiempo' contiene la lectura del valor del potenciometro.
	if (interrupcion == 1) 
	{
		leds--;

//Antes de encender los leds, verificamos que la variable 'leds' no desborde (menor a LEDB, limite inferior de la placa).
		if (leds < LEDB  ) {
		
			 leds = LED3; }
			
		gpioWrite( (leds), (1) );
		
		delay( (tiempo) / 1000.0 );
		
		gpioWrite( (leds), (0) );
		
		
            }
         }
	
   return 0;
}

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

/*==================[fin del archivo]========================================*/

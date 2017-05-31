/* Copyright (c) 2017, Eric Pernia.
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
 * =============================================================================
 *
 * Copyright (c) 2017, Eric Pernia.
 * Todos los derechos reservados.
 * 
 * La redistribución y el uso en las formas de código fuente y binario, con o 
 * sin modificaciones, están permitidos siempre que se cumplan las siguientes 
 * condiciones:
 * 
 * 1. Las redistribuciones del código fuente deben conservar el aviso de
 *    copyright anterior, esta lista de condiciones y el siguiente descargo de
 *    responsabilidad.
 * 
 * 2. Las redistribuciones en formato binario deben reproducir el aviso de
 *    copyright anterior, esta lista de condiciones y el siguiente descargo de
 *    responsabilidad en la documentación y/u otros materiales suministrados 
 *    con la distribución.
 * 
 * 3. Ni el nombre de los titulares de derechos de autor ni los nombres de sus
 *    colaboradores pueden usarse para apoyar o promocionar productos 
 *    derivados de este software sin permiso específico previo y por escrito.
 * 
 * ESTE SOFTWARE SE SUMINISTRA POR LOS PROPIETARIOS DEL COPYRIGHT Y
 * COLABORADORES ''COMO ESTÁ'' Y CUALQUIER GARANTÍAS EXPRESA O IMPLÍCITA,
 * INCLUYENDO, PERO NO LIMITADO A, LAS GARANTÍAS IMPLÍCITAS DE COMERCIALIZACIÓN
 * Y APTITUD PARA UN PROPÓSITO PARTICULAR SON RECHAZADAS. EN NINGÚN CASO LOS
 * PROPIETARIOS DEL COPYRIGHT Y COLABORADORES SERÁN RESPONSABLES POR NINGÚN
 * DAÑO DIRECTO, INDIRECTO, INCIDENTAL, ESPECIAL, EJEMPLAR O COSECUENCIAL
 * (INCLUYENDO, PERO NO LIMITADO A, LA ADQUISICIÓN O SUSTITUCIÓN DE BIENES O
 * SERVICIOS; LA PÉRDIDA DE USO, DE DATOS O DE BENEFICIOS; O INTERRUPCIÓN DE LA
 * ACTIVIDAD EMPRESARIAL) O POR CUALQUIER TEORÍA DE RESPONSABILIDAD, YA SEA POR
 * CONTRATO, RESPONSABILIDAD ESTRICTA O AGRAVIO (INCLUYENDO NEGLIGENCIA O
 * CUALQUIER OTRA CAUSA) QUE SURJA DE CUALQUIER MANERA DEL USO DE ESTE SOFTWARE,
 * INCLUSO SI SE HA ADVERTIDO DE LA POSIBILIDAD DE TALES DAÑOS.
 *
 * =============================================================================
 * Fecha de creacion: 2017-03-18
 * =============================================================================
 */


/*
   NOTA: 

   A diferencia de snap4Arduino que los pines de leds estan en:

      LEDR - Pin 0
      LEDG - Pin 1
      LEDB - Pin 2
      LED1 - Pin 3
      LED2 - Pin 4
      LED3 - Pin 5

   En C tenemos los pines en este orden:

      LED1
      LED2
      LED3
      LEDR
      LEDG
      LEDB

   Por eso se usa para este programa los Leds 1 a 3 y el R ya que son nuúmeros
   de pines consecutivos. Entonces en este caso al sumar 1 a LED3 se obtiene el 
   numero de indice de LEDR.

   Los numeros concretos se pueden ver en la biblioteca sAPI en el archivo 
   "sapi_peripheral_map.h".
*/


/*==================[bibliotecas]============================================*/


#include "sapi.h"            /* <= sAPI header */

/*==================[macros]=================================================*/

#define DERECHA   1
#define IZQUIERDA 0
#define BALIZA	0
#define ROTATIVO 1



uint8_t iteradorB = LED1;    // Guarda el indice del LED a encender
static uint8_t queledrbB = LEDR;
static uint8_t sentidoB  = DERECHA; // Guarda el sentido de giro actual
static uint16_t velocidadW = 150;
static uint8_t modo = 0;






//*************************************************************************************************
//			Interrupcion cada 10ms
//*************************************************************************************************
bool_t IntTimer( void *ptr )
{

if (!(gpioRead(TEC1)))
	modo = BALIZA;

if (!(gpioRead(TEC2)))
	{
	sentidoB = IZQUIERDA;
	queledrbB = LEDR;
	}
if (!(gpioRead(TEC3)))
	{
	sentidoB = DERECHA;
	queledrbB = LEDB;
	}

if (!(gpioRead(TEC4)))
	modo = ROTATIVO;

velocidadW = (adcRead(CH1) + 150);

return 1;
}
//*************************************************************************************************
//*************************************************************************************************





/*==================[programa principal]=====================================*/
int main( void )
{
// ---------- CONFIGURACIONES ------------------------------
// Inicializar y configurar la plataforma
boardConfig();  

//Interupcion cada 10ms
tickConfig( 10, IntTimer );


// Inicializar todos los pines apagados
gpioWrite(LED1, 0);
gpioWrite(LED2, 0 );
gpioWrite(LED3, 0);
gpioWrite(LEDR, 0);
gpioWrite(LEDG, 0);
gpioWrite(LEDB, 0);


//Inicializa AD.
adcConfig( ADC_ENABLE );




// ---------- REPETIR POR SIEMPRE --------------------------
while(1)
{

if (modo)
	{
	//Se actualiza "interador" segun "sentido".
	if (sentidoB == DERECHA)
		iteradorB = iteradorB + 1;
	else	iteradorB = iteradorB - 1;


	//Se verifican limites.
	if (iteradorB < LED1)
		 iteradorB = LEDR;
	if (iteradorB > LEDR)
		iteradorB = LED1;

	gpioWrite(iteradorB, 1);
	delay(velocidadW);
	gpioWrite(iteradorB, 0);
	}
else	{
	gpioWrite( LED1, 1);
	gpioWrite( LED2, 1);
	gpioWrite( LED3, 1);
	if (queledrbB == LEDR)
		gpioWrite( LEDR, 1);
	if (queledrbB == LEDB)
		gpioWrite( LEDB, 1);
	delay(velocidadW);
	gpioWrite( LED1, 0);
	gpioWrite( LED2, 0);
	gpioWrite( LED3, 0);
	gpioWrite( LEDR, 0);
	gpioWrite( LEDB, 0);
	delay(velocidadW);
	}

}
return 0 ;
}

/*==================[fin del archivo]========================================*/

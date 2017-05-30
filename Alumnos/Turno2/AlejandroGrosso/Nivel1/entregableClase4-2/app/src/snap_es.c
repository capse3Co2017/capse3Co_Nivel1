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

#include "snap_es.h"

/*==================[macros]=================================================*/

#define DERECHA   	1
#define IZQUIERDA 	3
#define BALIZA    	1
#define GIRO	0

/*==================[programa principal]=====================================*/
void apagarleds(int LED[4])
{
  crearVariable myForIterator=0;
  
	repetir(4)
	 {
	   fijarPinDigitalEn(LED[myForIterator],APAGAR);
	 }

}

programa()
{
   // ---------- CONFIGURACIONES ------------------------------

   // Inicializar y configurar la plataforma
   configurarPlataforma();  
   configurarConversorAD( ADC_ENABLE );

   // Crear 2 variables e inicializarlas
   crearVariable iterador = 0;    // Guarda el indice del LED a encender
   crearVariable sentido  = DERECHA; // Guarda el sentido de giro actual
   crearVariable valor = 0;          // Guarda el valor leido del ADC
   crearVariable modo = GIRO;
   crearVariable titilar=ENCENDER;
   
   crearVariable LED[4]={LED1,LED2,LED3,LEDB};
   // Inicializar todos los pines apagados
   fijarPinDigitalEn( LED1, APAGAR );
   fijarPinDigitalEn( LED2, APAGAR );
   fijarPinDigitalEn( LED3, APAGAR );
   fijarPinDigitalEn( LEDR, APAGAR );
   
   fijarPinDigitalEn( LEDG, APAGAR );
   fijarPinDigitalEn( LEDB, APAGAR );

   // ---------- REPETIR POR SIEMPRE --------------------------

   porSiempre()
   {
      // Si el sentido de giro es a la derecha incrementa la variable iterador   

      // Si se esta presionada la TEC1, entonces cambia el modo Baliza.
      si( no(lecturaDigital(TEC1)) ){
         modo = BALIZA;
      }
      // Si se esta presionada la TEC4, entonces cambia al modo giro
      // la derecha
      si( no(lecturaDigital(TEC4)) ){
         apagarleds(LED);
         modo = GIRO;
      }
      si( modo==GIRO){
      	LED[3]=LEDB;  //cambia a led azul cuando en modo Baliza quedo el led rojo
      	iterador=(iterador+sentido)&0x3; //x+(n-1) mod n es lo mismo que restar 1
      	si( no(lecturaDigital(TEC2)) ){
         		sentido = IZQUIERDA;
      	}
      	// Si se esta presionada la TEC3, entonces cambia el sentido de giro a
      	// la derecha
      	si( no(lecturaDigital(TEC3)) ){
      	   sentido = DERECHA;
      	}
      	fijarPinDigitalEn( LED[iterador] , ENCENDER );
      	valor=((lecturaAnalogica( CH2 )& 0x3FF)>>6)+1;
      	esperarSegs( ((double)valor)/10.0 );
      	fijarPinDigitalEn(LED[iterador],APAGAR);
      }siNo{
      	// Si se esta presionada la TEC2, entonces usa led roja
      	si( no(lecturaDigital(TEC2)) ){
      	   fijarPinDigitalEn(LED[3], APAGAR);
	   LED[3] = LEDR;
      	}
      	// Si se esta presionada la TEC3, entonces usa led azul
      	si( no(lecturaDigital(TEC3)) ){
      	   fijarPinDigitalEn(LED[3], APAGAR);
      	   LED[3]=LEDB;
      	}
      	{crearVariable i=0;
      	  repetir(4)
      	  {
      		fijarPinDigitalEn(LED[i++],titilar);
      	  }
      	}
      	titilar=1-titilar;
          valor=((lecturaAnalogica( CH2 )& 0x3FF)>>6)+1;
      	esperarSegs(((double)valor)/10.0);
/*     	{crearVariable i=0;
      	  repetir(4)
      	  {
      		fijarPinDigitalEn(LED[i++],APAGAR);
      	  }
      	}
      	esperarSegs(((double)valor)/10.0);
*/
      }     
   }   
}

/*==================[fin del archivo]========================================*/

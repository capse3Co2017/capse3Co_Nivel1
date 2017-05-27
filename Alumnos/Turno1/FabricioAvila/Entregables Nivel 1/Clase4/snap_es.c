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
 * La redistribuci?n y el uso en las formas de c?digo fuente y binario, con o 
 * sin modificaciones, est?n permitidos siempre que se cumplan las siguientes 
 * condiciones:
 * 
 * 1. Las redistribuciones del c?digo fuente deben conservar el aviso de
 *    copyright anterior, esta lista de condiciones y el siguiente descargo de
 *    responsabilidad.
 * 
 * 2. Las redistribuciones en formato binario deben reproducir el aviso de
 *    copyright anterior, esta lista de condiciones y el siguiente descargo de
 *    responsabilidad en la documentaci?n y/u otros materiales suministrados 
 *    con la distribuci?n.
 * 
 * 3. Ni el nombre de los titulares de derechos de autor ni los nombres de sus
 *    colaboradores pueden usarse para apoyar o promocionar productos 
 *    derivados de este software sin permiso espec?fico previo y por escrito.
 * 
 * ESTE SOFTWARE SE SUMINISTRA POR LOS PROPIETARIOS DEL COPYRIGHT Y
 * COLABORADORES ''COMO EST?'' Y CUALQUIER GARANT?AS EXPRESA O IMPL?CITA,
 * INCLUYENDO, PERO NO LIMITADO A, LAS GARANT?AS IMPL?CITAS DE COMERCIALIZACI?N
 * Y APTITUD PARA UN PROP?SITO PARTICULAR SON RECHAZADAS. EN NING?N CASO LOS
 * PROPIETARIOS DEL COPYRIGHT Y COLABORADORES SER?N RESPONSABLES POR NING?N
 * DA?O DIRECTO, INDIRECTO, INCIDENTAL, ESPECIAL, EJEMPLAR O COSECUENCIAL
 * (INCLUYENDO, PERO NO LIMITADO A, LA ADQUISICI?N O SUSTITUCI?N DE BIENES O
 * SERVICIOS; LA P?RDIDA DE USO, DE DATOS O DE BENEFICIOS; O INTERRUPCI?N DE LA
 * ACTIVIDAD EMPRESARIAL) O POR CUALQUIER TEOR?A DE RESPONSABILIDAD, YA SEA POR
 * CONTRATO, RESPONSABILIDAD ESTRICTA O AGRAVIO (INCLUYENDO NEGLIGENCIA O
 * CUALQUIER OTRA CAUSA) QUE SURJA DE CUALQUIER MANERA DEL USO DE ESTE SOFTWARE,
 * INCLUSO SI SE HA ADVERTIDO DE LA POSIBILIDAD DE TALES DA?OS.
 *
 * =============================================================================
 * Fecha de creacion: 2017-03-04
 * =============================================================================
 */

/* EJERCICIO ENTREGABLE CLASE 4 */
/* NOMBRE : FABRICIO AVILA      */

/*==================[bibliotecas]============================================*/

#include "snap_es.h"
void Estado0 (void);
void Estado1 (void);
void Estado2 (void);
void Estado3 (void);
void VerificaTec(void);
/*================[variables globales]======================================*/
crearVariable Estado = 3;
crearVariable Control = 0;
/*==================[programa principal]=====================================*/
programa()
{
   // ---------- CONFIGURACIONES ------------------------------
   // Inicializar y configurar la plataforma
   configurarPlataforma();   
   
   // ---------- REPETIR POR SIEMPRE --------------------------
   porSiempre()
   {      
      si(Estado==0)
      {	if(Control==0)
      		Estado1();
      	else
      		Estado3();
      }	
      si(Estado==1)
      {	if(Control==0)
      		Estado2();
      	else
      		Estado0();
      }	
      si(Estado==2)
      {	if(Control==0)
      		Estado3();
      	else
      		Estado1();
      }
      si(Estado==3)
      {	if(Control==0)
      		Estado0();
      	else
      		Estado2();
      }      
   }   
}

void Estado0(void)
{
	Estado = 0;
	fijarPinDigitalEn( LEDB, ON );
	fijarPinDigitalEn( LED1, OFF );
	fijarPinDigitalEn( LED2, OFF );
	fijarPinDigitalEn( LED3, OFF );
	esperarSegs(0.5);
	VerificaTec();
}

void Estado1(void)
{
	Estado = 1;
	fijarPinDigitalEn( LEDB, OFF);
	fijarPinDigitalEn( LED1, ON );
	fijarPinDigitalEn( LED2, OFF );
	fijarPinDigitalEn( LED3, OFF );
	esperarSegs(0.5);
	VerificaTec();
}

void Estado2(void)
{
	Estado = 2;
	fijarPinDigitalEn( LEDB, OFF );
	fijarPinDigitalEn( LED1, OFF );
	fijarPinDigitalEn( LED2, ON );
	fijarPinDigitalEn( LED3, OFF );
	esperarSegs(0.5);
	VerificaTec();
}

void Estado3(void)
{
	Estado = 3;
	fijarPinDigitalEn( LEDB, OFF );
	fijarPinDigitalEn( LED1, OFF );
	fijarPinDigitalEn( LED2, OFF );
	fijarPinDigitalEn( LED3, ON );
	esperarSegs(0.5);
	VerificaTec();
}

void VerificaTec(void)
{
si(no(lecturaDigital(TEC3)))
	{Control = 1;}
	
si(no(lecturaDigital(TEC2)))
	{Control = 0;}

}

/*==================[fin del archivo]========================================*/

/*============================================================================
 * Copyright 2017, Lucas Andres Gimenez, Carlos Eduardo López Aldana.
 * All rights reserved.
 *
 * This file is part sAPI library for microcontrollers.
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
 

 * Date: 2017/04/10

 *===========================================================================*/

#ifndef _MEF_ASCENSORPUERTAS_H_
#define _MEF_ASCENSORPUERTAS_H_

/*==================[inclusiones]============================================*/


/*==================[macros]=================================================*/
#define setbit32(var, bit)               ((var) |=  (uint32_t)((uint32_t)1<<(uint32_t)(bit)))
#define clrbit32(var, bit)             ((var) &= ~(uint32_t)((uint32_t)1<<(uint32_t)(bit)))
#define querybit32(var, bit)             ((bool_t)((var)>>(uint32_t)(bit)) & ((uint32_t)1))


//volatile uint32_t flag1DW = 0;
// Bit 0  Indica en 1 que se ejecuto el bloque de ejecucion unica del estado PARADO del ascensor.
// Bit 1  En 1 indica el pedido de ejecucion de la secuencia de apertura de puertas.
// Bit 2  En 1 indica el pedido de ejecucion de la secuencia de cierre de puertas.
// Bit 3  En 1 ya se solicito apertura de las puertas en PB.
// Bit 4  En 1 se esta pidiendo un nuevo piso, lo cual lo informa "CargaNuevoPiso".
// Bit 31
#define Set_AscParadoFlag	setbit32(flag1DW, 0)
#define Clr_AscParadoFlag	clrbit32(flag1DW, 0)
#define Ask_AscParadoFlag	querybit32(flag1DW, 0)
#define Set_AbrePuertasFlag	setbit32(flag1DW, 1)
#define Clr_AbrePuertasFlag	clrbit32(flag1DW, 1)
#define Ask_AbrePuertasFlag	querybit32(flag1DW, 1)
#define Set_CierraPuertasFlag	setbit32(flag1DW, 2)
#define Clr_CierraPuertasFlag	clrbit32(flag1DW, 2)
#define Ask_CierraPuertasFlag	querybit32(flag1DW, 2)
#define Set_PidioAperturaFlag	setbit32(flag1DW, 3)
#define Clr_PidioAperturaFlag	clrbit32(flag1DW, 3)
#define Ask_PidioAperturaFlag	querybit32(flag1DW, 3)
#define Set_PideNuevoPisoFlag	setbit32(flag1DW, 4)
#define Clr_PideNuevoPisoFlag	clrbit32(flag1DW, 4)
#define Ask_PideNuevoPisoFlag	querybit32(flag1DW, 4)




/*==================[typedef]================================================*/
// Nuevo tipo de datos enumerado llamado estadoMEF
typedef enum{
	EN_PLANTA_BAJA,		// 0
	SUBIENDO,		// 1
	BAJANDO,		// 2
	PARADO,			// 3
	YENDO_A_PLANTA_BAJA,	// 4
	MODO_CONFIGURACION	// 5
} estadoMEFASC_t;

typedef enum{
	PUERTA_CERRADA,			// 0
	ABRIENDO_PUERTA,		// 1
	PUERTA_ABIERTA,			// 2
	INTENTANDO_CERRAR_PUERTAS,	// 3
	CERRANDO_PUERTA,		// 4
	ALARMA_PUERTA_ABIERTA		// 5
} estadoMEFAbreCierraPuerta_t;







/*==================[declaraciones de funciones]====================*/

void InicializarMEFAsc(void);
void ActualizarMEFAsc(void);
void InicializarMEFPuerta(void);
void ActualizaMEFPuerta(void);



/*==================[end of file]============================================*/
#endif /* _MEFASCENSORPUERTAS_H_ */

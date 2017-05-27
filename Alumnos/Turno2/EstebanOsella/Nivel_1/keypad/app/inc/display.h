/*============================================================================
 * Licencia: 
 * Autor: 
 * Fecha: 
 *===========================================================================*/

#ifndef _PROGRAM_H_
#define _PROGRAM_H_

/*==================[inclusiones]============================================*/

/*==================[c++]====================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/*==================[tipos de datos declarados por el usuario]===============*/

/*==================[declaraciones de datos externos]========================*/

/*==================[declaraciones de funciones externas]====================*/

void display7SegmentosTestearPines( void );

// Configuración de pines para el display 7 segmentos
void display7SegmentosConfigurarPines( void );

// Mostrar dato en el display 7 segmentos
void display7SegmentosMostrar( uint8_t symbolIndex );

/*==================[c++]====================================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* _PROGRAM_H_ */


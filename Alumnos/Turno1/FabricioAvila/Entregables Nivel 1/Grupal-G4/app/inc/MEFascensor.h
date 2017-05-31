#include "program.h"   // <= su propio archivo de cabecera
#include "sapi.h"



typedef enum{ EN_ESPERA, SUBIENDO, BAJANDO, ABRIENDO_PUERTA, PUERTA_ABIERTA, CERRANDO_PUERTA, PUERTA_CERRADA, MODO_CONFIGURACION, LEER_PISO,ALARMA_P_ABIERTA } estado_ascensor;
void InicializarMEF( void );
void ActualizarMEF( void );

estado_ascensor estadoActual;
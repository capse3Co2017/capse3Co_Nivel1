/*==================[bibliotecas]============================================*/

#include "snap_es.h"

/*==================[macros]=================================================*/

#define DERECHA   1
#define IZQUIERDA 0

/*==================[programa principal]=====================================*/

programa()
{
  // ---------- CONFIGURACIONES ------------------------------

  // Inicializar y configurar la plataforma
  configurarPlataforma();

  // Crear 2 variables e inicializarlas
  crearVariable iterador = LED1;    // Guarda el indice del LED a encender
  crearVariable sentido  = DERECHA; // Guarda el sentido de giro actual

  // Inicializar todos los pines apagados
  fijarPinDigitalEn( LED1, APAGAR );
  fijarPinDigitalEn( LED2, APAGAR );
  fijarPinDigitalEn( LED3, APAGAR );
  fijarPinDigitalEn( LEDR, APAGAR );

  fijarPinDigitalEn( LEDG, APAGAR );
  fijarPinDigitalEn( LEDB, APAGAR );

  //pwn//
  crearVariable valor = 0;
  crearVariable led = LEDR;
  crearVariable salida = 1;
  configurarPwm( 0, PWM_ENABLE );
  configurarConversorAD( ADC_ENABLE );


  // ---------- REPETIR POR SIEMPRE --------------------------

  porSiempre()
  {
    salida = 1;
    valor = lecturaAnalogica( CH1 );

    // Si el sentido de giro es a la derecha incrementa la variable iterador
    si( sentido == DERECHA ){
      // Esto se puede escribir tambien en C como "iterador++;"
      iterador = iterador + 1;
    }
    // Si no, entonces es a la izquierda y decrementa la variable iterador
    siNo{
      // Esto se puede escribir tambien en C como "iterador--;"
      iterador = iterador - 1;
    }

    // Si la variable iterador vale menor al indice LED1 entonces lo cambia a
    // LEDR que es el indice del otro extremo
    si( iterador < LED1 ){
      iterador = LEDR;
      valor = lecturaAnalogica( CH1 );
    }

    // Si la variable iterador vale mayor al indice LEDR entonces lo cambia a
    // LED1 que es el indice del otro extremo
    si( iterador > LEDR ){
      iterador = LED1;
      valor = lecturaAnalogica( CH1 );
    }

    // Enciende el Led en la posicion guardada en la variable iterador
    fijarPinDigitalEn( iterador, ENCENDER );

    // Espera medio segundo.
    esperarSegs(valor/1000.0);

    // Apaga el Led en la posicion guardada en la variable iterador
    fijarPinDigitalEn( iterador, APAGAR );
    valor = lecturaAnalogica( CH1 );

    // Si se esta presionada la TEC2, entonces cambia el sentido de giro a
    // la izquierda
    si( no(lecturaDigital(TEC2)) ){
      sentido = IZQUIERDA;
      valor = lecturaAnalogica( CH1 );
    }
    // Si se esta presionada la TEC3, entonces cambia el sentido de giro a
    // la derecha
    si( no(lecturaDigital(TEC3)) ){
      sentido = DERECHA;
      valor = lecturaAnalogica( CH1 );
    }
    si( no(lecturaDigital(TEC1)) ){
      while(salida != 0) {
        valor = lecturaAnalogica( CH1 );
        fijarPinDigitalEn( LED1, ENCENDER );
        fijarPinDigitalEn( LED2, ENCENDER );
        fijarPinDigitalEn( LED3, ENCENDER );
        fijarPinDigitalEn( led, ENCENDER );
        esperarSegs(valor/1000.0);
        fijarPinDigitalEn( LED1, APAGAR );
        fijarPinDigitalEn( LED2, APAGAR );
        fijarPinDigitalEn( LED3, APAGAR );
        fijarPinDigitalEn( led, APAGAR );
        esperarSegs(valor/1000.0);
        valor = lecturaAnalogica( CH1 );
        si (no (lecturaDigital(TEC2)) ){
          led = LEDB;
        }
        si (no (lecturaDigital(TEC3)) ){
          led = LEDR;
        }
        si (no (lecturaDigital(TEC4)) ){
          salida = 0;
        }
      }

      valor = lecturaAnalogica( CH1 );
    }
  }
}
/*============================================================================
 * Licencia: 
 * Autor: 
 * Fecha: 
 *===========================================================================*/

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
int main( void ){

   // ---------- CONFIGURACIONES ------------------------------
   
   // Inicializar y configurar la plataforma
   boardConfig();   
   
   // 0 a 180 grados
   uint8_t servoAngle = 0;

   // Configurar periferico Servo
   servoConfig( 0, SERVO_ENABLE );
   
   // Configurar pin de Servo SERVO0 (T_FIL1)
   servoConfig( SERVO0, SERVO_ENABLE_OUTPUT );

   // Escribir pin de Servo SERVO0
   servoWrite( SERVO0, servoAngle );
   
   // Leer pin de Servo SERVO0
   servoAngle = servoRead( SERVO0 );

   // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE )
   {      
       uint8_t anglemax = 45;
       uint8_t k = 0;
       for (k=0;k<anglemax;k++)
       {
           servoWrite( SERVO0, k );
           delay(50);
           }
           for (k=anglemax;k>=0;k--)
       {
           servoWrite( SERVO0, k );
           delay(50);
           }
           /*
      servoWrite( SERVO0, 0 );
      gpioWrite( LEDR, ON  );
      gpioWrite( LEDG, OFF );
      gpioWrite( LEDB, OFF );
      delay(500);

      servoWrite( SERVO0, 90 );
      gpioWrite( LEDR, OFF );
      gpioWrite( LEDG, ON  );
      gpioWrite( LEDB, OFF );
      delay(500);

      servoWrite( SERVO0, 180 );
      gpioWrite( LEDR, OFF );
      gpioWrite( LEDG, OFF );
      gpioWrite( LEDB, ON  );
      delay(500);*/
   } 

   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta 
   // directamenteno sobre un microcontroladore y no es llamado/ por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
}

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

/*==================[fin del archivo]========================================*/

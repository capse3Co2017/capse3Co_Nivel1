/*============================================================================
 * Licencia: 
 * Autor: 
 * Fecha: 
 *===========================================================================*/

/*==================[inlcusiones]============================================*/

#include "programa.h"   // <= su propio archivo de cabecera
#include "sapi.h"       // <= Biblioteca sAPI

/*==================[definiciones y macros]==================================*/
typedef enum{BUTTON_FALLING, 
             BUTTON_DOWN, 
             BUTTON_RISING,
             BUTTON_UP
    } keyFSM_t;

/*==================[definiciones de datos internos]=========================*/

/*==================[definiciones de datos externos]=========================*/
keyFSM_t actualState;
/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

void InicializarMEF( void );
void ActualizarMEF( void );

void buttonPressed(void);
void buttonReleased(void);
void romperMEF( void );

/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void ){

   // ---------- CONFIGURACIONES ------------------------------
   // Inicializar y configurar la plataforma
   boardConfig();   
   uartConfig( UART_USB, 115200 ); // Configurar UART a 115200 
   // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE )
   {
        ActualizarMEF();
        romperMEF();
   }

   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta 
   // directamenteno sobre un microcontroladore y no es llamado/ por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
}

/*==================[definiciones de funciones internas]=====================*/

void romperMEF( void ){
   if( !gpioRead(TEC4) ){
      actualState = 58;      
   }
}

delay_t bouncingTime;


// Función Inicializar MEF
void InicializarMEF( void ){
   actualState = BUTTON_UP;
   delayConfig( &bouncingTime, 40);
    uartWriteString(UART_USB,"MEF Reinicializada\r\n");
}
void buttonPressed()
{
    uartWriteString(UART_USB,"Button Pressed\r\n");
    }

void buttonReleased(){
    uartWriteString(UART_USB,"Button releassed\r\n");
    } 
    
void ActualizarMEF(void){
   
   switch(actualState) {
      case BUTTON_UP:
         if(!gpioRead(TEC1))
             actualState = BUTTON_FALLING;
      break;
      
      case BUTTON_DOWN:
          if(gpioRead(TEC1))
             actualState = BUTTON_RISING;            
      break;
      case BUTTON_FALLING:
         if( delayRead(&bouncingTime) ){
             if (!gpioRead(TEC1))
                 {
                     actualState = BUTTON_DOWN; 
                     buttonPressed();
                 }
             else
                 actualState = BUTTON_UP;
         }
      break;
      case BUTTON_RISING:
          if( delayRead(&bouncingTime) ){
              if (gpioRead(TEC1)){ 
                  actualState = BUTTON_UP; 
                  buttonReleased();
                  }
                  }
             else
                 actualState = BUTTON_DOWN;

         break;
      default:
         InicializarMEF();
      break;
    
   }      
}


/*==================[definiciones de funciones externas]=====================*/

/*==================[fin del archivo]========================================*/

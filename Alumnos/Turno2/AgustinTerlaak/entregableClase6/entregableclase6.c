#include "sapi.h"   // <= Biblioteca sAPI
// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET.
int flaga, flagv, flagr, flag1, flag2, flag3;

int main( void ){
  // ------------- INICIALIZACIONES ----------------
  boardConfig();                  // Inicializar la placa
  uartConfig( UART_USB, 115200 ); // Inicializar la UART2 a 115200 baudios
  uint8_t receivedByte;

  flaga=0;
  flagv=0;
  flagr=0;        //banderas de estado on / off
  flag1=0;
  flag2=0;
  flag3=0;

  char leda[] = "apagado \n";
  char ledv[] = "apagado \n";
  char ledr[] = "apagado \n";	//mensaje de estado
  char led1[] = "apagado \n";
  char led2[] = "apagado \n";
  char led3[] = "apagado \n";

  // ------------- REPETIR POR SIEMPRE -------------
  while(1) {
    // Se Recibe de la UART2 del microcontrolador, si hay byte recibido
    // entonces se chequea que letra es y ejecuta la funcion corresponiente

    uartWriteString( UART_USB, "Ingrese a, v, r, 1, 2 o 3. \n");
    uartWriteString( UART_USB, "Estado led Azul \n" );
    uartWriteString( UART_USB, leda );
    uartWriteString( UART_USB, "Estado led Verde \n" );
    uartWriteString( UART_USB, ledv );
    uartWriteString( UART_USB, "Estado led rojo \n" );
    uartWriteString( UART_USB, ledr );
    uartWriteString( UART_USB, "Estado led 1 \n" );
    uartWriteString( UART_USB, led1 );
    uartWriteString( UART_USB, "Estado led 2 \n" );
    uartWriteString( UART_USB, led2 );
    uartWriteString( UART_USB, "Estado led 3 \n" );
    uartWriteString( UART_USB, led3 );
    delay (1000);
    if( uartReadByte( UART_USB, &receivedByte ) )
    {
      switch( receivedByte )
      {
        case 'a': // Si recibe 'a' entonces prender led blanco
        if (flaga == 0)
        {
          uartWriteString( UART_USB, "LED Azul Ahora prendido\r\n" );
          flaga = 1;
          gpioWrite( LEDB, ON );
          sprintf(leda,"%s","Encendido \n");

        }
        else
        {
          uartWriteString( UART_USB, "LED Azul Ahora apagado\r\n" );
          flaga = 0;
          gpioWrite( LEDB, OFF );
          sprintf(leda,"%s","Apagado \n");
        }
        break;
        case 'v': // Si recibe 'v' entonces
        if (flagv == 0)
        {
          uartWriteString( UART_USB, "LED verde Ahora prendido\r\n" );
          flagv = 1;
          gpioWrite( LEDG, ON );
          sprintf(ledv,"%s","Encendido \n");
        }
        else
        {
          uartWriteString( UART_USB, "LED verde Ahora apagado\r\n" );
          flagv = 0;
          gpioWrite( LEDG, OFF );
          sprintf(ledv,"%s","Apagado \n");
        }
        break;
        case 'r':
        if (flagr == 0)
        {
          uartWriteString( UART_USB, "LED rojo Ahora prendido\r\n" );
          flagr = 1;
          gpioWrite( LEDR, ON );
          sprintf(ledr,"%s","Encendido \n");
        }
        else
        {
          uartWriteString( UART_USB, "LED rojo Ahora apagado\r\n" );
          flagr = 0;
          gpioWrite( LEDR, OFF );
          sprintf(ledr,"%s","Apagado \n");
        }
        break;
        case '1': // Si recibe '1' entonces
        if (flag1 == 0)
        {
          uartWriteString( UART_USB, "LED 1 Ahora prendido\r\n" );
          flag1 = 1;
          gpioWrite( LED1, ON );
          sprintf(led1,"%s","Encendido \n");
        }
        else
        {
          uartWriteString( UART_USB, "LED 1 Ahora apagado\r\n" );
          flag1 = 0;
          gpioWrite( LED1, OFF );
          sprintf(led1,"%s","Apagado \n");
        }
        break;
        case '2':
        if (flag2 == 0)
        {
          uartWriteString( UART_USB, "LED 2 Ahora prendido\r\n" );
          flag2 = 1;
          gpioWrite( LED2, ON );
          sprintf(led2,"%s","Encendido \n");
        }
        else
        {
          uartWriteString( UART_USB, "LED 2 Ahora apagado\r\n" );
          flag2 = 0;
          gpioWrite( LED2, OFF );
          sprintf(led2,"%s","Apagado \n");
        }
        break;
        case '3':
        if (flag3 == 0)
        {
          uartWriteString( UART_USB, "LED 3 Ahora prendido\r\n" );
          flag3 = 1;
          gpioWrite( LED3, ON );
          sprintf(led3,"%s","Encendido \n");
        }
        else
        {
          uartWriteString( UART_USB, "LED 3 Ahora apagado\r\n" );
          flag3 = 0;
          gpioWrite( LED3, OFF );
          sprintf(led3,"%s","Apagado \n");
        }
        break;
        default: // Si recibe cualquier otra cosa envia un mensaje de error
        uartWriteString( UART_USB, "Error. Ingrese a, v, r, 1, 2 o 3.\r\n" );
        break;
      }
    }
  }
  // NO DEBE LLEGAR NUNCA AQUI
  return 0 ;
}

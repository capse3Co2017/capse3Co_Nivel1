
/*==================[inlcusiones]============================================*/

#include "programa.h"   // <= su propio archivo de cabecera
#include "sapi.h"       // <= Biblioteca sAPI



/*==================[definiciones y macros]==================================*/
#define setbit32(var, bit)               ((var) |=  (uint32_t)((uint32_t)1<<(uint32_t)(bit)))
#define clrbit32(var, bit)             ((var) &= ~(uint32_t)((uint32_t)1<<(uint32_t)(bit)))
#define querybit32(var, bit)             ((bool_t)((var)>>(uint32_t)(bit)) & ((uint32_t)1))




/*==================[definiciones de datos internos]=========================*/
static uint8_t timer1segB = 0;    // Contador para un timer de 1 seg.


uint8_t datouartB = 0;


volatile uint32_t flag1DW = 0;
// Bit 0  Indica en 1 que a pasado 1 seg.

// Bit 31
#define Set_cont1seg	setbit32(flag1DW, 0)
#define Clr_cont1seg	clrbit32(flag1DW, 0)
#define Ask_cont1seg	querybit32(flag1DW, 0)




/*==================[definiciones de datos externos]=========================*/




/*==================[declaraciones de funciones externas]====================*/








//*************************************************************************************************
//			Interrupcion cada 10ms
//*************************************************************************************************
bool_t IntTimer( void *ptr )
{


timer1segB++;
if(timer1segB == 100)
	{
	timer1segB = 0;
	Set_cont1seg;		// Flag que indica que paso 1 seg.
	}


return 1;
}
//*************************************************************************************************
//*************************************************************************************************




// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O REset.
int main( void ){

// ---------- CONFIGURACIONES ------------------------------
// Inicializar y configurar la plataforma
boardConfig();   

// Interupcion cada 10ms.
tickConfig( 10, IntTimer );

// UART_USB a 115200 baudios.
uartConfig( UART_USB, 115200 );



// ---------- REPETIR POR SIEMPRE --------------------------
while( TRUE )
{      



// Recibe datos de la UART_USB.
if (uartReadByte(UART_USB, &datouartB))
	{ 
	switch (datouartB){
		case 'a':
			gpioWrite( LEDB, !gpioRead(LEDB));
			break;

		case 'v':
			gpioWrite( LEDG, !gpioRead(LEDG));
			break;

		case 'r':
			gpioWrite( LEDR, !gpioRead(LEDR));
			break;

		case '1':
			gpioWrite( LED1, !gpioRead(LED1));
			break;

		case '2':
			gpioWrite( LED2, !gpioRead(LED2));
			break;

		case '3':
			gpioWrite( LED3, !gpioRead(LED3));
			break;

		default:
			break;
		}
	}




// Consulta si ya paso 1 seg.
if (Ask_cont1seg)
	{
	Clr_cont1seg;
	
	if (!gpioRead(TEC1))
		uartWriteString(UART_USB, "Tecla1 ON\r\n");
	else	uartWriteString(UART_USB, "Tecla1 OFF\r\n");

	if (!gpioRead(TEC2))
		uartWriteString(UART_USB, "Tecla2 ON\r\n");
	else	uartWriteString(UART_USB, "Tecla2 OFF\r\n");

	if (!gpioRead(TEC3))
		uartWriteString(UART_USB, "Tecla3 ON\r\n");
	else	uartWriteString(UART_USB, "Tecla3 OFF\r\n");

	if (!gpioRead(TEC4))
		uartWriteString(UART_USB, "Tecla4 ON\r\n");
	else	uartWriteString(UART_USB, "Tecla4 OFF\r\n");
	
	
	if (gpioRead(LEDR))
		uartWriteString(UART_USB, "Led Rojo del rgb ENCENDIDO\r\n");
	else	uartWriteString(UART_USB, "Led Rojo del rgb APAGADO\r\n");
	
	if (gpioRead(LEDG))
		uartWriteString(UART_USB, "Led Verde del rgb ENCENDIDO\r\n");
	else	uartWriteString(UART_USB, "Led Verde del rgb APAGADO\r\n");
	
	if (gpioRead(LEDB))
		uartWriteString(UART_USB, "Led Azul del rgb ENCENDIDO\r\n");
	else	uartWriteString(UART_USB, "Led Azul del rgb APAGADO\r\n");
	
	
	if (gpioRead(LED1))
		uartWriteString(UART_USB, "Led 1 ENCENDIDO\r\n");
	else	uartWriteString(UART_USB, "Led 1 APAGADO\r\n");
	
	if (gpioRead(LED2))
		uartWriteString(UART_USB, "Led 2 ENCENDIDO\r\n");
	else	uartWriteString(UART_USB, "Led 2 APAGADO\r\n");
	
	if (gpioRead(LED3))
		uartWriteString(UART_USB, "Led 3 ENCENDIDO\r\n\n");
	else	uartWriteString(UART_USB, "Led 3 APAGADO\r\n\n");
	}




} 
// NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta 
// directamenteno sobre un microcontroladore y no es llamado/ por ningun
// Sistema Operativo, como en el caso de un programa para PC.
return 0;
}


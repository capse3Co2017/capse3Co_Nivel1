/** @mainpage Entregable clase 6
   Practica Doxigen documentando el proyecto Acelerometro + Reloj grabados a tarjeta SD.

   Usa las sAPI de Eric Pernia para RealTimeClock integrado en micro y Acelerometro HMC.

   Usa las librerias de Chan para SD.

   Archivo donde esta el main: sd_spi.c

   @note Los ejemplos de CAPAS estan en comentarios dentro de main
 */
 
/*==================[inlcusiones]============================================*/
#include "rtc.h"
#include "sd_spi.h"
#include "sapi.h"
#include "ff.h"

/*==================[definiciones y macros]==================================*/

#define FILENAME "Datos.txt"  ///< nombre del archivo en el SD
#define DATOSX 26             ///< posicion de datos Ascii X en el Buffer[]
#define DATOSY DATOSX + 7     ///< posicion de datos Ascii Y respecto de X en el Buffer[]
#define DATOSZ DATOSY + 7     ///< posicion de datos Ascii Z respecto de X en el Buffer[]
#define DATOSD 0              ///< posicion de datos Ascii Dia en el Buffer[]
#define DATOSH 12             ///< posicion de datos Ascii Hora respecto de dia en el Buffer[]
#define BUFFERTAM 47          ///< tamaño  del Buffer[]

/*==================[definiciones de datos internos]=========================*/

static FATFS fs;              ///<  FatFs work area needed for each volume
static FIL fp;                ///< File object needed for each open file
            //   012345678901234567890123456789012345678901234567   para calcular posicion y tamaño
char Buffer[] = "DD/MM/YYYY, HH:MM:SS => X=    ,Y=    ,Z=    \r\n";  ///< Formacion de Buffer[] fijo a completar por cargarDatosRTC y cargarDatosHMC
/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/
/**itoa = Integer To Ascii

Esta funcion es la encargada de convertir int value (pasado por referencia)  y devolver un puntero a string
con la cadena de caracterees de resultado

@param value es un int y contiene una copia del valor a convertir
@param result es un puntero y apunta a la cadena de caracteres de respuesta
@param base es un int y se le pasa el tipo de sistema de numeracion: 2=binario; 10=decimal; 16=HEXA
@returns retorna result idem a parametro result
*/
char* itoa(int value, char* result, int base) {
   // check that the base if valid
   if (base < 2 || base > 36) { *result = '\0'; return result; }

   char* ptr = result, *ptr1 = result, tmp_char;
   int tmp_value;

   do {
      tmp_value = value;
      value /= base;
      *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
   } while ( value );

   // Apply negative sign
   if (tmp_value < 0) *ptr++ = '-';
   *ptr-- = '\0';
   while(ptr1 < ptr) {
      tmp_char = *ptr;
      *ptr--= *ptr1;
      *ptr1++ = tmp_char;
   }
   return result;
}

/**cargarDatosRTC

Esta funcion es la encargada de escribir en Buffer[] los datos de RealTimeClock
@param rtc es un puntero y apunta a la estructura de datos del RealTimeClock a convertir en ascii por itoa
@returns void nada
@par DATOSD y DATOSH son indices definidos para no usar numeros magicos segun Note
@note   posicion ascii===========0123456789  0123456
@note   fecha y hora en formato="DD/MM/YYYY, HH:MM:SS"
@par El objetivo final es cargar Buffer[]*/
void cargarDatosRTC( rtc_t * rtc ){
	   char Buff[]="    ";
	   itoa( (int) (rtc->mday), (char*)Buff, 10 );
	   if( (rtc->mday)<10 ) {
		   Buffer[DATOSD]=  '0';
		   Buffer[DATOSD+1]=  Buff[0];
	   }else{
		   Buffer[DATOSD]=  Buff[0];
		   Buffer[DATOSD+1]=  Buff[1];
	   }

	   itoa( (int) (rtc->month), (char*)Buff, 10 );
	   if( (rtc->month)<10 ) {
		   Buffer[DATOSD+3]=  '0';
		   Buffer[DATOSD+4]=  Buff[0];
	   }else{
		   Buffer[DATOSD+3]=  Buff[0];
		   Buffer[DATOSD+4]=  Buff[1];
	   }

	   itoa( (int) (rtc->year), (char*)Buff, 10 );
		   Buffer[DATOSD+6]=  Buff[0];
		   Buffer[DATOSD+7]=  Buff[1];
		   Buffer[DATOSD+8]=  Buff[2];
		   Buffer[DATOSD+9]=  Buff[3];

	   itoa( (int) (rtc->hour), (char*)Buff, 10 );
	   if( (rtc->hour)<10 ) {
		   Buffer[DATOSH]=  '0';
		   Buffer[DATOSH+1]=  Buff[0];
	   }else{
		   Buffer[DATOSH]=  Buff[0];
		   Buffer[DATOSH+1]=  Buff[1];
	   }

	   itoa( (int) (rtc->min), (char*)Buff, 10 );
	   if( (rtc->min)<10 ) {
		   Buffer[DATOSH+3]=  '0';
		   Buffer[DATOSH+4]=  Buff[0];
	   }else{
		   Buffer[DATOSH+3]=  Buff[0];
		   Buffer[DATOSH+4]=  Buff[1];
	   }

	   itoa( (int) (rtc->sec), (char*)Buff, 10 );
	   if( (rtc->sec)<10 ) {
		   Buffer[DATOSH+6]=  '0';
		   Buffer[DATOSH+7]=  Buff[0];
	   }else{
		   Buffer[DATOSH+6]=  Buff[0];
		   Buffer[DATOSH+7]=  Buff[1];
	   }
}
/**cargarDatosHMC

Esta funcion es la encargada de escribir en Buffer[] los datos del Magnetometro HMC
@param ejeRaw es un valor int16 crudo a convertir en ascii por itoa
@param Pos es la posicion dentro de Buffer[]
@note uso una variable local itoBuffAux que se inicializa en espacios o sea limpio lo que retorna de itoa
@returns void nada
@par El objetivo final es cargar Buffer[]*/

void cargarDatosHMC(int16_t ejeRaw,uint8_t Pos){
	  uint8_t i;
	  char itoBuffAux[]="    ";
	  itoa( ejeRaw, itoBuffAux, 10 ); /* base 10 significa decimal */
	  for (i=0; i<=3;i++){
		   if (itoBuffAux[i]!=0)  Buffer[Pos+i]= itoBuffAux[i];
		   else Buffer[8+i]= ' ';
	}
}


/*==================[declaraciones de funciones externas]====================*/
/** FUNCION que se ejecuta cada vezque ocurre un Tick*/

bool_t diskTickHook( void *ptr );


/*==================[funcion principal]======================================*/

/** @par FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.*/
int main( void ){
   ///@par--------------------------CONFIGURACIONES GENERALES--------------------------

   /// Inicializar y configurar la plataforma
   boardConfig();

   /// SPI configuration
   spiConfig(SPI0);
   
   /// Inicializar el conteo de Ticks con resolución de 10ms,
   /// con tickHook diskTickHook
   tickConfig( 10, diskTickHook );
             
   /// ------ Montaje de SD -------

   UINT nbytes;
   
   // Give a work area to the default drive
   if( f_mount( &fs, "", 0 ) != FR_OK ){
      // If this fails, it means that the function could
      // not register a file system object.
      // Check whether the SD card is correctly connected
   }
   /// ------ Inicalizo HMC -------
   delay_t ciclo;
   delayConfig(&ciclo,3000);
   int16_t hmc5883l_x_raw;
   int16_t hmc5883l_y_raw;
   int16_t hmc5883l_z_raw;
   ///declaro variables de retorno y pongo a medicion continua el sensor

   HMC5883L_config_t hmc5883L_configValue;
   hmc5883lPrepareDefaultConfig( &hmc5883L_configValue );
   hmc5883L_configValue.mode = HMC5883L_continuous_measurement;
   hmc5883L_configValue.samples = HMC5883L_8_sample;
   hmc5883lConfig( hmc5883L_configValue );

   /// ------ Inicalizo RelojTiempoReal -------
   rtc_t rtc;              /* Estructura RTC */
   rtc.year = 2017;
   rtc.month = 5;
   rtc.mday = 26;
   rtc.wday = 3;
   rtc.hour = 2;
   rtc.min = 30;
   rtc.sec= 0;
   rtcConfig( &rtc );     ///  pone en hora

   ///@par--------------------------REPETIR POR SIEMPRE --------------------------
   while( TRUE )
   {  ///leer sensor HMC
		  hmc5883lRead( &hmc5883l_x_raw, &hmc5883l_y_raw, &hmc5883l_z_raw );
	  ///___________Capa Aplicacion:actual<=>Capa Servicio:sapi_hmc5883l<=>Capa Interface:sapi_i2c<=>Capa Driver:i2c_18xx_43xx
		  delay(100);
      ///si el delay ciclador lo indica hacer:
    if (delayRead(&ciclo)){
    	gpioWrite( LEDR, OFF );
    	gpioWrite( LEDG, OFF );
     ///.....leer el RealTimeClock,
        rtcRead( &rtc );
	 ///___________Capa Aplicacion:actual<=>Capa Servicio:N/A<=>Capa Interface:sapi_rtc<=>Capa Driver:rtc_18xx_43xx
        cargarDatosRTC( &rtc );
     ///.....completar el Buffer[],
        cargarDatosHMC(hmc5883l_x_raw,DATOSX);
        cargarDatosHMC(hmc5883l_y_raw,DATOSY);
        cargarDatosHMC(hmc5883l_z_raw,DATOSZ);

     ///.....escribir a SD,
    	   // Create/open a file, then write a string and close it
    	   if( f_open( &fp, FILENAME, FA_WRITE | FA_OPEN_APPEND ) == FR_OK ){
    	      f_write( &fp, Buffer, BUFFERTAM, &nbytes );
    			 ///___________Capa Aplicacion:actual<=>Capa Servicio:ff<=>Capa Interface:sapi_spi<=>Capa Driver:ssp_18xx_43xx
    	      f_close(&fp);
    	  ///..... prender led segun corresponda:
    	      if( nbytes == BUFFERTAM ){
    	         ///.....	 Turn ON LEDG if the write operation was successful
    	         gpioWrite( LEDG, ON );
    	      }
    	   } else{
    	        ///.....	 Turn ON LEDR if the write operation was fail
    	      gpioWrite( LEDR, ON );
    	   }
    }
   } 
   ///@par--------------------------FIN REPETIR POR SIEMPRE --------------------------
   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta 
   // directamenteno sobre un microcontroladore y no es llamado/ por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
}

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

// FUNCION que se ejecuta cada vezque ocurre un Tick
bool_t diskTickHook( void *ptr ){
   disk_timerproc();   // Disk timer process
   return 1;
}


/*==================[fin del archivo]========================================*/

-
   
//Creamos el siguiente pool de variables para conocer el estado de los leds de la placa
	int estadoA = 0;
	int estadoV = 0;
	int estadoR = 0;
	int estado1 = 0;
	int estado2 = 0;
	int estado3 = 0;
	
//Creamos el siguiente pool de variables para conocer el estado de las teclas de la placa
	int estadoTec1 = 0;
	int estadoTec2 = 0;
	int estadoTec3 = 0;
	int estadoTec4 = 0;
	
//Creamos la siguienta variable para recibir datos desde la pc host a la EDU-CIAA
	uint8_t receivedByte;
   
   // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE )
   {      
//Lo primero que hacemos es leer el dato que viene desde la pc host a traves de la UART_USB.
//En funcion del caracter recibido hacemos un 'toggle' al led correspondiente con dicho
//caracter.
	 if( uartReadByte( UART_USB, &receivedByte ) ) {
	 

		switch( receivedByte ){   
            		case 'a': 
               			gpioToggle(LEDB);
            			break;
           		case 'v': 
               			gpioToggle(LEDG);
            			break;
            		case 'r': 
               			gpioToggle(LEDR);
            			break;
            		case '1': 
               			gpioToggle(LED1);
            			break;
            		case '2': 
               			gpioToggle(LED2);
            			break;
            		case '3': 
               			gpioToggle(LED3);
            			break;
            		default: 
               			uartWriteString( UART_USB, "Error. Ingrese a, v, r, 1, 2, o 3.\r\n" );
             			break;
             	}
	}
	
	
//En esta parte del codigo leemos el estado de los leds y las teclas de la EDU-CIAA.
//Si el led está encendido mandamos el mensaje "LEDx ON", si se encuentra apagado
//mandamos "LEDx OFF". Con las teclas es lo mismo, pero invertido (ya que las teclas
//se consideran presionadas cuando leemos un '0' en el pin). Todos los mensajes son
//enviados a traves de la UART_USB con un delay de 1000 mSeg.
	estadoA = gpioRead (LEDB);
	if (estadoA == 1){
		uartWriteString( UART_USB,  "LEDB ON\r\n" );
		delay (1000); }
	else {
		uartWriteString( UART_USB,  "LEDB OFF\r\n" );
		delay (1000); }
	
	estadoV = gpioRead (LEDG);
	if (estadoV == 1){
		uartWriteString( UART_USB,  "LEDG ON\r\n" );
		delay (1000); }
	else {
		uartWriteString( UART_USB,  "LEDG OFF\r\n" );
		delay (1000); }
	
	estadoR = gpioRead (LEDR);
	if (estadoR == 1){
		uartWriteString( UART_USB,  "LEDR ON\r\n" );
		delay (1000); }
	else {
		uartWriteString( UART_USB,  "LEDR OFF\r\n" );
		delay (1000); }
	
	estado1 = gpioRead (LED1);
	if (estado1 == 1){
		uartWriteString( UART_USB,  "LED1 ON\r\n" );
		delay (1000); }
	else {
		uartWriteString( UART_USB,  "LED1 OFF\r\n" );
		delay (1000); }
	
	estado2 = gpioRead (LED2);
	if (estado2 == 1){
		uartWriteString( UART_USB,  "LED2 ON\r\n" );
		delay (1000); }
	else {
		uartWriteString( UART_USB,  "LED2 OFF\r\n" );
		delay (1000); }
	
	estado3 = gpioRead (LED3);
	if (estado3 == 1){
		uartWriteString( UART_USB,  "LED3 ON\r\n" );
		delay (1000); }
	else {
		uartWriteString( UART_USB,  "LED3 OFF\r\n" );
		delay (1000); }
	
	estadoTec1 = gpioRead (TEC1);
	if (estadoTec1 == 1){
		uartWriteString( UART_USB,  "TEC1 OFF\r\n" );
		delay (1000); }
	else {
		uartWriteString( UART_USB,  "TEC1 ON\r\n" );
		delay (1000); }
	
	estadoTec2 = gpioRead (TEC2);
	if (estadoTec2 == 1){
		uartWriteString( UART_USB,  "TEC2 OFF\r\n" );
		delay (1000); }
	else {
		uartWriteString( UART_USB,  "TEC2 ON\r\n" );
		delay (1000); }
	
	estadoTec3 = gpioRead (TEC3);
	if (estadoTec3 == 1){
		uartWriteString( UART_USB,  "TEC3 OFF\r\n" );
		delay (1000); }
	else {
		uartWriteString( UART_USB,  "TEC3 ON\r\n" );
		delay (1000); }
		
	estadoTec4 = gpioRead (TEC4);
	if (estadoTec4 == 1){
		uartWriteString( UART_USB,  "TEC4 OFF\r\n" );
		delay (1000); }
	else {
		uartWriteString( UART_USB,  "TEC4 ON\r\n" );
		delay (1000); }
	
	
	
   } 

   return 0;
}

	

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

/*==================[fin del archivo]========================================*/

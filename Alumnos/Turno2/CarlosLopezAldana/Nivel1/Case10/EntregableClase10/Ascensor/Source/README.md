*******************************************************************************

Todo el código fue diseñado y desarrollado por Carlos Eduardo López Aldana y
Lucas Andres Gimenez, como parte del trabajo práctico entregable de la clase
número 10, en el marco de los Cursos Abiertos de Programacion de Sistemas
Embebidos (CAPSE). Año 2017.

*******************************************************************************

En este directorio se pueden encontrar todos los archivos .c que conforman
la computadora de un ascensor. El archivo principal es el llamado Ascensor.c
mientras que el resto de los archivos conforman el banco de drivers o módulos
del sistema completo, entre los cuales se encuentran: 

        - MEF_ascensorPuertas.c (Máquina de estados finitos para conocer el 
        estado de las puertas y los estados por el cual transita el ascensor,
        todo en tiempo real).

        - MEF_tecladoMatricial.c (Máquina de estados finitos para conocer el
        estado del teclado matricial en tiempo real, hay que recordar que
        dentro de este mismo archivo se encuentra lo que conforma el driver
        del mismo teclado matricial).

        - driverDisplay.c (Driver que permite el manejo y configuración del
        display 7 segmentos x 4).

        - modoConfiguración.c (Archivo en el cual fue diseñado y desarrollado
        todo lo pedido para el modo configuración del ascensor).

        - Remoto.c (Archivo que nos permite conocer, presentado a través de
        una terminal serie, todos los estados del ascensor. Se incluyen, la
        visualización del buffer que almacena los pisos ingresados, los estados
        de las puertas, los estados del teclado matricial y el estado del
        movimiento/parado del ascensor en general).

*******************************************************************************

Contacto con los autores:

        Carlos Eduardo López Aldana -->>  cela1972@gmail.com
        Lucas Andres Gimenez        -->>  lucas07gimenez@gmail.com

*******************************************************************************


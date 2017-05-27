Eric estuve trabajando un rato con el tema del teclado que nos quedo al final d ayer, entre algunas cosas que encontré es que faltaba la llama a "configurarTecladoMatricial();"
Al final tomaba teclas pero tenia problemas, algunas veces no tomaba....
Asi que opte por alterar algo el programa, como se ve engorroso el tema del scaneo de la teclas, arme una funcion que devuelve 1 si hay alguna tecla presionada y carga en
"key" la tecla reconocida (la primera que se presiona).
Ya con esta función se hace mas fácil implementar la MEF prupuesta, incluso puse la consulta si es la misma tecla, se puede pulir algunos detalles pero lo que te adjunto esta funcionando.


PD Yo uso Realterm por lo cual agregue una linea "uartWriteString( UART_USB, "\x1b[2J\x1b[H" );" para borrar la terminal en PC, comento por las dudas que cause problemas....
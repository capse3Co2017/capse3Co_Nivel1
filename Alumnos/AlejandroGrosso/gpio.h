#ifndef _GPIO_H_
#define _GPIO_H_

//#include "lpc_17xx_44xx.h"


#define reg_m(port, pin) ((port)*2+(pin)/16)
#define bitPos_m(pin) (((pin)%16)*2)

#define gpioConfigMux_m(pinNamePort,pinNamePin,pinMode,pinFunc)\
                  LPC_IOCON->PINSEL[reg_m(pinNamePort,pinNamePin)]=LPC_IOCON->PINSEL[reg_m(pinNamePort,pinNamePin)]&\
                                                                   ~(0x3UL << bitPos_m(pinNamePin)) |\
                                                                    (pinFunc << bitPos_m(pinNamePin));\
                 LPC_IOCON->PINMODE[reg_m(pinNamePort,pinNamePin)]=LPC_IOCON->PINMODE[reg_m(pinNamePort,pinNamePin)]&\
                                                                   ~(0x3UL << bitPos_m(pinNamePin)) |\
                                                                    (pinMode << bitPos_m(pinNamePin));

#define gpioConfig(p) gpioConfigMux_m(p##port_enum__, p##pin_enum__, p##mode_enum__, p##func_enum__)

#endif

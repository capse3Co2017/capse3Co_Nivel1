#ifndef SAPI_I18N_ES
#define SAPI_I18N_ES

#include "sapi.h"

/*
#include <stdio.h>
#include <stdint.h>
*/

#define definir                                   define
#define incluir                                   include
#define siDefinido                                ifdef
#define siNoDefinido                              ifndef
#define finSi                                     endif


#define CIERTO                                    TRUE
#define VERDADERO                                 TRUE
#define FALSO                                     FALSE

#define ENCENDER                                  ON
#define APAGAR                                    OFF

#define ALTO                                      HIGH
#define BAJO                                      LOW


#define funcionPrincipal                          main


#define vacio                                     void

#define entero                                    int
#define corto                                     short
#define largo                                     long
#define caracter                                  char
#define flotante                                  float
#define dobleFlotante                             double

#define sinSigno                                  unsigned
#define registro                                  register
#define volatil                                   volatile
#define externa                                   extern
#define privada                                   static


#define booleano_t                                bool_t

#define enteroSinSigno8_t                         uint8_t
#define enteroSinSigno16_t                        uint16_t
#define enteroSinSigno32_t                        uint32_t
#define enteroSinSigno64_t                        uint64_t

#define entero8_t                                 int8_t
#define entero16_t                                int16_t
#define entero32_t                                int32_t
#define entero64_t                                int64_t

#define contadorDeBaseDeTiempo_t                  tick_t

#define sAPI_PunteroAFuncion_t                    sAPI_FuncPtr_t
#define sAPI_PunteroAFuncionNula                  sAPI_NullFuncPtr


#define si                                        if
#define siNo                                      else
#define conmutar                                  switch
#define elegir                                    switch
#define caso                                      case
#define casoPorDefecto                            default
#define hacer                                     do
#define mientras                                  while
#define repetirPara                               for

#define retornar                                  return
#define continuar                                 continue
#define romper                                    break

#define imprimir                                  printf


#define mapaDePines_t                             gpioMap_t
#define mapaDeConversoresAD_t                     adcMap_t
#define mapaDeConversoresDA_t                     dacMap_t
#define mapaDeUARTs_t                             uartMap_t
#define mapaDeTemporizadores_t                    timerMap_t
#define mapaDeI2Cs_t                              i2cMap_t
#define mapaDePWMs_t                              pwmMap_t
#define mapaDeSERVOs_t                            servoMap_t

#define configurarConversorAD_t                   adcConfig_t
#define configurarConversorAD                     adcConfig
#define leerConversorAD                           adcRead
                                                
#define configurarPlataforma                      boardConfig

#define configurarConversorDA_t                   dacConfig_t
#define configurarConversorDA                     dacConfig
#define escribirConversorDA                       dacWrite

#define retardo_t                                 delay_t
#define retardoInexacto                           delayInaccurate
#define retardoInexactoUs                         delayInaccurateUs
#define retardo                                   delay
#define configurarRetardo                         delayConfig
#define seCumplioElRetardo                        delayRead
#define escribirRetardo                           delayWrite

#define configurarPin_t                           gpioConfig_t
#define configurarPin                             gpioConfig
#define leerPin                                   gpioRead
#define escribirPin                               gpioWrite

#define configurarI2c                             i2cConfig
#define leerI2c                                   i2cRead
#define escribirI2c                               i2cWrite

#define configurarPwm_t                           pwmConfig_t
#define configurarPwm                             pwmConfig
#define leerPwm                                   pwmRead
#define escribirPwm                               pwmWrite

#define relojDeTiempoReal_t                       rtc_t
#define configurarRelojDeTiempoReal               rtcConfig
#define leerRelojDeTiempoReal                     rtcRead
#define escribirRelojDeTiempoReal                 rtcWrite

#define configurarServo_t                         servoConfig_t
#define configurarServo                           servoConfig
#define leerServo                                 servoRead
#define escribirServo                             servoWrite

#define dormirHastaLaProximaInterrupcion          sleepUntilNextInterrupt

#define configurarBaseDeTiempo                    tickConfig
#define leerContadorDeBaseDeTiempo                tickRead
#define escribirContadorDeBaseDeTiempo            tickWrite

#define estadoDeEsperarARecibirCadenaOTiempoCumplido_t   waitForReceiveStringOrTimeoutState_t
#define esperarARecibirCadenaOTiempoCumplido_t           waitForReceiveStringOrTimeout_t
#define esperarARecibirCadenaOTiempoCumplido             waitForReceiveStringOrTimeout
#define esperarARecibirCadenaOTiempoCumplidoBloqueando   waitForReceiveStringOrTimeoutBlocking
#define configurarUart                                   uartConfig
#define recibirBytePorUart                               uartReadByte
#define eviarBytePorUart                                 uartWriteByte
#define eviarCadenaPorUart                               uartWriteString

#define APAGAR_DISPLAY_7_SEGMENTOS                DISPLAY_7_SEGMENT_OFF
#define testearPinesDeDisplay7Segmentos           display7SegmentTestPins
#define configurarPinesDeDisplay7Segmentos        display7SegmentPinConfig
#define escribirDisplay7Segmentos                 display7SegmentWrite

#define tecladoMatricial_t                        keypad_t
#define configurarTecladoMatricial                keypadConfig
#define leerTecladoMatricial                      keypadRead

#endif /* SAPI_I18N_ES */

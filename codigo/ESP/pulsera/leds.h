#ifndef lends_h
#define lends_h

  #define ledAlertaPin 12
  #define ledSincronizacionPin 11

  extern unsigned long tiempoAnterior;
  extern bool encendido;

  void encenderLED(int pin, int tiempo);
  void encenderLEDS(int tiempo, int i = 3);
  void declarar_leds();
  void LEDSoff();

#endif
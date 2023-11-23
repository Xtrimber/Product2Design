#ifndef boton_h
#define boton_h

  #define botonPin 7

  extern volatile bool flagInterrupcion;

  void declarar_boton();
  void prueva_boton();
  void manejoInterrupcion_boton();
#endif
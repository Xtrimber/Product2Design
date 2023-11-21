#ifndef maquinaEstados_h
#define maquinaEstados_h
  #include "transmisiondatos.h"

  // Constantes de tiempo
  #define tiempoParaEsclavo 3000 // Tiempo en milisegundos para entrar en modo esclavo
  #define tiempoParaMaestro 6000 // Tiempo en milisegundos para entrar en modo maestro
  #define tiempoParaApagado 9000 // Tiempo en milisegundos para apagar

  extern unsigned long tiempoSincronizacion; // Tiempo en milisegundos para entrar en modo esclavo
  extern unsigned long tiempoAnterior;
  extern bool encendidoSincronizacion;
  extern bool esclavo;
  extern char nombreDisp_conec[20];
  extern char nombreDispositivo;
  extern bool enModoMaestro;
  extern char puesto;

  #include "leds.h"
  #include "boton.h"
/*Maquina de estados*/
  /*estructura*/
    enum Estado
    {
      ENCENDIDO,
      SINCRONIZANDO,
      SINCRONIZANDO_ESCLAVO,
      SINCRONIZACION_maestro,
      transmiciondatos,
      apagado
    };
    /*variable del estado*/
  extern Estado estado;

//cuenta y pasa a el modo esclavo si completa el tiempo adecuado
void sincronizado();
void sincronizadoEsclavo();
void sincronizadoMaestro();
#endif
#ifndef transmisiondatos_h
#define transmisiondatos_h
  extern char banderaInicio;
  extern char banderaFinal;
#include <RH_ASK.h>//RadioHead
#include <SPI.h>
extern RH_ASK rf_driver;
  /*transmite datos atrabes de tx*/
  //resive la variable mensaje para posteriormente transmitirla
  void transmitirMensaje(const char* mensaje);

  /*recibe los datos de parte de algun dispositivo*/
  //resive y guarda los datos en un 
  void recibirMensaje();

  /*crea una trama de datos y remite esos datos*/
  /*crea tambien lo que es el bit de paridad entre nombreDispositivo, nombreDispositivoRemitente y mensaje*/
  /*-->banderaInicio para identificar el inicio de los datos*/
  /*-->nombreDispositivo nombre de quien envia el mensaje*/
  /*-->nombreDispositivoRemitente nombre de ha quien va dirigido el mensaje*/
  /*-->mensaje mensaje al remitente*/
  /*-->banderaFinal para saber el final del mensaje*/
  void crearMensaje(char banderaInicio, char nombreDispositivo, char nombreDispositivoRemitente, const char* mensaje, char banderaFinal);

#endif
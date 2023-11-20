#ifndef transmisiondatos_h
#define transmisiondatos_h

  #include <RH_ASK.h>//con libreria RadioHead
  #include <SPI.h>

  extern char banderaInicio;
  extern char banderaFinal;

  extern char re_nombreDispositivo;
  extern char re_nombreDispositivoRemitente;
  extern char re_mensaje[20];

  #define PIN_TRANSMIT 2 // Cambia 12 al número de pin que desees para la transmisión
  #define PIN_RECEIVE 4  // Cambia 11 al número de pin que desees para la recepción

// speed = 2000, uint8_t rxPin = 11, uint8_t txPin = 12
extern RH_ASK rf_driver;

  void declaracion_transmisiondatos();

  /*transmite datos atrabes de tx*/
  //resive la variable mensaje para posteriormente transmitirla
  void transmitirMensaje(const char* mensaje);

  /*crea una trama de datos y remite esos datos*/
  /*crea tambien lo que es el bit de paridad entre nombreDispositivo, nombreDispositivoRemitente y mensaje*/
  /*-->banderaInicio para identificar el inicio de los datos*/
  /*-->nombreDispositivo nombre de quien envia el mensaje*/
  /*-->nombreDispositivoRemitente nombre de ha quien va dirigido el mensaje*/
  /*-->mensaje mensaje al remitente*/
  /*-->banderaFinal para saber el final del mensaje*/
  void crearMensaje(char banderaInicio, char nombreDispositivo, char nombreDispositivoRemitente, const char* mensaje, char banderaFinal);

  /*recibe los datos de parte de algun dispositivo*/
  //resive y guarda los datos en un 
  void recibirMensaje(char retona = '0');

  //transmite lo que quiera que este dentro la funcion
  void prueva_tx_transmisiondatos(char *mensaje);

  //recive y imprime lo que llege del sensor 433mhz y lo imprime en la consola
  void prueva_rx_transmisiondatos();

  /*tiene dos casos*/
  /*caso 1: manda señal para conectarse y espera respuesta*/
  /*caso 2: recive la señal del otro dispocitivo y emite para conestar*/
  void prueva_conectividad_transmisiondatos(int estado);

#endif
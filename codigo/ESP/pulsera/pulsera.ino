#include "transmisiondatos.h"
#include "leds.h"
#include "boton.h"
#include "maquinaEstados.h"

//RH_ASK rf_driver;
// Variables
char re_nombreDispositivo = '0';
char re_nombreDispositivoRemitente ='0';
char re_mensaje[20];

bool enModoMaestro = false;
unsigned long ultimoTiempoBoton = 0;
unsigned long inicioSincronizacion = 0;
char nombreDispositivo = 'A'; // Nombre del dispositivo (cámbialo según sea necesario)
char banderaInicio = '#'; // Caracter de inicio de mensaje
char banderaFinal = '$'; // Caracter de final de mensaje
char nombreDisp_conec[20];
unsigned long tiempoAnterior; // Tiempo en milisegundos para entrar en modo esclavo
bool encendido = false;

unsigned long tiempoSincronizacion; // Tiempo en milisegundos para entrar en modo esclavo
bool encendidoSincronizacion = false;
bool esclavo = true;

Estado estado;

void setup()
{
  declaracion_transmisiondatos();
  declarar_leds();
  pinMode(botonPin, INPUT);
}


void loop() {
  switch (estado) {

    case ENCENDIDO:
      encenderLEDS(1000, 3);
      LEDSoff();
      estado = SINCRONIZANDO;
      break;
      
    case  SINCRONIZANDO:
      sincronizado();
      break;

    case  SINCRONIZANDO_ESCLAVO:
      sincronizadoEsclavo();
      break;

    case  SINCRONIZACION_maestro:
      sincronizadoMaestro();
      break;
    
    case transmiciondatos:
      break;

    case apagado:
      break;
  }
}
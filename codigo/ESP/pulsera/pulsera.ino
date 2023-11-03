#include "transmisiondatos.h"
#include "leds.h"
#include "boton.h"
#include "transmisiondatos.h"
#include "maquinaEstados.h"

RH_ASK rf_driver;
// Variables
bool enModoSincronizacion = false;
bool enModoMaestro = false;
unsigned long ultimoTiempoBoton = 0;
unsigned long inicioSincronizacion = 0;
enum Estado { ENCENDIDO, SINCRONIZANDO, SINCRONIZANDO_ESCLAVO, SINCRONIZACION_maestro, transmiciondatos, apagado };
Estado estado = ENCENDIDO;
char nombreDispositivo = 'A'; // Nombre del dispositivo (cámbialo según sea necesario)
char mensaje[20] = ""; // Mensaje a transmitir
char banderaInicio = '#'; // Caracter de inicio de mensaje
char banderaFinal = '$'; // Caracter de final de mensaje

// Constantes de tiempo
unsigned long tiempoParaEsclavo = 3000; // Tiempo en milisegundos para entrar en modo esclavo
unsigned long tiempoParaMaestro = 6000; // Tiempo en milisegundos para entrar en modo maestro
unsigned long tiempoParaApagado = 9000; // Tiempo en milisegundos para apagar

void setup()
{
  declarar_leds();
  if (!rf_driver.init())
  {
    Serial.println("RadioHead no se pudo inicializar.");
  }
  // Configura el pin de transmisión en el constructor
  RH_ASK rf_driver(2000, 10); // Velocidad de transmisión (2000 bps), pin de transmisión (cambia el número según tu configuración)
  pinMode(botonPin, INPUT_PULLUP);
}


void loop() {
  unsigned long tiempoActual = millis();
  void mian_maquinaEstados();
}
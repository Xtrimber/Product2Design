#include "transmisiondatos.h"
#include "leds.h"

RH_ASK rf_driver;

// Definición de pines
const int ledAlertaPin = 2;
const int ledSincronizacionPin = 3;
const int botonPin = 4;

// Variables
bool enModoSincronizacion = false;
bool enModoMaestro = false;
unsigned long ultimoTiempoBoton = 0;
unsigned long inicioSincronizacion = 0;
enum Estado { NORMAL, MODO_MAESTRO, MODO_ESCLAVO, MODO_SINCRONIZACION };
Estado estado = NORMAL;

char nombreDispositivo = 'A'; // Nombre del dispositivo (cámbialo según sea necesario)
char mensaje[20] = ""; // Mensaje a transmitir
char banderaInicio = '#'; // Caracter de inicio de mensaje
char banderaFinal = '$'; // Caracter de final de mensaje

// Constantes de tiempo
unsigned long tiempoParaEsclavo = 5000; // Tiempo en milisegundos para entrar en modo esclavo
unsigned long tiempoParaMaestro = 10000; // Tiempo en milisegundos para entrar en modo maestro
unsigned long tiempoParaApagado = 15000; // Tiempo en milisegundos para apagar

void setup() {
  pinMode(ledAlertaPin, OUTPUT);
  pinMode(ledSincronizacionPin, OUTPUT);
  pinMode(botonPin, INPUT_PULLUP);
  if (!rf_driver.init()) {
    Serial.println("RadioHead no se pudo inicializar.");
  }
  // Configura el pin de transmisión en el constructor
  RH_ASK rf_driver(2000, 10); // Velocidad de transmisión (2000 bps), pin de transmisión (cambia el número según tu configuración)
}


void loop() {
  unsigned long tiempoActual = millis();
  
  switch (estado) {
    case NORMAL:
      if (digitalRead(botonPin) == LOW) {
        if (tiempoActual - ultimoTiempoBoton > 50) {
          estado = enModoSincronizacion ? MODO_SINCRONIZACION : (enModoMaestro ? MODO_MAESTRO : MODO_ESCLAVO);
          ultimoTiempoBoton = tiempoActual;
        }
      }
      break;
      
    case MODO_MAESTRO:
      // Código para modo maestro
      // ...
      break;

    case MODO_ESCLAVO:
      // Código para modo esclavo
      // ...
      break;

    case MODO_SINCRONIZACION:
      // Código para modo de sincronización
      // ...
      break;
  }
}
#include "leds.h"
#include <Arduino.h>

void encenderLED(int pin, int veces)
{
  unsigned long tiempoActual = millis();
  static unsigned long tiempoAnterior = 0;
  static bool encendido = false;

  if (tiempoActual - tiempoAnterior >= 1000)
  {
    tiempoAnterior = tiempoActual;
    encendido = !encendido;
    digitalWrite(pin, encendido ? HIGH : LOW);
    veces--;
  }
}

void declarar_leds()
{
  pinMode(ledAlertaPin, OUTPUT);
  pinMode(ledSincronizacionPin, OUTPUT);
}
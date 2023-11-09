#include "leds.h"
#include <Arduino.h>

void encenderLED(int pin, int tiempo)
{
  if ((millis() - tiempoAnterior) >= tiempo)
  {
    tiempoAnterior = millis();
    encendido = !encendido;
    digitalWrite(pin, encendido ? HIGH : LOW);
  }
}

void encenderLEDS(int tiempo, int i = 3)
{
  while(i > 0)
  {
    if ((millis() - tiempoAnterior) >= tiempo)
    {
      tiempoAnterior = millis();
      encendido = !encendido;
      digitalWrite(ledAlertaPin, encendido ? HIGH : LOW);
      digitalWrite(ledSincronizacionPin, digitalRead(ledAlertaPin));
      i = i-1;
    }
  }
  
}

void LEDSoff()
{
  digitalWrite(ledAlertaPin, LOW);
  digitalWrite(ledSincronizacionPin, LOW);
}

void declarar_leds()
{
  pinMode(ledAlertaPin, OUTPUT);
  pinMode(ledSincronizacionPin, OUTPUT);
}
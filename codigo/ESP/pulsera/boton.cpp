#include "boton.h"
#include <Arduino.h>
#include "leds.h"

void declarar_boton()
{
  pinMode(botonPin, INPUT);
}

void prueva_boton()
{
  if(digitalRead(botonPin) == 0)
  {
    Serial.println("seleccionado");
    Serial.print("boton en: ");
    Serial.println(digitalRead(botonPin));
    digitalWrite(ledAlertaPin, LOW);
    digitalWrite(ledSincronizacionPin, HIGH);
    delay(1000);
  }
  else
  {
    Serial.println("no seleccionado");
    Serial.print("boton en: ");
    Serial.println(digitalRead(botonPin));
    digitalWrite(ledAlertaPin, HIGH);
    digitalWrite(ledSincronizacionPin, LOW);
    delay(1000);
  }
}
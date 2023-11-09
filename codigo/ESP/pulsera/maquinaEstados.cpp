#include "maquinaEstados.h"
#include <Arduino.h>

void sincronizado()
  {
    tiempoSincronizacion = millis();
    while((digitalRead(botonPin)==1) && (encendidoSincronizacion == false))
    {
      if((millis() - tiempoSincronizacion) >= tiempoParaEsclavo)
      {
        estado = SINCRONIZANDO_ESCLAVO;
        encendidoSincronizacion = true;
      }
    }
  }


void sincronizadoEsclavo()
  {
    //mientras seleccione el boton la cantidad de tiempo adecuada se mantrendra en esclavo, sino lo manda a maestro
    while((digitalRead(botonPin)==1) && (encendidoSincronizacion == true))
    {
      encenderLED(ledSincronizacionPin, 500);
      if(((millis() - tiempoSincronizacion) >= tiempoParaMaestro))
      {
        estado = SINCRONIZACION_maestro;
        esclavo = false;
        encendidoSincronizacion = false;
        LEDSoff();
      }
      estado = transmiciondatos;
    }
    //sincronisacion con maestro
    while((encendidoSincronizacion == false) && (esclavo = true))
    {
      encenderLED(ledSincronizacionPin, 500);
      bool conectado=false;
      //envia mensaje para conectar
      crearMensaje(banderaInicio, '0', nombreDispositivo, "conectar", banderaFinal);
      //mantiene aqui asata resivir respuesta
      tiempoSincronizacion=millis();
      while(conectado==true)
      {
        encenderLED(ledSincronizacionPin, 500);
        recibirMensaje();
        //si recive respuesta del maestro por su nombre sale de esperar respuesta 
        if((re_mensaje[20] == "conectado") && (re_nombreDispositivo == nombreDispositivo))
        {
          estado = transmiciondatos;
          nombreDisp_conec[0] = re_nombreDispositivoRemitente;
          conectado=false;
          encendidoSincronizacion=true;
          encenderLEDS(500, 8);
          crearMensaje(banderaInicio, nombreDisp_conec[0], nombreDispositivo, "confirmado", banderaFinal);
        }
        //si recive error de parte del maestro remite para volver a mandar señal
        else if((re_mensaje[20] == "error") || ((millis() - tiempoSincronizacion) >= 500))
        {
          conectado=false;
        }
      }
    }
     LEDSoff();
  }
void sincronizadoMaestro()
  {
    LEDSoff();
    //si mantiene el boton oprimido lo manda a apagar
    while((digitalRead(botonPin)==1) && (encendidoSincronizacion == false))
    {
      encenderLED(ledAlertaPin, 500);
      //mada a apagar si se supera el tiempo
      if((millis() - tiempoSincronizacion) >= tiempoParaApagado)
      {
        estado = apagado;
        encendidoSincronizacion = true;
        LEDSoff();
      }
      // sale para iniciar sincronizacion
      else 
      {
        estado = transmiciondatos;
        encendidoSincronizacion = false;
      }
    }
    //inicio de sincronizacion para maestro
    while(encendidoSincronizacion == true)
    {
      char puesto = 0;
      bool conectado=false;
      encenderLED(ledAlertaPin, 500);
      //para esperar mensaje de sincronizacion
      while(conectado==false)
      {
        encenderLED(ledAlertaPin, 500);
        recibirMensaje();
        //si recive mensaje de sincronizacion y no tienen a quien se envia
        //guarda el remitente y espera otro para conectar
        if((re_mensaje[20] == "conectar") && (re_nombreDispositivo == '0'))
        {
          estado = transmiciondatos;
          nombreDisp_conec[puesto] = re_nombreDispositivoRemitente;
          crearMensaje(banderaInicio, nombreDisp_conec[puesto], nombreDispositivo, "conectado", banderaFinal);
          puesto = puesto +1;
        }
        tiempoSincronizacion = millis();
        //si se preciona el boton misma cantidad de tiempo permite salir del modo maestro
        while(digitalRead(botonPin)==1)
        {
          if((millis() - tiempoSincronizacion) >= tiempoParaMaestro)
          {
            encenderLEDS(500, 8);
            LEDSoff();
            encendidoSincronizacion = false;
            conectado = true;
            break;
          }
        }
      }
    }
     LEDSoff();
  }
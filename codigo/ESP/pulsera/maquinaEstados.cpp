#include "maquinaEstados.h"
#include <Arduino.h>

void sincronizado()
  {
    tiempoSincronizacion = millis();
    while((digitalRead(botonPin)==0) && (encendidoSincronizacion == false))
    {
      if((millis() - tiempoSincronizacion) >= tiempoParaEsclavo)
      {
        estado = SINCRONIZANDO_ESCLAVO;
        encendidoSincronizacion = true;
        flagInterrupcion = false;
      }
    }
  }

void sincronizadoEsclavo()
  {
    //mientras seleccione el boton la cantidad de tiempo adecuada se mantrendra en esclavo, sino lo manda a maestro
    while((digitalRead(botonPin)==0) && (encendidoSincronizacion == true))
    {
      digitalWrite(ledSincronizacionPin, HIGH);
      if(((millis() - tiempoSincronizacion) >= tiempoParaMaestro))
      {
        estado = SINCRONIZACION_maestro;
        esclavo = false;
        encendidoSincronizacion = false;
        flagInterrupcion = false;
        LEDSoff();
      }
      else
      {
        flagInterrupcion = false;
        estado = transmiciondatos;
      }
    }
    LEDSoff();
    //sincronisacion con maestro
    while((encendidoSincronizacion == true) && (esclavo = true))
    {
      encenderLED(ledSincronizacionPin, 500);
      bool conectado=false;
      //envia mensaje para conectar
      crearMensaje(banderaInicio, '0', nombreDispositivo, "conectar", banderaFinal);
      tiempoSincronizacion=millis();//reinicia el tiempo para volver a mandar mensaje
      //mantiene aqui asata resivir respuesta
      conectado = true;
      while(conectado==true)
      {
        encenderLED(ledSincronizacionPin, 500);
        recibirMensaje();
        //si recive respuesta del maestro por su nombre sale de esperar respuesta 
        if((strcmp(re_mensaje, "conectado") == 0) && (strcmp(re_nombreDispositivo, nombreDispositivo)==0))
        {
          conectado=false;
          encendidoSincronizacion=false;
          estado = transmiciondatos;
          LEDSoff();
          nombreDisp_conec[0] = re_nombreDispositivoRemitente;
          crearMensaje(banderaInicio, nombreDisp_conec[0], nombreDispositivo, "confirmado", banderaFinal);
          encenderLEDS(500, 2);
        }
        //si recive error de parte del maestro remite para volver a mandar señal
        else if((millis() - tiempoSincronizacion) >= 500)
        {
          conectado=false;
        }
        tiempoAnterior = millis();
        while(digitalRead(botonPin) == 0)
        {
          
          if((millis() - tiempoAnterior) >= 1000)
          {
            flagInterrupcion = false;
            conectado = false;
            encendidoSincronizacion = false;
            estado = apagado;
          }
        }
      }
    }
    LEDSoff();
  }

void sincronizadoMaestro()
  {
    LEDSoff();
    enModoMaestro = true;
    digitalWrite(ledAlertaPin, HIGH);
    //si mantiene el boton oprimido lo manda a apagar
    while((digitalRead(botonPin) == 0) && (encendidoSincronizacion == false))
    {
      //mada a apagar si se supera el tiempo
      if((millis() - tiempoSincronizacion) >= tiempoParaApagado)
      {
        flagInterrupcion = false;
        estado = apagado;
        encendidoSincronizacion = true;
        enModoMaestro = false;
        LEDSoff();
      }
      // sale para iniciar sincronizacion
      else 
      {
        enModoMaestro = true;
        estado = transmiciondatos;
      }
    }
    LEDSoff();
    //inicio de sincronizacion para maestro
    int posicion = 0;
    while(encendidoSincronizacion == false)
    {
      bool con=true;
      while(con ==true)
      {
        encenderLED(ledAlertaPin, 500);
        recibirMensaje();
        Serial.println("esperando para conectar");
        //si recive mensaje de sincronizacion y no tienen a quien se envia
        //guarda el remitente y espera otro para conectar
        tiempoSincronizacion = millis();
        while(strcmp(re_mensaje, "conectar") == 0)
        { 
          nombreDisp_conec[posicion] = re_nombreDispositivoRemitente;
          crearMensaje(banderaInicio, nombreDisp_conec[posicion], nombreDispositivo, "conectado", banderaFinal);
          Serial.println((millis() - tiempoSincronizacion));
          if((millis() - tiempoSincronizacion) >= 3000)
          {
            recibirMensaje();
            memset(re_mensaje, 0, sizeof(re_mensaje));
            con = false;
          }
          recibirMensaje();
          
        }
        tiempoSincronizacion = millis();
        //si se preciona el boton misma cantidad de tiempo permite salir del modo maestro
        while(digitalRead(botonPin)==0)
          {
            if((millis() - tiempoSincronizacion) >= 1000)
            {
              flagInterrupcion = false;
              encenderLEDS(500, 8);
              LEDSoff();
              encendidoSincronizacion = true;
              estado = transmiciondatos;
              con=false;
            }
          }
      }
      encenderLED(ledAlertaPin, 500);
      estado = transmiciondatos;
      if(posicion < 9)
      {
        memset(re_mensaje, 0, sizeof(re_mensaje));
        Serial.print("suma 1: ");
        Serial.println(posicion);
        posicion = posicion + 1;
      }
      else
      {
        encenderLEDS(500, 8);
        LEDSoff();
        encendidoSincronizacion = true;
      }
      tiempoSincronizacion = millis();
      //si se preciona el boton misma cantidad de tiempo permite salir del modo maestro
      while(digitalRead(botonPin)==0)
        {
          if((millis() - tiempoSincronizacion) >= 1000)
          {
            flagInterrupcion = false;
            encenderLEDS(500, 8);
            LEDSoff();
            con=false;
            encendidoSincronizacion = true;
            estado = transmiciondatos;
            Serial.print("puestos: ");
            puesto = posicion-1;
            Serial.println(puesto);
            
          }
        }
      }
    LEDSoff();
  }
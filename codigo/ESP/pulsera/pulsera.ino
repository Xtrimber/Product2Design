#include "transmisiondatos.h"
#include "leds.h"
#include "boton.h"
#include "maquinaEstados.h"

RH_ASK rf_driver(2000, PIN_RECEIVE, PIN_TRANSMIT, 14, false);

// Variables
char re_nombreDispositivo = '0';
char re_nombreDispositivoRemitente ='0';
char re_mensaje[20];
char puesto = 0;

bool enModoMaestro = false;
unsigned long ultimoTiempoBoton = 0;
unsigned long inicioSincronizacion = 0;
char nombreDispositivo = 'w'; // Nombre del dispositivo (cámbialo según sea necesario)
char banderaInicio = '#'; // Caracter de inicio de mensaje
char banderaFinal = '$'; // Caracter de final de mensaje
char nombreDisp_conec[20];
unsigned long tiempoAnterior; // Tiempo en milisegundos para entrar en modo esclavo
bool encendido = false;
bool recivido = false;

unsigned long tiempoSincronizacion; // Tiempo en milisegundos para entrar en modo esclavo
bool encendidoSincronizacion = false;
bool esclavo = true;

Estado estado;

void setup()
{
  Serial.begin(9600);
  declaracion_transmisiondatos();
  declarar_leds();
  declarar_boton();
}


void loop() {
  switch (estado) {

    case ENCENDIDO:
      encenderLEDS(500, 3);
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
        Serial.println("transmiciondatos");
        while(esclavo == true)
        {
          Serial.println(" ");
          Serial.print("nombre del maestro: ");
          Serial.println(nombreDisp_conec[0]);
          tiempoAnterior = millis();
          while(digitalRead(botonPin) == 0)
            {
              if((millis() - tiempoAnterior) >= 1000)
              {
                estado = apagado;
              }
            }
        }
        while(enModoMaestro == true)
        {
          for(int x = puesto; x >= 0; x--)
          {
            Serial.println(" ");
            Serial.print(x);
            Serial.print(") nombre del maestro: ");
            Serial.println(nombreDisp_conec[x]);
          }
          tiempoAnterior = millis();
          while(digitalRead(botonPin) == 0)
            {
              if((millis() - tiempoAnterior) >= 1000)
              {
                estado = apagado;
              }
            }
        }
      break;

    case apagado:
    Serial.println("apagado");
      break;
  }

/*----------------------------------------------------pruevas--------------------------------------------------*/
  //pruevas de transmicion
    //nombreDisp_conec[0]='B';
    //crearMensaje(banderaInicio, nombreDispositivo , nombreDisp_conec[0], " se acabo todo se", banderaFinal);
    //recibirMensaje();
    //prueva_tx_transmisiondatos("mueste del dios de la luna y el mar");
    //prueva_rx_transmisiondatos();
    //prueva_conectividad_transmisiondatos(1);

  //pruevas de leds
    //#define ledAlertaPin 12
    //#define ledSincronizacionPin 11
    //encenderLED(ledSincronizacionPin, 1000);
    //encenderLED(ledAlertaPin, 1000);
    //encenderLEDS(1000, 3);

  //prueva boton;
    //prueva_boton();
}
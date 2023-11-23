#include "transmisiondatos.h"
#include "leds.h"
#include "boton.h"
#include "maquinaEstados.h"

RH_ASK rf_driver(2000, PIN_RECEIVE, PIN_TRANSMIT, 14, false);

volatile bool flagInterrupcion = false;

// Variables
char re_nombreDispositivo = '0';
char re_nombreDispositivoRemitente ='0';
char re_mensaje[20];

char puesto = 0;

bool enModoMaestro = false;
unsigned long ultimoTiempoBoton = 0;
unsigned long inicioSincronizacion = 0;
char nombreDispositivo = 'A'; // Nombre del dispositivo (cámbialo según sea necesario)
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
  memset(re_mensaje, 0, sizeof(re_mensaje));
  memset(nombreDisp_conec, 0, sizeof(nombreDisp_conec));
  Serial.begin(9600);
  declaracion_transmisiondatos();
  declarar_leds();
  declarar_boton();
}


void loop() {
  switch (estado) {

    case ENCENDIDO:
      encenderLEDS(500, 3);
      Serial.println("encendido");
      estado = SINCRONIZANDO;
      if(flagInterrupcion==true){Serial.print("hola soy la interupcion");
      flagInterrupcion=false;}
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

/*----------------------esclavo-----------------------*/
        while(esclavo == true)
        {
        
          Serial.println("esclavo");
          
          // funcion normal
          recibirMensaje();
          //pasado de lista del maestro
          if((strcmp(re_mensaje, "revisando") == 0) && (strcmp(re_nombreDispositivo, nombreDispositivo)==0) && (strcmp(re_nombreDispositivoRemitente, nombreDisp_conec[0])==0))
          { 
            int salir = 0;
            crearMensaje(banderaInicio, nombreDisp_conec[0], nombreDispositivo,  "estoy", banderaFinal);
            while((strcmp(re_mensaje, "confirmado") != 0) && (salir < 5))
            {
              salir = salir + 1;
              crearMensaje(banderaInicio, nombreDisp_conec[0], nombreDispositivo,  "estoy", banderaFinal);
            }
          }
          //para reunion con el maestro
          else if((strcmp(re_mensaje, "reunion") == 0) && (strcmp(re_nombreDispositivo, nombreDispositivo)==0) && (strcmp(re_nombreDispositivoRemitente, nombreDisp_conec[0])==0))
          {
            recibirMensaje();
            encenderLED(ledAlertaPin, 500);
          }
          memset(re_mensaje, 0, sizeof(re_mensaje));
          //para apagado    
          tiempoAnterior = millis();
          while(digitalRead(botonPin) == 0)
          {
            if((millis() - tiempoAnterior) >= 2000)
            {
              estado = apagado;
              esclavo = false;                                
              flagInterrupcion = false;
              digitalWrite(ledAlertaPin,HIGH);
              digitalWrite(ledSincronizacionPin,HIGH);
              break;
            }
          }
          //caso de emergencia
          if(flagInterrupcion == true)
          {Serial.print("emergencia");
            encenderLED(ledSincronizacionPin, 500);
            crearMensaje(banderaInicio, nombreDisp_conec[0], nombreDispositivo,  "peligro", banderaFinal);
          }
          //para apagar el caso de emergencia
          while(digitalRead(botonPin) == 0)
          {
            if((millis() - tiempoAnterior) >= 1000)
            {                                 
              flagInterrupcion = false;
              LEDSoff();
            }
          }
          enModoMaestro = false;
        }

/*------------------------MAESTRO-------------------------*/
        tiempoSincronizacion = millis();
        while(enModoMaestro == true)
        {
        Serial.println("maestro");

          //funcion normal
          recibirMensaje();
          if((strcmp(re_mensaje, "peligro") == 0) && (strcmp(re_nombreDispositivo, nombreDispositivo)==0))
          {
            for(int x = puesto; x >=0; x--)
            {
              if(strcmp(re_nombreDispositivoRemitente, nombreDisp_conec[x])==0)
              {
                flagInterrupcion = true;
              encenderLED(ledAlertaPin, 500);
              break;
              }
            }
          }
          
          if((millis()-tiempoSincronizacion) > 2000)
          {
            for(int x = puesto; x >= 0; x--)
            {
              crearMensaje(banderaInicio, nombreDisp_conec[x], nombreDispositivo,  "revisando", banderaFinal);
              int salir = 3;
              while((strcmp(re_mensaje, "estoy") != 0)  && (salir < 20))
              {
                recibirMensaje();
                  salir = salir + 1;
              }
              while((salir < 10) && (strcmp(re_mensaje, "estoy") == 0) && (strcmp(re_nombreDispositivoRemitente, nombreDisp_conec[x])==0) && (strcmp(re_nombreDispositivo, nombreDispositivo)==0))
              {
                crearMensaje(banderaInicio, nombreDisp_conec[x], nombreDispositivo,  "confirmado", banderaFinal);
                salir = salir + 1;
              }
            }
            tiempoSincronizacion = millis();
          }
          //apagado
          tiempoAnterior = millis();
          while(digitalRead(botonPin) == 0)
          {
            if((millis() - tiempoAnterior) >= 2000)
            {
              enModoMaestro = false ;
              estado = apagado;
              flagInterrupcion = false;
              digitalWrite(ledAlertaPin,HIGH);
              digitalWrite(ledSincronizacionPin,HIGH);
            }
          }
          recibirMensaje();
          //selenciona el boton para reunion 
          if(flagInterrupcion == true)
          {
            for(int x = puesto; x >=0; x--)
            {
              encenderLED(ledSincronizacionPin, 500);
              crearMensaje(banderaInicio, nombreDisp_conec[x], nombreDispositivo,  "reunion", banderaFinal);
            }
          }
          //para apagar el caso de reunion
          while(digitalRead(botonPin) == 0)
          {
            if((millis() - tiempoAnterior) >= 1000)
            {     
              crearMensaje(banderaInicio, nombreDisp_conec[0], nombreDispositivo,  "can reunion", banderaFinal);                            
              flagInterrupcion = false;
              LEDSoff();
            }
          }
        }
      LEDSoff();
      break;

    case apagado:
    encenderLEDS(100, 9);
    flagInterrupcion = false;
    estado = ENCENDIDO;
    

      // Variables
      puesto = 0;
      enModoMaestro = false;
      ultimoTiempoBoton = 0;
      inicioSincronizacion = 0;
      tiempoAnterior; // Tiempo en milisegundos para entrar en modo esclavo
      encendido = false;
      recivido = false;
      encendidoSincronizacion = false;
      esclavo = true;
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
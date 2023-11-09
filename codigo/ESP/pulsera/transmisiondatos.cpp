#include "transmisiondatos.h"
#include <Arduino.h>

void declaracion_transmisiondatos()
{
  vw_setup(2000);  // Velocidad de transmisión (2000 bps)
  vw_set_tx_pin(PIN_TRANSMIT);  // Pin de transmisión (ajusta el número según tu configuración)
  vw_set_rx_pin(PIN_RECEIVE);  // Pin de recepción (ajusta el número según tu configuración)
  vw_rx_start();  // Inicializa el receptor
}

void transmitirMensaje(const char* mensaje)
{
  vw_set_tx_pin(PIN_TRANSMIT);
  vw_setup(2000); // Cambia 2000 a la velocidad deseada (en bits por segundo)
  vw_send((uint8_t*)mensaje, strlen(mensaje));
  vw_wait_tx();
}

void crearMensaje(char banderaInicio, char nombreDispositivo, char nombreDispositivoRemitente, const char* mensaje, char banderaFinal) {
  char mensajeCompleto[30]; // Ajusta el tamaño según tus necesidades
  
  // Construye el mensaje completo
  sprintf(mensajeCompleto, "%c%c%c%s%c", banderaInicio, nombreDispositivo, nombreDispositivoRemitente, mensaje, banderaFinal);
  
  // Calcula el bit de paridad
  char bitDeParidad = 0;
  for (int i = 0; mensajeCompleto[i] != '\0'; i++) {
    bitDeParidad ^= mensajeCompleto[i];
  }
  
  // Agrega el bit de paridad al mensaje completo
  mensajeCompleto[strlen(mensajeCompleto)] = bitDeParidad;
  mensajeCompleto[strlen(mensajeCompleto) + 1] = '\0'; // Añade el carácter nulo al final
  
  // Ahora, mensajeCompleto contiene el mensaje completo con bit de paridad
  transmitirMensaje(mensajeCompleto);
}

void recibirMensaje()
{
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  if (vw_get_message(buf, &buflen)) {
    buf[buflen] = '\0'; // Añade un carácter nulo al final para convertirlo en una cadena

    // Verifica si el mensaje contiene las banderas
    char* inicio = strchr((char*)buf, banderaInicio);
    char* final = strchr((char*)buf, banderaFinal);

    if (inicio != NULL && final != NULL) {
      // Calcula la longitud del mensaje sin las banderas
      size_t longitudMensaje = final - inicio - 1;

      // Verifica el bit de paridad (XOR de todos los caracteres)
      char bitDeParidad = buf[0];
      for (size_t i = 1; i < longitudMensaje; i++) {
        bitDeParidad ^= buf[i];
      }

      if (bitDeParidad == buf[longitudMensaje]) {
        // Bit de paridad correcto, desglosa los datos
        re_nombreDispositivo = buf[1];
        re_nombreDispositivoRemitente = buf[2];
        re_mensaje[20];
        strncpy(re_mensaje, (char*)inicio + 1, longitudMensaje - 2);
        re_mensaje[longitudMensaje - 2] = '\0';
      }
      else
      {
        crearMensaje(banderaInicio, '0', '0', "error", banderaFinal);
      }
    }
  }
}

/* //con libreria RadioHead
void transmitirMensaje(const char* mensaje)
 {
  rf_driver.send((uint8_t*)mensaje, strlen(mensaje));
  rf_driver.waitPacketSent();
}

void recibirMensaje()
{
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);

  if (rf_driver.recv(buf, &buflen)) {
    buf[buflen] = '\0'; // Añade un carácter nulo al final para convertirlo en una cadena

    // Verifica si el mensaje contiene las banderas
    char* inicio = strchr((char*)buf, banderaInicio);
    char* final = strchr((char*)buf, banderaFinal);

    if (inicio != NULL && final != NULL) {
      // Calcula la longitud del mensaje sin las banderas
      size_t longitudMensaje = final - inicio - 1;

      // Verifica el bit de paridad (XOR de todos los caracteres)
      char bitDeParidad = buf[0];
      for (size_t i = 1; i < longitudMensaje; i++) {
        bitDeParidad ^= buf[i];
      }

      if (bitDeParidad == buf[longitudMensaje]) {
        // Bit de paridad correcto, desglosa los datos
        char nombreDispositivo = buf[1];
        char nombreDispositivoRemitente = buf[2];
        char mensaje[20];
        strncpy(mensaje, (char*)inicio + 1, longitudMensaje - 2);
        mensaje[longitudMensaje - 2] = '\0';
      }
    }
  }
}

void crearMensaje(char banderaInicio, char nombreDispositivo, char nombreDispositivoRemitente, const char* mensaje, char banderaFinal) {
  char mensajeCompleto[30]; // Ajusta el tamaño según tus necesidades
  
  // Construye el mensaje completo
  sprintf(mensajeCompleto, "%c%c%c%s%c", banderaInicio, nombreDispositivo, nombreDispositivoRemitente, mensaje, banderaFinal);
  
  // Calcula el bit de paridad
  char bitDeParidad = 0;
  for (int i = 0; mensajeCompleto[i] != '\0'; i++) {
    bitDeParidad ^= mensajeCompleto[i];
  }
  
  // Agrega el bit de paridad al mensaje completo
  mensajeCompleto[strlen(mensajeCompleto)] = bitDeParidad;
  mensajeCompleto[strlen(mensajeCompleto) + 1] = '\0'; // Añade el carácter nulo al final
  
  // Ahora, mensajeCompleto contiene el mensaje completo con bit de paridad
  transmitirMensaje(mensajeCompleto);
}*/
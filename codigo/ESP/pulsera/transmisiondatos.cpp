#include "transmisiondatos.h"
#include <Arduino.h>

void declaracion_transmisiondatos()
{
  if (!rf_driver.init()) {
    Serial.println("Error al inicializar el módulo de RF");
    while (1);
  }
}

void transmitirMensaje(const char* mensaje)
{ 
  // Transmisión
  rf_driver.send((uint8_t *)mensaje, strlen(mensaje));
  rf_driver.waitPacketSent();
  Serial.print("Mensaje enviado:");
  Serial.println(mensaje);
  delay(1000);
}

void crearMensaje(char banderaInicio, char nombreDispositivo, char nombreDispositivoRemitente, const char* mensaje, char banderaFinal) {
  
  char mensajeCompleto[30]; // Ajusta el tamaño según tus necesidades
  memset(mensajeCompleto, 0, sizeof(mensajeCompleto));//reinicia
  
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

void recibirMensaje(char retona = '0') {
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);
  memset(buf, 0, sizeof(buf));

  // Paso 1: Recibir el mensaje desde el módulo RF
  if (rf_driver.recv(buf, &buflen)) {
    // Paso 2: Convierte el búfer en una cadena de caracteres antes de procesar
    char mensajeStr[RH_ASK_MAX_MESSAGE_LEN];
    strncpy(mensajeStr, (char*)buf, buflen);
    mensajeStr[buflen] = '\0';

    // Variables para almacenar los componentes del mensaje
    char re_banderaInicio;
    extern char nombreDispositivo;
    extern char re_nombreDispositivo;
    extern char re_nombreDispositivoRemitente;
    extern char re_mensaje[20]; // Ajusta el tamaño según tus necesidades
    char re_banderaFinal;
    char bitDeParidadRecibido;
    extern bool recivido;
    memset(re_mensaje, 0, sizeof(re_mensaje));

    // Paso 3: Separar el mensaje en componentes
    sscanf(mensajeStr, "%c%c%c%19[^$]%c%c", &re_banderaInicio, &re_nombreDispositivo, &re_nombreDispositivoRemitente, re_mensaje, &re_banderaFinal, &bitDeParidadRecibido);

        
    // Paso 4: Verificar las banderas de inicio y final
    if (banderaInicio == re_banderaInicio && banderaFinal == re_banderaFinal) {
      Serial.println("Correctas banderas de inicio y/o final");
        /*Serial.println(mensajeStr);
        Serial.print("banderaInicio: ");
        Serial.println(re_banderaInicio);
        Serial.print("Nombre Dispositivo: ");
        Serial.println(re_nombreDispositivo);
        Serial.print("Nombre Dispositivo Remitente: ");
        Serial.println(re_nombreDispositivoRemitente);
        Serial.print("Mensaje: ");
        Serial.println(re_mensaje);
        Serial.print("banderaFinal: ");
        Serial.println(re_banderaFinal);
        Serial.print("bitDeParidadRecibido: ");
        Serial.println(bitDeParidadRecibido);
        Serial.println(" ");*/

      // Paso 5: Realizar el cálculo del bit de paridad
      char mensajeCompleto[30]; // Ajusta el tamaño según tus necesidades
      memset(mensajeCompleto, 0, sizeof(mensajeCompleto));//reinicia
      sprintf(mensajeCompleto, "%c%c%c%s%c", re_banderaInicio, re_nombreDispositivo, re_nombreDispositivoRemitente, re_mensaje, re_banderaFinal);

      char bitDeParidadCalculado = mensajeCompleto[0];
      for (int i = 1; mensajeCompleto[i] != '\0'; i++) {
        bitDeParidadCalculado ^= mensajeCompleto[i];
      }

      /*Serial.print("calculado: ");
      Serial.println(bitDeParidadCalculado);
      Serial.print("recivido: ");
      Serial.println(bitDeParidadRecibido);*/

      // Paso 6: Verificar el bit de paridad
      if (bitDeParidadRecibido == bitDeParidadCalculado) {
        // El bit de paridad es correcto, puedes utilizar los componentes del mensaje
        Serial.println("Bit de paridad correcto");
        /*Serial.print("Nombre Dispositivo: ");
        Serial.println(re_nombreDispositivo);
        Serial.print("Nombre Dispositivo Remitente: ");
        Serial.println(re_nombreDispositivoRemitente);
        Serial.print("Mensaje: ");
        Serial.println(re_mensaje);*/
        recivido = true;
      } else {
        // Error en el bit de paridad
        Serial.println("Error en el bit de paridad");
        crearMensaje(banderaInicio, nombreDispositivo , retona, "error", banderaFinal);
        recivido = false;
      }
    } else {
      // Banderas incorrectas
      crearMensaje(banderaInicio, nombreDispositivo , retona, "error", banderaFinal);
      Serial.println("Error en las banderas de inicio y/o final");
      recivido = false;
    }
  }
}

void prueva_tx_transmisiondatos(char *mensaje)
{
  // Transmisión
  rf_driver.send((uint8_t *)mensaje, strlen(mensaje));
  rf_driver.waitPacketSent();
  Serial.print("Mensaje enviado:");
  Serial.println(mensaje);
  delay(1000);
}

void prueva_rx_transmisiondatos()
{
  // Recepción
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);
  memset(buf, 0, sizeof(buf));//reinicia la memoria del mensaje
  if (rf_driver.recv(buf, &buflen))
  {
    Serial.print("Mensaje recibido: ");
    Serial.println((char *)buf);
  }
  delay(1000);
}

void prueva_conectividad_transmisiondatos(int x) {
  extern char nombreDispositivo;
  extern char banderaInicio;
  extern char banderaFinal;

  switch (x) {

    case 1:
    {
      crearMensaje(banderaInicio, nombreDispositivo, '0', "conectar", banderaFinal);
      extern bool recivido;
      int conteo = 0;
      recivido = true;
      Serial.print("Conectando");
      while (recivido) {
        conteo = conteo + 1;
        Serial.print(".");
        delay(50);
        if (conteo == 30) {
          recivido = false;
          Serial.println(" ");
        }

        recibirMensaje();
        while(strcmp(re_mensaje, "conectado")) {
          Serial.println("Conectado con éxito");
          crearMensaje(banderaInicio, nombreDispositivo, '0', "confirmado", banderaFinal);
        }
      }
    }
    break;

    case 2:
    {
      Serial.println("Enviando solicitud de conexión");
      recibirMensaje();
      bool conectado = false;
      while (strcmp(re_mensaje, "conectar") || conectado == true)
      {
        conectado = true;
        Serial.println("Se envió señal para conectar");
        crearMensaje(banderaInicio, nombreDispositivo, '0', "conectado", banderaFinal);
        recibirMensaje();
        while(strcmp(re_mensaje, "confirmado")) {
          Serial.println("Conectados");
          delay(1000);
        }
      }
    }
    break;
  }
}

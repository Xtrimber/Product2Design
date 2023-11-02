#ifndef transmisiondatos_h
#define transmisiondatos_h

#include <RH_ASK.h>
#include <SPI.h>

extern RH_ASK rf_driver;

extern char banderaInicio;
extern char banderaFinal;

/*transmite datos atrabes de tx*/
//resive la variable mensaje para posteriormente transmitirla
void transmitirMensaje(const char* mensaje) {
  rf_driver.send((uint8_t*)mensaje, strlen(mensaje));
  rf_driver.waitPacketSent();
}

/*recibe los datos de parte de algun dispositivo*/
//resive y guarda los datos en un 
void recibirMensaje() {
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

/*crea una trama de datos y remite esos datos*/
/*crea tambien lo que es el bit de paridad entre nombreDispositivo, nombreDispositivoRemitente y mensaje*/
/*-->banderaInicio para identificar el inicio de los datos*/
/*-->nombreDispositivo nombre de quien envia el mensaje*/
/*-->nombreDispositivoRemitente nombre de ha quien va dirigido el mensaje*/
/*-->mensaje mensaje al remitente*/
/*-->banderaFinal para saber el final del mensaje*/
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
#endif
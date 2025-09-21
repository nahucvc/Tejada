#ifndef CODIGO_H
#define CODIGO_H
#include "RespuestaServidor.h"

AsyncWebServer server(80); // creacion del servidor

void notFound(AsyncWebServerRequest *request); // Respuesta del servidor cuando no encuentra la dirrección
void IniciarServidor(void);
AsyncWebSocket ws("/ws"); // se crea el socket
void EventosSockets(AsyncWebSocket *server, AsyncWebSocketClient *cliente, AwsEventType evento, void *arg, uint8_t *datos, size_t len); //funcion que analiza el tipo de evento del socket y llama a la funcion correspondiente  

#endif
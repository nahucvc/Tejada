#include "codigo.h"


void notFound(AsyncWebServerRequest *request)
{
    request->send(404, "text/plain", "Not found");
}


void IniciarServidor(void)
{

    if (!SPIFFS.begin(true))
    {
        Serial.println("A ocurrido un error al montando SPIFFS");
        return;
    }
    // Se enruta las solicitudes del servidor , tambien se puede ver mas tipo de contenido en https://www.iana.org/assignments/media-types/media-types.xhtml
    server.on("/", [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/joystick.html", "text/html"); });

    server.on("/styles.css", [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/styles.css", "text/css"); });

    server.on("/script.js", [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/script.js", "text/plain"); });
    server.onNotFound(notFound);
    server.begin();
    ws.onEvent(EventosSockets);
    server.addHandler(&ws);
}



void EventosSockets(AsyncWebSocket *server, AsyncWebSocketClient *cliente, AwsEventType evento, void *arg, uint8_t *datos, size_t len)
{
    switch (evento)
    {
    case WS_EVT_CONNECT:
        Serial.printf("cliente WebSocket nro #%u connected from %s\n", cliente->id(), cliente->remoteIP().toString().c_str());

        break;
    case WS_EVT_DISCONNECT:
        aceleracion=0;
        Serial.printf("cliente Nro %u desconectado\n", cliente->id());
        break;
    case WS_EVT_DATA:
        Recibirdatos(arg, datos, len);
        break;
    }
}
#include "codigo.h"


void notFound(AsyncWebServerRequest *request)
{
    request->send(404, "text/plain", "Not found");
}


void IniciarServidor(void)
{


    if (!SPIFFS.begin(true))
    {
        Serial.println("Ha ocurrido un error al montar SPIFFS");
        return;
    }

    // Publicar la carpeta completa en /Telemetry_files
    server.serveStatic("/Telemetry_files", SPIFFS, "/Telemetry_files");



    server.on("/", [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/Telemetry.html", "text/html"); });

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
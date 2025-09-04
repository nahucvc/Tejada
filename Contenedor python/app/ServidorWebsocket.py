# app.py  (tu archivo FastAPI)
import json
import asyncio
from typing import Set, Dict, Any

from fastapi import FastAPI, WebSocket, WebSocketDisconnect
from fastapi.responses import HTMLResponse
from gui_dashboard import start_dashboard, set_angle, set_ac


app = FastAPI()

active_sockets: Set[WebSocket] = set()
client_state: Dict[WebSocket, Dict[str, Any]] = {}


@app.on_event("startup")
async def _start_gui():
    # Levanta la GUI (o headless) en un hilo aparte
    start_dashboard()


@app.get("/")
async def root():
    return HTMLResponse("<h3> WS server OK</h3><p>Conectá a <code>ws://HOST/ws</code></p>")


@app.websocket("/ws")
async def websocket_endpoint(ws: WebSocket):
    await ws.accept()
    active_sockets.add(ws)
    client_state[ws] = {"angle": None, "ac": 0.0, "last": None}

    try:
        await ws.send_text(json.dumps({"encendido": True}))

        while True:
            msg_text = await ws.receive_text()
            try:
                data = json.loads(msg_text)
                client_state[ws]["last"] = data
            except json.JSONDecodeError:
                await ws.send_text(json.dumps({"type": "error", "error": "JSON inválido"}))
                continue

            # 1) Handshake {en:1} (si querés responder algo, hacelo acá)
            if isinstance(data, dict) and data.get("en") == 1:
                # Ej: enviar ack si querés
                # await ws.send_text(json.dumps({"type":"ack","cmd":"en","ok":True}))
                continue

            # 2) Joystick: {angle:<grados>}
            if isinstance(data, dict) and "angle" in data:
                try:
                    angle = float(data["angle"])
                    client_state[ws]["angle"] = angle
                    print("Ángulo:", angle)
                    set_angle(angle)  # 👈 actualiza GUI
                except (TypeError, ValueError):
                    await ws.send_text(json.dumps({"type": "error", "error": "angle debe ser numérico"}))
                continue

            # 3) Aceleración: {ac:<0..100>}
            if isinstance(data, dict) and "ac" in data:
                try:
                    ac = float(data["ac"])
                    print("Acelerador:", ac)
                    client_state[ws]["ac"] = ac
                    set_ac(ac)        # 👈 actualiza GUI
                except (TypeError, ValueError):
                    await ws.send_text(json.dumps({"type": "error", "error": "ac debe ser numérico"}))
                continue

            # Echo/debug para lo demás
            await ws.send_text(json.dumps({"type": "echo", "data": data}))
            await asyncio.sleep(0)

    except WebSocketDisconnect:
        pass
    finally:
        active_sockets.discard(ws)
        client_state.pop(ws, None)

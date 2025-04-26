from flask import Flask, send_from_directory
from flask_socketio import SocketIO, emit
import time
import socketio
from com import Hilo


# variables globales 
app = Flask(__name__)
socketio_ = SocketIO(app)
sio = socketio.SimpleClient()
cordenada:None
contador=0
# Ruta para la página principal
@app.route("/")
def home():
    return "<h1>¡Bienvenido a tu servidor web en Python!</h1>"

# Ruta para servir el archivo auto.html desde la carpeta 'data'
@app.route("/control-manual")
def control_manual():
    return send_from_directory(directory="data", path="joystick.html")

@app.route("/styles.css")
def server_style():
    return send_from_directory(directory="data", path="styles.css")

@app.route("/script.js")
def server_script():
    return send_from_directory(directory="data", path="script.js")

@app.route("/socket.io.min.js")
def server_script_io():
    return send_from_directory(directory="data", path="socket.io.min.js")

@app.route("/mapa")
def control_mapa():
    return send_from_directory(directory="mapa", path="mapa.html")

@app.route("/mapa/styles.css")
def control_mapa_css():
    return send_from_directory(directory="mapa", path="styles.css")

@app.route("/mapa/script.js")
def control_mapa_js():
    return send_from_directory(directory="mapa", path="script.js")

#########################################################################################################
def enviar_datos(carater, valor):
    sio.emit('enviar', {'c':carater,'f':valor})
        
#####################################################################################################
@socketio_.on('connect')
def handle_connect():
    print("Un cliente se ha conectado.")

@socketio_.on('send_message')
def handle_message(data):
    print(f"Mensaje recibido: {data}")
    emit('response', {'data': f"Mensaje recibido: {data}"}, broadcast=True) 


@socketio_.on('disconnect')
def handle_disconnect():
    print("Un cliente se ha desconectado.")

@socketio_.on('ac')
def handle_message_recibido(data):
   #print(f"Mensaje recibido: {data}")
    enviar_datos('a', data)

@socketio_.on('angle')
def handle_message_recibido(data):
   # print(f"Mensaje recibido: {data}")
    enviar_datos('g', data *-1)

@socketio_.on('en')
def handle_message_recibido_en(data):
   # print(f"Mensaje recibido: {data}")
    enviar_datos('z', data)

@socketio_.on('reversa')
def handle_message_recibido_re(data):
   # print(f"Mensaje recibido: {data}")
    enviar_datos('r', data)

@socketio_.on('mapa')
def handle_message_cordenadas(data):
    global contador
    contador+=1
    cordenada= {'lat': (-34.57426352636719+0.00001*contador), 'lng': -58.435791015625, 'title': f"Ubicación{contador}"}
    emit('cordenadas',cordenada, broadcast=True)

    

# Iniciar el servidor
if __name__ == "__main__":
     
     Hilo.start()
     time.sleep(2)
     sio.connect('http://127.0.0.1:5000')
     
     socketio_.run(app, host="0.0.0.0", port=8000, debug=False)
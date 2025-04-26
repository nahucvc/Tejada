import serial
import struct
import serial.tools.list_ports
import time
import numpy as np
from flask import Flask, send_from_directory
from flask_socketio import SocketIO, emit
import threading

app = Flask('com')
socketio = SocketIO(app)


class STM32:
    def __init__(self):
        self.puerto = self.buscar_puerto()
        if self.puerto:
            try:
                self.ser = serial.Serial(self.puerto, 460800, timeout=1)
                print(f"Conexión establecida en {self.puerto}")
            except serial.SerialException as e:
                print(f"Error al abrir el puerto serial: {e}")
                self.ser = None
        else:
            print("No se encontró un puerto compatible.")
            self.ser = None

    @staticmethod
    def buscar_puerto():
        puertos = serial.tools.list_ports.comports()
        for puerto in puertos:
            if "0483:5740" in puerto.hwid:  # Modifica esto si usas otro VID:PID
                return puerto.device
        return None

    def enviar_datos(self, caracter, numero_float):
        if self.ser and self.ser.is_open:
            try:
                # Convertir el carácter en su valor ASCII (uint8)
                char_as_uint8 = np.uint8(ord(caracter))
                
                # Convertir el número float a 4 bytes (formato IEEE 754)
                float_bytes = struct.pack('<f', numero_float)  # Little-endian
                
                # Crear una lista de datos para enviar
                datos = [char_as_uint8]  # Iniciar con el valor del carácter
                datos.extend(float_bytes)  # Agregar los bytes del número float
                datos.append(np.uint8(ord(';')))  # Agregar el delimitador ';'
                
                # Enviar los datos por el puerto serie
                self.ser.write(bytearray(datos))  # Convertir a bytearray antes de enviar
                time.sleep(0.0005)  # Pequeña pausa para asegurar la comunicación
                
               # print(f"Datos enviados: {datos}")
            except Exception as e:
                print(f"Error al enviar datos: {e}")
        else:
            print("El puerto serial no está abierto o no está disponible.")
            self.ser.open()

    def cerrar_puerto(self):
        if self.ser and self.ser.is_open:
            self.ser.close()
            print("Puerto serial cerrado.")

#************************************************************************************************
def iniciarServidorCOM():
    socketio.stm32=STM32()
    socketio.run(app, host="0.0.0.0", port=5000, debug=False)


@socketio.on('connect')
def handle_connect():
    print("Un cliente se ha conectado.")

@socketio.on('enviar')
def handle_enviar( datos):
    try:
        
        carater=datos['c']
        valor = datos['f']
        socketio.stm32.enviar_datos(carater,valor)
        
    except:
        print("error al enviar datos por serial")






    
Hilo=None
if Hilo==None:
    Hilo=threading.Thread(target=iniciarServidorCOM)




if __name__ == "__main__":
    Hilo.start()

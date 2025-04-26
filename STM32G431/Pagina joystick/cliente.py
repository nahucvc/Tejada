import socketio
class STM32cliente:
    def __init__(self,url):
        self.sio = socketio.SimpleClient()
        try:
            self.sio.connect(url)
        except:
            print(print("error al conectarse al servidor"))
    def enviar_datos(self , carater, valor):
        self.sio.emit('enviar', {'c':carater,'f':valor})

if __name__ == "__main__":
    stm32=STM32cliente('http://127.0.0.1:5000')
    stm32.enviar_datos('g', 20)
    print("dato enviado")
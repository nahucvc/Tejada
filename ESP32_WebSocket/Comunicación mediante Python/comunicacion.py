import serial
import threading
import time


class ControlSerial ():
    def __init__(self,puerto, baudios):
        try:
            self.ser = serial.Serial(port=puerto,baudrate=baudios)
        except serial.SerialException as e:
         print(f"No se pudo conectar al puerto serial: {e}")
    
        except ValueError as e:
            print(f"Error en la configuración: {e}")
        self.datos=0

    def control(self):
        
        threading.Thread(target=self.write_to_port, daemon=True).start()
        while True:
            try:
                if self.ser.in_waiting > 0:
                    data = self.ser.readline().decode('utf-8').strip()
                    print(f"Recibido por serial: {data}")
                if self.datos !=0:
                    self.ser.write(self.datos.encode("utf-8"))
                    print("Datos", self.datos,"\n")
                    self.datos=0
            except Exception as e:
                print(f"Error leyendo el puerto: {e}")
                break

    def write_to_port(self):
         while True:
            time.sleep(2)
            self.datos = '{"en":1,"angle":120,"ac":120}'
    
        
def main():
    
    port = "COM9"
    baud_rate = "115200"
    COM = ControlSerial(port,baud_rate)
    COM.control()
                
 
main()
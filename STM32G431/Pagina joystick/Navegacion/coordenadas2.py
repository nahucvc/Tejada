import math
import time
from pyproj import Proj
from vnpy import *
import serial.tools.list_ports
from cliente import STM32cliente
import matplotlib.pyplot as plt

stm32 = STM32cliente('http://127.0.0.1:5000')
time.sleep(0.5)

# Variables globales
trayectoria = [  # Coordenadas GPS
    (-34.574459075927734, -58.435604095458984),(-34.574283599853516, -58.43564987182617)
    
]
coordenadas_planas = []
vn = VnSensor()

# Sensor VectorNav
def buscar_puerto_vectornav():
    puertos = serial.tools.list_ports.comports()
    for puerto in puertos:
        print(f"Puerto: {puerto.device}, Descripción: {puerto.description}, HWID: {puerto.hwid}")
        if "0403:6001" in puerto.hwid:
            print("Puerto encontrado\n")
            return puerto.device
    raise Exception("No se encontró el puerto del sensor VectorNav")

puerto = buscar_puerto_vectornav()
vn.connect(puerto, 115200)

# Configurar el gráfico
def configurar_grafico():
    plt.ion()
    fig, ax = plt.subplots(figsize=(10, 10))
    ax.set_title("Visualización de trayectoria y posición actual")
    ax.set_xlabel("Coordenada X")
    ax.set_ylabel("Coordenada Y")
    ax.grid(True)
    
    # Agregar recuadros de información
    fig.text(0.2, 0.9, "Siguiente Punto: P?", fontsize=14, ha='center', bbox=dict(facecolor='white', edgecolor='black'))
    fig.text(0.5, 0.9, "RD = ?", fontsize=14, ha='center', bbox=dict(facecolor='white', edgecolor='black'))
    fig.text(0.8, 0.9, "D = ?", fontsize=14, ha='center', bbox=dict(facecolor='white', edgecolor='black'))
    
    fig.text(0.2, 0.05, "RA = ?", fontsize=14, ha='center', bbox=dict(facecolor='white', edgecolor='black'))
    fig.text(0.5, 0.05, "X = ?", fontsize=14, ha='center', bbox=dict(facecolor='white', edgecolor='black'))
    fig.text(0.8, 0.05, "Y = ?", fontsize=14, ha='center', bbox=dict(facecolor='white', edgecolor='black'))
    
    return fig, ax

# Función principal
def main():
    global coordenadas_planas
    pos_actual = (0.0, 0.0)
    fig, ax = configurar_grafico()
    puntos_plot, = ax.plot([], [], 'ro-', label="Trayectoria")
    posicion_plot, = ax.plot([], [], 'bo', label="Posición Actual")
    linea_conexion, = ax.plot([], [], 'g--', label="Conexión Actual-P1")
    etiquetas = []

    # Inicialización
    for i, punto in enumerate(trayectoria):
        x, y = convertir_a_planas(punto)
        coordenadas_planas.append((x, y))
        etiqueta = ax.text(x, y, f"P{i+1}", fontsize=9, color="red", ha='right')
        etiquetas.append(etiqueta)

    puntos_plot.set_data(*zip(*coordenadas_planas))

    pos_actual = leer_posicion_GPS()

    # Ajustar los límites del gráfico
    ax.set_xlim(min(x for x, _ in coordenadas_planas) - 10, max(x for x, _ in coordenadas_planas) + 10)
    ax.set_ylim(min(y for _, y in coordenadas_planas) - 10, max(y for _, y in coordenadas_planas) + 10)
    ax.legend()

    # Navegar hacia los puntos de la trayectoria
    etiqueta_auto = ax.text(0, 0, "Auto", fontsize=9, color="blue", ha='left')  # Etiqueta para posición actual
    for idx, punto in enumerate(coordenadas_planas):
        while not cerca_de_waypoint(pos_actual, punto):
            rumbo_deseado = calcular_rumbo_siguiente(pos_actual, punto)
            rumbo_actual = leer_rumbo_actual()
            error_rumbo = rumbo_deseado - rumbo_actual
            if error_rumbo > 180:
                error_rumbo -= 360
            elif error_rumbo < -180:
                error_rumbo += 360
            angle = error_rumbo  # ENVIAR ANGLE Y AC
            distancia = calcular_distancia(pos_actual, punto)

            # Actualizar gráficos
            posicion_plot.set_data([pos_actual[0]], [pos_actual[1]])
            linea_conexion.set_data([pos_actual[0], punto[0]], [pos_actual[1], punto[1]])
            etiqueta_auto.set_position((pos_actual[0], pos_actual[1]))
            etiqueta_auto.set_text("Auto")
            
            # Actualizar recuadros de información
            fig.texts[0].set_text(f"Siguiente Punto: P{idx+1}")
            fig.texts[1].set_text(f"RD = {rumbo_deseado:.2f}")
            fig.texts[2].set_text(f"D = {distancia:.2f} m")
            fig.texts[3].set_text(f"RA = {rumbo_actual:.2f}")
            fig.texts[4].set_text(f"X = {pos_actual[0]:.2f}")
            fig.texts[5].set_text(f"Y = {pos_actual[1]:.2f}")
            
            ax.set_title(f"Rumbo Actual: {rumbo_actual:.2f}° | Distancia al siguiente punto: {distancia:.2f} m")
            plt.pause(0.01)
            # Enviar datos al comunicador
            stm32.enviar_datos('g',angle)
            stm32.enviar_datos('a',15)
            time.sleep(0.01)


            # Actualizar la posición actual
            pos_actual = leer_posicion_GPS()
            time.sleep(0.02)

        print(f"Waypoint alcanzado: {punto}")

    plt.ioff()
    plt.show()

def leer_posicion_GPS():
    gps = vn.read_gps_solution_lla()
    posicion = convertir_a_planas((gps.lla.x, gps.lla.y))
    return posicion

def leer_rumbo_actual():
    lectura = vn.read_yaw_pitch_roll()
    return lectura.x

def calcular_rumbo_siguiente(pos_actual, punto):
    x, y = pos_actual
    x2, y2 = punto
    rumbo_deseado = math.atan2(x2 - x, y2 - y)
    return rumbo_deseado * (180 / math.pi)

def cerca_de_waypoint(pos_actual, punto):
    distancia = calcular_distancia(pos_actual, punto)
    return distancia < 3

def calcular_distancia(pos1, pos2):
    return math.sqrt((pos1[0] - pos2[0])**2 + (pos1[1] - pos2[1])**2)

def convertir_a_planas(coordenadas):
    lat, lon = coordenadas
    proj_gk = Proj(proj="tmerc", lon_0=-57, k=1, x_0=6500000, y_0=10001965.7, ellps="WGS84")
    x, y = proj_gk(lon, lat)
    return x, y

if __name__ == "__main__":
    main()

import time
from cliente import STM32cliente


stm32 = STM32cliente('http://127.0.0.1:5000')
time.sleep(0.5)
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

def obtener_coordenadas_gps():
    """
    Simula la obtención de coordenadas GPS del sensor.
    Deberás modificar esta función para usar tu propio sensor GPS.
    """
    # Cambiar con la lectura real del GPS en tu implementación.
    gps = vn.read_gps_solution_lla()
    return (gps.lla.x, gps.lla.y)

def moverVehiculo(error_angulo):
    stm32.enviar_datos('g',error_angulo)
    stm32.enviar_datos('a',15)
    

def calculate_direction_and_correction(current, previous, target):
    # Misma función que la enviada previamente
    import math

    def haversine_distance(coord1, coord2):
        R = 6371000  # Radio de la Tierra en metros
        lat1, lon1 = map(math.radians, coord1)
        lat2, lon2 = map(math.radians, coord2)
        dlat = lat2 - lat1
        dlon = lon2 - lon1
        a = math.sin(dlat / 2)**2 + math.cos(lat1) * math.cos(lat2) * math.sin(dlon / 2)**2
        return R * 2 * math.atan2(math.sqrt(a), math.sqrt(1 - a))
    
    def calculate_bearing(coord1, coord2):
        lat1, lon1 = map(math.radians, coord1)
        lat2, lon2 = map(math.radians, coord2)
        dlon = lon2 - lon1
        x = math.sin(dlon) * math.cos(lat2)
        y = math.cos(lat1) * math.sin(lat2) - math.sin(lat1) * math.cos(lat2) * math.cos(dlon)
        return (math.degrees(math.atan2(x, y)) + 360) % 360

    distance_to_target = haversine_distance(current, target)
    if distance_to_target <= 5:  # Condición de proximidad
        return 0, 0
    
    current_direction = calculate_bearing(previous, current)
    target_direction = calculate_bearing(current, target)
    correction_angle = (target_direction - current_direction + 360) % 360
    if correction_angle > 180:
        correction_angle -= 360

    return current_direction, correction_angle

def navegar_ruta(ruta):
    """
    Navega por una ruta definida de coordenadas, pasando al siguiente punto
    cuando la distancia al actual sea menor o igual a 5 metros.
    
    :param ruta: Lista de tuplas con coordenadas (lat, lon).
    """
    # Inicializa el punto de partida
    if len(ruta) < 2:
        print("La ruta debe tener al menos dos puntos.")
        return

    index = 0
    previous_position = ruta[0]  # Inicializamos con el primer punto
    current_position = obtener_coordenadas_gps()  # Leer del GPS real

    while index < len(ruta) - 1:
        target_position = ruta[index + 1]

        # Calcula la dirección actual y el ángulo de corrección
        current_direction, correction_angle = calculate_direction_and_correction(
            current_position, previous_position, target_position
        )

        # Distancia al objetivo
        distance_to_target = calculate_direction_and_correction(current_position, previous_position, target_position)[1]

        # Imprime información del estado
        print(f"Posición actual: {current_position}")
        print(f"Distancia al objetivo: {distance_to_target:.2f} m")
        print(f"Dirección actual: {current_direction:.2f}°")
        print(f"Ángulo de corrección: {correction_angle:.2f}°")

        # Realiza el movimiento según el ángulo de corrección
        moverVehiculo(correction_angle)

        # Verifica si el punto objetivo se alcanzó
        if distance_to_target <= 5:
            print(f"Punto alcanzado: {target_position}")
            index += 1
            previous_position = current_position  # Actualiza posición previa
        else:
            print(f"Corrigiendo dirección hacia el objetivo...")
        
        # Simula la obtención de nueva posición del GPS
        current_position = obtener_coordenadas_gps()
        time.sleep(0.5)  # Simula un retraso entre lecturas GPS

    print("Ruta completada.")

# Ruta de ejemplo
ruta_de_prueba = [
    (-58.4349632263, -34.5730781551),
    (-58.43527603149414, -34.57279586791992),
    (-58.43650436401367, -34.571510314941406)   # Punto final
]
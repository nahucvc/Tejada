// Inicializa el mapa centrado en las nuevas coordenadas
const map = L.map('map').setView([-34.57426352636719, -58.435791015625], 16); // Punto inicial ajustado

// Añade la capa de satélite (Esri)
L.tileLayer('https://server.arcgisonline.com/ArcGIS/rest/services/World_Imagery/MapServer/tile/{z}/{y}/{x}', {
    maxZoom: 19,
    attribution: 'Tiles © Esri — Source: Esri, i-cubed, USDA, USGS, AEX, GeoEye, Getmapping, Aerogrid, IGN, IGP, UPR-EGP, and the GIS User Community'
}).addTo(map);

// Array para almacenar marcadores
const markers = [];

// Función para agregar una ubicación
function addLocation(lat, lng, title) {
    const marker = L.marker([lat, lng])
        .addTo(map)
        .bindPopup(title); // Ventana emergente con información
    markers.push(marker);
}

// Función para eliminar la última ubicación agregada
function removeLastLocation() {
    if (markers.length > 0) {
        const lastMarker = markers.pop(); // Elimina el último marcador del array
        map.removeLayer(lastMarker); // Elimina el marcador del mapa
    } else {
        alert("No hay ubicaciones para eliminar.");
    }
}

// Función para agregar las ubicaciones iniciales
function addInitialLocations() {
    const initialLocations = [
        { lat: -34.57426352636719, lng: -58.435791015625, title: "Ubicación 1" },
        { lat: -34.574459075927734, lng: -58.435604095458984, title: "Ubicación 2" },
    ];

    initialLocations.forEach(location => {
        addLocation(location.lat, location.lng, location.title);
    });
}

var servidor = window.location.protocol + '//' + window.location.hostname + ':8000'; // Construir URL dinámicamente

// Función para conectar el cliente WebSocket
function connectWebSocket() {
  try {
    const socket = io(servidor); // Usamos io() en lugar de WebSocket nativo

    // Evento que se dispara cuando la conexión se establece con éxito
    socket.on('connect', () => {
      console.log('Conexión establecida correctamente.');

    });

    // Evento que se dispara cuando el servidor envía un mensaje
    socket.on('message', (data) => {
      console.log('Mensaje recibido:', data);

    });

    // Evento que se dispara cuando se cierra la conexión
    socket.on('disconnect', (reason) => {
      console.log('Conexión cerrada:', reason);
     
    });

    // Evento que se dispara cuando ocurre un error
    socket.on('connect_error', (error) => {
      console.error('Error en la conexión:', error);

    });
    socket.on('cordenadas',(reason) => {
      // console.log('dato recibido:', reason.lat, reason.lng, reason.title);
       addLocation(reason.lat, reason.lng, reason.title)
       
      });

    // Retornar el objeto socket para usarlo en otras partes del código
    return socket;
  } catch (error) {
    console.error('Error al establecer la conexión WebSocket:', error);
    return null;
  }
}

// Conectar al WebSocket
const webSocket = connectWebSocket();

// Enviar datos cada 10 ms si la conexión es exitosa
setInterval(() => {
    if (webSocket !== null) { // Corregido la comparación
        webSocket.emit("mapa", 5454);
    }
}, 1000);

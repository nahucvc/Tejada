// =====================
// Joystick + Aceleración
// Envío combinado cada 100 ms (angle + ac)
// =====================

// ---- Referencias DOM ----
const joystick = document.querySelector('.joystick'); // Contenedor del joystick
const stick    = document.querySelector('.stick');    // Stick (parte móvil)

const slider       = document.getElementById('slider');
const barra        = document.getElementById('barra');
const valueElement = document.getElementById('value');

const encendidoBtn = document.getElementById("encendido");
const B_baja  = document.getElementById("B_baja");
const B_media = document.getElementById("B_media");
const B_alta  = document.getElementById("B_alta");

// ---- Estado global ----
let isDragging    = false;
let isAcelerando  = false;
let fondoEscala   = 0;    // 0.3 / 0.6 / 1 según botón
let webSocket     = null;

const state = {
  angle: 90,  // grados
  accel: 0,   // 0..100 (escalado por fondoEscala)
};

// ---- Loop de transmisión (cada 100 ms) ----
const TX_INTERVAL_MS = 20;
setInterval(() => {
  if (webSocket && webSocket.readyState === WebSocket.OPEN) {
    webSocket.send(JSON.stringify({ angle: state.angle, ac: state.accel }));
  }
}, TX_INTERVAL_MS);

// ---- Eventos táctiles del joystick ----
joystick.addEventListener('touchstart', startDragging, { passive: false });
window.addEventListener('touchend',   stopDragging,    { passive: true  });
window.addEventListener('touchmove',  drag,            { passive: false });

function startDragging(event) {
  event.preventDefault();
  isDragging = true;
  stick.style.transition = 'none';
}

function stopDragging() {
  if (!isDragging) return;
  isDragging = false;
  stick.style.transition = 'all 0.1s ease';
  // Volvemos el stick visualmente al centro (el valor angle se conserva)
  stick.style.transform = 'translate(-50%, -50%)';
}

function drag(event) {
  if (!isDragging) return;
  event.preventDefault();

  const joystickRect = joystick.getBoundingClientRect();
  const stickRect    = stick.getBoundingClientRect();

  let touch = event.touches[0];
  if (touch && joystickRect.right < touch.clientX && event.touches[1]) {
    touch = event.touches[1];
  }
  if (!touch) return;

  const offsetX = touch.clientX - joystickRect.left - joystickRect.width / 2;
  const offsetY = touch.clientY - joystickRect.top  - joystickRect.height / 2;

  const maxOffset = joystickRect.width / 2 - stickRect.width / 2;
  const distance  = Math.min(Math.hypot(offsetX, offsetY), maxOffset);

  const angleRad = Math.atan2(offsetY, offsetX);
  let angleDeg   = angleRad * (180 / Math.PI);
  angleDeg = (angleDeg * -1) - 17; // compensación original
  // Limitar a [-180, 180] (opcional)
  if (angleDeg > 180) angleDeg -= 360;
  if (angleDeg < -180) angleDeg += 360;

  state.angle = angleDeg;

  // Mover el stick
  const x = distance * Math.cos(angleRad);
  const y = distance * Math.sin(angleRad);
  stick.style.transform = `translate(calc(-50% + ${x}px), calc(-50% + ${y}px))`;
}

// ---- WebSocket ----
const servidor = 'ws://' + window.location.hostname + '/ws';
// const servidor = 'ws://192.168.0.8/ws';

function connectWebSocket() {
  try {
    const socket = new WebSocket(servidor);

    socket.onopen = () => {
      console.log('Conexión establecida correctamente.');
    };

    socket.onmessage = (event) => {
      const message = JSON.parse(event.data);
      if (message.encendido === true) {
        socket.send(JSON.stringify({ en: 1 }));
      }
    };

    socket.onclose = (event) => {
      console.log('Conexión cerrada:', event.code, event.reason);
    };

    socket.onerror = (error) => {
      console.error('Error en la conexión:', error);
    };

    return socket;
  } catch (error) {
    console.error('Error al establecer la conexión WebSocket:', error);
    return null;
  }
}

// ---- Aceleración (barra deslizante vertical) ----
barra.addEventListener('touchstart', starAcelerar, { passive: false });
barra.addEventListener('touchmove',  acelerar,     { passive: false });
barra.addEventListener('touchend',   stopAcelerar, { passive: true  });

function acelerar(event) {
  if (!isAcelerando) return;
  event.preventDefault();

  const rect = barra.getBoundingClientRect();
  let tacto  = event.touches[0];
  if (tacto && tacto.clientX < rect.left && event.touches[1]) {
    tacto = event.touches[1];
  }
  if (!tacto) return;

  let value = ((rect.bottom - tacto.clientY) / rect.height) * 100;
  if (value > 100) value = 100;
  if (value < 0)   value = 0;

  state.accel = value * fondoEscala;

  slider.style.height = value + '%';
  slider.style.transition = "0.1s";
  valueElement.textContent = Math.round(value) + "%";
}

function starAcelerar(event) {
  isAcelerando = true;
  event.preventDefault();
}

function stopAcelerar() {
  isAcelerando = false;
  valueElement.textContent = "0%";
  slider.style.height = '0%';
  state.accel = 0;
}

// ---- Botón de encendido / conexión ----
encendidoBtn.addEventListener('click', function () {
  if (webSocket) {
    if (encendidoBtn.textContent === "Encendido") {
      webSocket.close();
      webSocket = null;
      encendidoBtn.textContent = "Apagado";
      encendidoBtn.style.backgroundColor = "rgb(254, 10, 10)";
    } else {
      webSocket = connectWebSocket();
      encendidoBtn.style.backgroundColor = "rgb(0, 255, 38)";
      encendidoBtn.textContent = "Encendido";
    }
  } else {
    webSocket = connectWebSocket();
    encendidoBtn.style.backgroundColor = "rgb(0, 255, 38)";
    encendidoBtn.textContent = "Encendido";
  }
});

// ---- Configuración de botones de aceleración ----
B_baja.addEventListener('click', function () {
  fondoEscala = 0.3;
  B_baja.style.backgroundColor  = "rgb(0, 255, 8)";
  B_media.style.backgroundColor = "#f00";
  B_alta.style.backgroundColor  = "#f00";
});

B_media.addEventListener('click', function () {
  fondoEscala = 0.6;
  B_baja.style.backgroundColor  = "#f00";
  B_media.style.backgroundColor = "rgb(0, 255, 8)";
  B_alta.style.backgroundColor  = "#f00";
});

B_alta.addEventListener('click', function () {
  fondoEscala = 1;
  B_baja.style.backgroundColor  = "#f00";
  B_media.style.backgroundColor = "#f00";
  B_alta.style.backgroundColor  = "rgb(0, 255, 8)";
});

// ---- Eliminamos los delays bloqueantes del código original ----
// (Se reemplazó el envío por un intervalo periódico)

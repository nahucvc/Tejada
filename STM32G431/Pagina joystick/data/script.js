// Obtenemos los elementos del DOM que necesitaremos manipular
const joystick = document.querySelector('.joystick'); // El contenedor del joystick
const stick = document.querySelector('.stick'); // El stick (la parte mÃ³vil) del joystick
var angulo = { angle: 90 };
// Cuando el usuario toca el joystick, comenzamos a arrastrar el stick
joystick.addEventListener('touchstart', startDragging);

// Cuando el usuario levanta el dedo de cualquier parte de la pantalla, dejamos de arrastrar el stick
window.addEventListener('touchend', stopDragging);

// Mientras el usuario arrastra el dedo en cualquier parte de la pantalla, movemos el stick
window.addEventListener('touchmove', drag);

let isDragging = false;

// Esta funciÃ³n se ejecuta cuando el usuario toca el joystick para comenzar a arrastrar el stick
function startDragging(event) {
  event.preventDefault(); // Prevenimos el comportamiento predeterminado del evento touchstart
  isDragging = true;
  stick.style.transition = 'none'; // Desactivamos las transiciones para un movimiento mÃ¡s suave
}

// Esta funciÃ³n se ejecuta cuando el usuario levanta el dedo de cualquier parte de la pantalla
function stopDragging() {
  if (isDragging) {
    isDragging = false;
    stick.style.transition = 'all 0.1s ease'; // Agregamos una transiciÃ³n suave para volver el stick a su posiciÃ³n central
    stick.style.transform = 'translate(-50%, -50%)'; // Movemos el stick de vuelta a su posiciÃ³n central en el joystick
  }
}

// Esta funciÃ³n se ejecuta mientras el usuario arrastra el dedo en cualquier lugar de la pantalla
function drag(event) {
  if (isDragging) {
    // Obtenemos los tamaÃ±os y posiciones de los elementos para calcular el desplazamiento del stick
    const joystickRect = joystick.getBoundingClientRect();
    const stickRect = stick.getBoundingClientRect();
    let touch = event.touches[0]; // Obtiene la informaciÃ³n del primer dedo que se estÃ¡ moviendo
    if (joystickRect.right < touch.clientX) {
      touch = event.touches[1];
    }

    const offsetX = touch.clientX - joystickRect.left - joystickRect.width / 2;
    const offsetY = touch.clientY - joystickRect.top - joystickRect.height / 2;

    // Calculamos la distancia desde el centro del joystick al punto donde el usuario arrastra el dedo
    const maxOffset = joystickRect.width / 2 - stickRect.width / 2;
    const distance = Math.min(Math.sqrt(offsetX ** 2 + offsetY ** 2), maxOffset);

    // Calculamos el Ã¡ngulo de direcciÃ³n del movimiento del stick
    const angle = Math.atan2(offsetY, offsetX);
    angulo.angle = angle * (180 / Math.PI);
    angulo.angle = angulo.angle * (-1)
    // Calculamos las coordenadas X e Y del movimiento del stick segÃºn el Ã¡ngulo y la distancia
    const x = distance * Math.cos(angle);
    const y = distance * Math.sin(angle);

    // Movemos el stick a las coordenadas calculadas
    stick.style.transform = `translate(${x}px, ${y}px)`;
    // delayBloqueanteEnMilisegundos(30);
    setTimeout(() => {
      if (webSocket) {
        angulo.angle = (Math.abs(angulo.angle) * 0.38) - 34;

        webSocket.emit("angle", parseFloat(angulo.angle.toFixed(3)));
      }
    }, 0.07)



  }
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
      webSocket = null;
      encendido.textContent = "Apagado";
      encendido.style.backgroundColor = "rgb(254, 10, 10)"
    });

    // Evento que se dispara cuando ocurre un error
    socket.on('connect_error', (error) => {
      console.error('Error en la conexión:', error);

      webSocket = null;
      encendido.textContent = "Apagado";
      encendido.style.backgroundColor = "rgb(254, 10, 10)"
      socket.close();
    });

    // Retornar el objeto socket para usarlo en otras partes del código
    return socket;
  } catch (error) {
    console.error('Error al establecer la conexión WebSocket:', error);
    return null;
  }
}







//******************************************************************************************************************** */
// codigo para controlar la barra de aceleración 

const slider = document.getElementById('slider');
const barra = document.getElementById('barra');
const valueElement = document.getElementById('value');
var fondoEscala = 0;
let isAcelerando = false;
let _Acelerar = { ac: 0 };
barra.addEventListener('touchstart', starAcelerar);
barra.addEventListener('touchmove', acelerar);
barra.addEventListener('touchend', stopAcelerar);

function acelerar(event) {
  if (isAcelerando) {
    const _barra = barra.getBoundingClientRect();
    let tacto = event.touches[0];
    //console.log(_barra);
    //console.log(tacto);
    if (tacto.clientX < _barra.left) {
      tacto = event.touches[1];
    }

    const value = ((_barra.bottom - tacto.clientY) / _barra.height) * 100;
    if (value > 100) {
      value = 100;

    }
    _Acelerar.ac = value * fondoEscala;
    console.log(value);
    slider.style.height = value + '%';
    slider.style.transition = "0.1s"
    valueElement.textContent = value;
    setTimeout(() => {
      if (webSocket) {
        //webSocket.send(JSON.stringify(_Acelerar));
        webSocket.emit("ac", parseFloat(_Acelerar.ac.toFixed(3)));
      }

    }, 0.05)


  }
}


function starAcelerar(event) {
  isAcelerando = true;
  event.preventDefault();
}

function stopAcelerar() {
  isAcelerando = false;
  valueElement.textContent = "0%";
  slider.style.height = 0 + '%';
  _Acelerar.ac = 0;
  if (webSocket) {
    webSocket.emit("ac", parseFloat(_Acelerar.ac.toFixed(3)));

  }
}

// codigo para controlar si el joystick sigue conectado

let encendido = document.getElementById("encendido");
var webSocket = null;
encendido.addEventListener('click', function (event) {
  if (webSocket != null) {
    if (encendido.textContent == "Encendido") {
      webSocket.close();
      webSocket = null;
      encendido.textContent = "Apagado";
      encendido.style.backgroundColor = "rgb(254, 10, 10)"
    } else {
      webSocket = connectWebSocket();
      if (webSocket = !null) {
        encendido.style.backgroundColor = "rgb(0, 255, 38)"
        encendido.textContent = "Encendido";
      }

    }
  } else {
    webSocket = connectWebSocket();
    encendido.style.backgroundColor = "rgb(0, 255, 38)";
    encendido.textContent = "Encendido";

  }

});

//configuracion de los botones de aceleracion 
var B_baja = document.getElementById("B_baja");
var B_media = document.getElementById("B_media");
var B_alta = document.getElementById("B_alta");

B_baja.addEventListener('click', function (event) {
  fondoEscala = 0.3;
  B_baja.style.backgroundColor = " rgb(0, 255, 8)";
  B_media.style.backgroundColor = "#f00";
  B_alta.style.backgroundColor = "#f00";
});

B_media.addEventListener('click', function (event) {
  fondoEscala = 0.6;
  B_baja.style.backgroundColor = "#f00";
  B_media.style.backgroundColor = "rgb(0, 255, 8)";
  B_alta.style.backgroundColor = "#f00";
});

B_alta.addEventListener('click', function (event) {
  fondoEscala = 1;
  B_baja.style.backgroundColor = "#f00";
  B_media.style.backgroundColor = "#f00";
  B_alta.style.backgroundColor = "rgb(0, 255, 8)";
});

setInterval(() => {
  if (webSocket) {
    webSocket.emit("en", 1);

  }

}, 0.4)


const reversaButton = document.getElementById('reversa');
var stateReversa = 0;
// Función para alternar el color del botón
reversaButton.addEventListener('click', function () {
  if (reversaButton.style.backgroundColor === 'green') {
    reversaButton.style.backgroundColor = 'red';
    stateReversa = 0;
    if (webSocket) {
      webSocket.emit("reversa", 0);
      stateReversa = 0;

    }
  } else {
    reversaButton.style.backgroundColor = 'green';
    if (webSocket) {
      webSocket.emit("reversa", 1);
      stateReversa = 1;
    }
  }
});

// Inicializa el color del botón en rojo
reversaButton.style.backgroundColor = 'red';

// Selecciona el botón freno
const frenoButton = document.getElementById('freno');

// Función para cambiar el color a verde mientras se presiona
frenoButton.addEventListener('touchstart', function () {
  frenoButton.style.backgroundColor = 'green';
  if (webSocket) {
    if (stateReversa == 0) {
      webSocket.emit("reversa", 1);
      webSocket.emit("ac", 80);
    } 
    if (stateReversa == 1) {
      webSocket.emit("reversa", 0);
      webSocket.emit("ac", 80);
    }

  }
});

// Función para cambiar el color a rojo cuando se suelta
frenoButton.addEventListener('touchend', function () {
  frenoButton.style.backgroundColor = 'red';
  if (webSocket) {
    if (stateReversa == 0) {
      webSocket.emit("reversa", 0);
      webSocket.emit("ac", 0);
    } else {

      webSocket.emit("reversa", 1);
      webSocket.emit("ac", 0);


    }


  }
});

// Inicializa el color del botón en rojo
frenoButton.style.backgroundColor = 'red';


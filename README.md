# 🚗 Carrito Seguidor de Líneas de Color

Este proyecto consiste en un carrito autónomo diseñado para seguir una línea de color específica (naranja o verde) sobre un fondo blanco. Su principal objetivo es mantener el rumbo sin confundirse, incluso cuando las líneas de ambos colores se cruzan o entrelazan.

El sistema utiliza sensores de color y lógica de control para detectar y seguir únicamente el color deseado, ignorando interferencias visuales del otro tono. Es ideal para prácticas de robótica básica, visión por sensores y proyectos de ingeniería mecatrónica.

---

## ⚙️ Tecnologías y componentes utilizados

### Software
- Arduino IDE

### Hardware
- Arduino Uno
- Shield de motores L293D
- 2 sensores de color TCS3200
- 2 motores amarillos con reductores de eje doble
- 4 pilas AA (6V en total) para la alimentación
- Chasis con ruedas (según diseño propio)
- Protoboard y cables jumper (opcional, según la conexión)

---

## 🛠️ Instalación y uso

1. Clona este repositorio:
   ```bash
   git clone https://github.com/tuusuario/carrito-seguidor-lineas-color.git
   ```

2. Abre el archivo `.ino` en Arduino IDE.

3. Conecta el Arduino Uno mediante el cable USB y selecciona el puerto correcto.

4. Carga el código al microcontrolador.

5. Coloca el carrito sobre una pista blanca que contenga líneas naranja y verde.

6. Asegúrate de haber calibrado previamente los sensores TCS3200 para que reconozcan con precisión los valores RGB del color a seguir.

7. Alimenta el sistema con las 4 pilas AA.

---

## 🎯 Funcionamiento general

- El carrito utiliza dos sensores de color TCS3200 ubicados al frente, ligeramente separados, para escanear el color directamente bajo cada uno.
- Según el color detectado, el sistema toma decisiones para mantener el rumbo, girar o corregir su trayectoria.
- Si el carrito encuentra una línea del color **no deseado**, la ignora y continúa buscando el color objetivo.
- La lógica permite priorizar siempre un solo color durante todo el recorrido.

---

## ✨ Funcionalidades destacadas

- 🔍 Detección confiable de dos colores distintos (verde y naranja).
- 🧠 Discriminación activa: sigue solo el color objetivo ignorando el otro.
- 🔁 Mantenimiento de trayectoria incluso con entrelazamientos.
- ⚡ Bajo consumo energético gracias al uso eficiente de componentes.

---

## 📌 Recomendaciones

- Calibra los sensores de color bajo la misma iluminación que se usará en la pista.
- Usa una superficie blanca limpia para evitar interferencias.
- Puedes ajustar los umbrales de detección en el código para mejorar la precisión.

---

## 📄 Licencia

Este proyecto no cuenta con una licencia específica. Todos los derechos reservados por su autor.

---

## 👨‍💻 Autor

**Daniel Urzúa**  
Desarrollado con fines educativos y de competencia en proyectos de robótica autónoma.

---



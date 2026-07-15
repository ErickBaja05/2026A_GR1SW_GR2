# home2yo

## Misión
Transformar la experiencia de compra y venta de bienes raíces a través de tecnología inmersiva, ofreciendo a las
inmobiliarias y a sus clientes recorridos virtuales interactivos y ultrarrealistas. En home2you, optimizamos el tiempo 
y los recursos del sector, empoderando a los asesores con herramientas de vanguardia y brindando a los compradores la confianza
necesaria para tomar decisiones ágiles e informadas desde cualquier lugar. 

## Visión
Ser el software líder y el nuevo estándar global en la visualización de propiedades, redefiniendo el mercado inmobiliario 
para que la exploración y adquisición de una vivienda sea una experiencia 100% digital, accesible, transparente y sin fronteras. 

# Motor Gráfico 3D en C++ (OpenGL & GLM)
Un motor de renderizado 3D interactivo desarrollado en C++ utilizando OpenGL y la librería de matemáticas GLM. 
La aplicación está estructurada para manejar de manera eficiente el renderizado, las colisiones y las interacciones del
entorno mediante una arquitectura jerárquica robusta.

## Características Principales
### Importación de Modelos
El motor cuenta con la capacidad de importar y procesar modelos 3D complejos. Mediante el uso de abstracciones para las mallas (Meshes) 
y la asignación de materiales, se extraen vértices, índices, normales y texturas. Estos datos se envían a la tarjeta gráfica para renderizar 
geometrías de distintos niveles de complejidad (como paredes, pisos, sillas y otros elementos del escenario).

## Arquitectura
```text
📁 ProyectoSimulador/
├── 📄 .gitignore
[cite_start]├── 📄 README.md                 
├── 📄 Simulador.slnx
├── 📁 Dependencies/              
├── 📁 Assets/
│   ├── 📁 models/                
│   ├── 📁 textures/              
│   └── 📁 shaders/               
└── 📁 src/
    ├── 📁 Engine/                
    ├── 📁 Graphics/              
    ├── 📁 Scene/                 
    ├── 📁 Lighting/              
    └── 📄 main.cpp

```
### Uso de Lighting (Iluminación)
La iluminación de la escena se maneja de forma dinámica utilizando modelos de sombreado (como Phong). Se integran fuentes de luz puntuales 
en el entorno que calculan en tiempo real la contribución ambiental, difusa y especular sobre los materiales de los modelos. 
Gracias al sistema de manejo de luces, los shaders aplican la atenuación correspondiente según la distancia para lograr un efecto realista.

### Las Colisiones
Para la física y la navegación, se implementa un sistema de colisiones (`CollisionManager`) que evalúa las intersecciones en el entorno 
tridimensional. Esto previene que la cámara y los objetos atraviesen paredes u otros obstáculos, y es la base para el sistema de interacción
del usuario (detectando, por ejemplo, cuándo se está lo suficientemente cerca para accionar un interruptor de luz o abrir una puerta).

## Requisitos y Tecnologías
* **C++17** (o superior)
* **OpenGL 3.3+**
* **GLM** (OpenGL Mathematics)
* **Glad / GLFW** (Para la creación de contextos y manejo de ventanas)

## Compilación y Ejecución
1. Clonar el repositorio.
2. Generar los archivos de compilación usando CMake.
3. Compilar el proyecto y ejecutar el binario resultante. Asegúrese de que los shaders y recursos (modelos/texturas) se encuentren en el directorio de trabajo correcto.

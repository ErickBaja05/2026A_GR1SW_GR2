# home2yo

## Misión
Transformar la experiencia de compra y venta de bienes raíces a través de tecnología inmersiva, ofreciendo a las inmobiliarias y a sus clientes recorridos virtuales interactivos y ultrarrealistas.


## Visión
Que nuestro software se convierta en el estándar para la compra y venta de propiedades inmuebles dentro del país en los próximos 10 años. 

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


## Tecnologías
* **C++17**
* **OpenGL 3.3+**
* **GLM** (OpenGL Mathematics)
* **Glad / GLFW** (Para la creación de contextos y manejo de ventanas)

## Compilación y Ejecución
1. Clonar el repositorio.
2. Abrir el archivo Simulador.slnx en Visual Studio
3. Compilar la ejecución


# home2yo


# Motor Gráfico 3D en C++ (OpenGL & GLM)
Un motor de renderizado 3D interactivo desarrollado en C++ utilizando OpenGL y la librería de matemáticas GLM. 
La aplicación está estructurada para manejar de manera eficiente el renderizado y las interacciones del
entorno mediante una arquitectura jerárquica robusta.

## Características Principales
### Importación de Modelos
El motor cuenta con la capacidad de importar y procesar modelos 3D complejos. Mediante el uso de abstracciones para las mallas (Meshes) 
y la asignación de materiales, se extraen vértices, índices, normales y texturas. Estos datos se envían a la tarjeta gráfica para renderizar 
geometrías de distintos niveles de complejidad (como paredes, pisos, sillas y otros elementos del escenario).

## Arquitectura
```text
📁 ProyectoSimulador/
├──📄 .gitignore
├──📄 README.md
├──📄 CMakeLists.txt
├──📄 LICENSE                       
├── 📁 assets/
│   ├── 📁 models/                
│   ├── 📁 textures/              
│   └── 📁 shaders/               
└── 📁 src/
    ├── 📁 Engine/                
    ├── 📁 Graphics/              
    ├── 📁 Scene/                 
    ├── 📁 Lighting/
    ├── 📁 Input/
    ├── 📁 Interactable_Objects/               
    └── 📄 main.cpp
    └── 📄 glad.c

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

Este proyecto requiere:
- OpenGL
- GLFW3
- GLM
- Assimp

El primer paso es clonar el repositorio:

```bash
git clone https://github.com/ErickBaja05/2026A_GR1SW_GR2
```

### Instalación de dependencias

#### 🔹 Linux (Fedora / Ubuntu)
Instalar paquetes de desarrollo:

**Fedora:**
```bash
sudo dnf install glfw-devel glm-devel assimp-devel
```

**Ubuntu/Debian:**
```bash
sudo apt install libglfw3-dev libglm-dev libassimp-dev
```
#### 🔹 Windows
Usar vcpkg como gestor de dependencias

Realizar todos los pasos a continuación desde PowerShell

1.- Clonar vcpkg en C:\

```bash
cd C:\
git clone https://github.com/microsoft/vcpkg
cd vcpkg
.\bootstrap-vcpkg.bat

```

2.- Instalar librerias

```bash
.\vcpkg install glfw3 glm assimp
```

### Compilación y ejecución

**Linux:**

Desde la raíz del proyecto (GPU INTEGRADA EN LAPTOP O LA CONECTADA AL MONITOR EN PC ESCRITORIO)

```bash
cmake -B build -S .
cmake --build build
./build/ProyectoSimulador


```

Desde la raíz del proyecto (GPU DEDICADA EN LAPTOP NVIDIA O LA CONECTADA AL MONITOR EN PC ESCRITORIO)

```bash
cmake -B build -S .
cmake --build build
__NV_PRIME_RENDER_OFFLOAD=1 \
__GLX_VENDOR_LIBRARY_NAME=nvidia \
./build/ProyectoSimulador

```

**Windows:**


```bash
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Debug
cd build\Debug
.\ProyectoSimulador.exe

```

Asegurarse de que el Sistema Operativo este ejecutando el .exe con la gráfica dedicada (si tiene una) desde las configuraciones de Windows en 
Configuración -> Pantalla -> Gráficos. Añada el ProyectoSimulador.exe y escoja la grafica de su preferencia para la ejecución del Simulador

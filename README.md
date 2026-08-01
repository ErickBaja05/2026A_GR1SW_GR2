# home2yo


# Motor Gráfico 3D en C++ (OpenGL & GLM)
Un motor de renderizado 3D interactivo desarrollado en C++ utilizando OpenGL y la librería de matemáticas GLM. 
La aplicación está estructurada para manejar de manera eficiente el renderizado y las interacciones del
entorno mediante una arquitectura jerárquica robusta.

# Desarolladores

- Erick Bajaña -----> Gestión de escenas, arquitectura del Sistema,  texturas dinámicas y migración a CMake para que sea SO independiente
- Anderson Pilataxi ----> Artista 3D a cargo de mejorar los modelos y separalos para lograr funcionalides específicas utilizando Blender
- Josue Carcelén -----> Desarollador de la lógica de objetos dinámicos como la apertura de puertas, la detección de luces para ON/OFF Y demás.
- Jorge Revelo ------> Desarollador de shaders para iluminación para implementar el modelo de iluminación de Phong en alta calidad.

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

Desde la raíz del proyecto (ejecución utilizando la GPU predeterminada del sistema)

```bash
cmake -B build -S .
cmake --build build
./build/ProyectoSimulador


```

Desde la raíz del proyecto (ejecución forzada utilizando GPU NVIDIA dedicada mediante PRIME Render Offload)

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

## Notas

- El proyecto es independiente del Sistema Operativo y del IDE utilizado. La compilación y generación del ejecutable se gestionan mediante **CMake**, por lo que no depende de una configuración específica del entorno de desarrollo.

- El proyecto puede compilarse completamente desde consola utilizando únicamente el archivo `CMakeLists.txt` ubicado en la raíz del proyecto.

- **Visual Studio (Windows):**
  - No abrir el proyecto como una solución (`.sln`) ni crear manualmente un proyecto de Visual Studio.
  - Abrir directamente la carpeta raíz del proyecto mediante:
    ```
    File → Open → Folder
    ```
  - Visual Studio detectará automáticamente el archivo `CMakeLists.txt` y configurará el proyecto como un proyecto basado en CMake.
  - Desde la interfaz de Visual Studio se podrá seleccionar la configuración de compilación (`Debug` o `Release`), la arquitectura (`x64`) y ejecutar el proyecto directamente.
  - En caso de utilizar librerías instaladas mediante **vcpkg**, asegurarse de abrir Visual Studio con la configuración correcta del toolchain de CMake:
    ```
    -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
    ```

- **Visual Studio Code (Linux/Windows):**
  - Instalar las extensiones recomendadas:
    - **C/C++** (Microsoft)
    - **CMake**
    - **CMake Tools**
  - Abrir únicamente la carpeta raíz del proyecto, donde se encuentra el archivo `CMakeLists.txt`.
  - CMake Tools detectará automáticamente la configuración del proyecto y permitirá:
    - Configurar el compilador.
    - Generar la carpeta `build`.
    - Compilar.
    - Ejecutar y depurar el proyecto desde el editor.
  - También es posible realizar todo el proceso desde la terminal integrada utilizando los comandos indicados anteriormente.

- **CLion:**
  - Abrir directamente la carpeta raíz del proyecto.
  - CLion detectará automáticamente el archivo `CMakeLists.txt` y configurará el proyecto.
  - No es necesario importar archivos adicionales ni generar proyectos manualmente.
  - La compilación, ejecución y depuración se gestionan mediante la configuración de CMake integrada en el IDE.
  - En Windows, si las dependencias fueron instaladas mediante **vcpkg**, configurar el toolchain correspondiente en:
    ```
    Settings → Build, Execution, Deployment → CMake → Toolchain
    ```

- **Otros IDEs compatibles:**
  - Cualquier entorno que soporte proyectos basados en CMake puede utilizarse sin modificaciones adicionales.
  - Basta con abrir la carpeta raíz del proyecto y permitir que el IDE configure el proyecto utilizando el archivo `CMakeLists.txt`.

- Para evitar problemas con la selección de GPU:
  - En equipos con múltiples tarjetas gráficas (por ejemplo, laptops con GPU integrada + NVIDIA dedicada), verificar que el sistema operativo esté utilizando la GPU deseada para ejecutar el simulador.
  - En Windows esto se configura desde:
    ```
    Configuración → Sistema → Pantalla → Gráficos
    ```
    agregando `ProyectoSimulador.exe` y seleccionando la GPU preferida.
  - En Linux, utilizar las variables de entorno correspondientes para ejecutar con la GPU dedicada NVIDIA o AMD.

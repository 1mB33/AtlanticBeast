# Table of content

- [About the project](#atlanticbeast)
- [About demo](#about-demo)
- [Physics showcase](#demos-physics-showcase)
- [Screenshots](#screenshots)
- [Demo keybinds](#current-demo-keybinds)
- [Building the project](#building-the-project)

# AtlanticBeast

Realtime GPU ray tracer with custom acceleration structure based on a simplistic voxel grid in vulkan.<br>
Project is separated into several libraries. For demonstration purpose a Minecraft alike demo game was created.

### Engine libraries:
- Core (Fundamental functionality, including debugging tools, correct choosen system headers)
- Application (Windows managment, input managment, etc.)
- Math (Vector math, primitive geometrical structures, matrices and vector struckts)
- Rendering (Vulkan C++ wrappers, rendering logic, shaders and tracer renderer)


Project uses voxel traversal algorithm implementation based on 
'A Fast Voxel Traversal Algorithm for Ray Tracing'
by John Amanatides and Andrew Woo (1987) and Blinn-Phong Reflection lighting.


Implements a custom cross-platform window management system with runtime switchable behavior.
Features a low-latency input system and a fully custom Vulkan-based compute rendering pipeline.

# About demo

In demo we can play a simple minecraft lookalike game where you can place and remove blocks. Uses Jolt Physics to enhance the experience and showcase functionality. 


# Demos physics showcase
<p float="left">
  <img src="https://github.com/Im-Bee/AtlanticBeast/blob/master/Docs/PROMO_1.gif?raw=true" alt="Screenshot" width="98%"/>
</p>
<p float="left">
  <img src="https://github.com/Im-Bee/AtlanticBeast/blob/master/Docs/PROMO_2.gif?raw=true" alt="Screenshot" width="98%"/>
</p>

# Screenshots
<p float="left">
  <img src="https://github.com/Im-Bee/AtlanticBeast/blob/master/Docs/DocScreen01.jpg?raw=true" alt="Screenshot" width="49%"/>
  <img src="https://github.com/Im-Bee/AtlanticBeast/blob/master/Docs/DocScreen02.jpg?raw=true" alt="Screenshot" width="49%"/>
</p>
<p float="left">
  <img src="https://github.com/Im-Bee/AtlanticBeast/blob/master/Docs/DocScreen03.jpg?raw=true" alt="Screenshot" width="49%"/>
  <img src="https://github.com/Im-Bee/AtlanticBeast/blob/master/Docs/readmeSrceenshot_6.jpg?raw=true" alt="Screenshot" width="49%"/>
</p>																
<p float="left">													
  <img src="https://github.com/Im-Bee/AtlanticBeast/blob/master/Docs/DocScreen05.jpg?raw=true" alt="Screenshot" width="49%"/>
  <img src="https://github.com/Im-Bee/AtlanticBeast/blob/master/Docs/DocScreen06.jpg?raw=true" alt="Screenshot" width="49%"/>
</p>

# Current demo keybinds:

### Movement:
W - Move forward<br>
S - Move backward<br>
D - Strafe right<br>
A - Strafe left<br>
E - Fly up<br>
Q - Fly down<br>
Mouse movement - camera rotation<br>

### Interactions:
SPACE or LEFT MOUSE BUTTON - Place a block<br>
X or RIGHT MOUSE BUTTON - Remove a block<br>
1 - Weak push<br>
2 - Medium push<br>
3 - Strong push<br>
Z - Debug view<br>


# Building the project

Build dependencies:
  - Vulkan SDK
  - CMake or Visual Studio with CMake support
  - Compiler that supports at least C++17
  - Windows or Linux operating system
  - X11 libraries [Only on linux]

## Linux

### Get necessary libraries and headers 

### Arch linux based systems
``` sh
sudo pacman -S --needed cmake make libx11 libxrandr libxinerama libxcursor libxi vulkan-tools vulkan-headers vulkan-utility-libraries glslang spirv-tools
```

### Ubuntu/Debian based systems
``` sh
sudo apt update && sudo apt install -y cmake make libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev vulkan-tools libvulkan-dev vulkan-utility-libraries-dev glslang-tools spirv-tools
```

### Download and build the project

``` sh
git clone --recurse-submodules -j8 https://github.com/Im-Bee/AtlanticBeast.git &&
cd AtlanticBeast &&
mkdir Build &&
cd Build &&
cmake .. -DCMAKE_BUILD_TYPE=Release &&
cmake --build .
```

Run project from a bin directory that is created in root directory of the project **Bin/TARGET_ARCH/BUILD_TYPE/**.

## Windows

Download Vulkan SDK from a place like [LunarG](https://vulkan.lunarg.com/sdk/home).
If you are using Visual Studio make sure to have CMake support installed.

### Visual Studio 

Open root directory of the project with Visual Studio. Wait for cmake to build. Select AtlBee project as a build target.
Prefer release build.

### CMake

``` batch
mkdir Build
cd Build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

Run project from a bin directory that is created in root directory of the project **Bin/TARGET_ARCH/BUILD_TYPE/**.



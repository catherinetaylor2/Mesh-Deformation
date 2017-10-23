# Surface-Based Mesh Deformation

![c1](https://cloud.githubusercontent.com/assets/25514442/24172564/92dcb04a-0e80-11e7-9d0f-f3a3ba9ca8bb.PNG)


Coursework for EngD computer animation and games 2 module. Carries out surface-based mesh deformation for a 2D triangular mesh.

### Dependencies:

To run successfully, this code must be linked to the following 4 libraries.

* OpenGl
* GLEW 
* GLFW3
* GLM

### How to compile:

CL on Windows:

     cl /O2  /EHsc   mesh_deformation.cpp shader.cpp  read_obj.cpp /openmp /link /NODEFAULTLIB:"LIBCMT"  "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\lib\glfw3.lib" "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\lib\glew32.lib" "C:\Program Files (x86)\Windows Kits\8.1\Lib\winv6.3\um\x86\OpenGL32.Lib"  "user32.lib" "gdi32.lib"  "shell32.lib" "msvcrt.lib" 

Clang on Linux:

    clang++ -std=c++11 -I/usr/include -lGLEW -lglfw -lGL Mesh_Deformation.cpp readObj.cpp readObj.hpp shader.cpp shader.hpp


### To Run:

* Left click to select fixed vertices and handles.
* Right click to choose goal position.
* Enter to reset.




 

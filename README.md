# Mesh Deformation

![c0](https://cloud.githubusercontent.com/assets/25514442/24153203/b5e0d6e2-0e45-11e7-854b-72ecb5b13b7b.PNG)

Coursework for computer animation and games module. Carries out surface-based mesh deformation for a 2D triangular mesh.

### Dependencies

To run, this code must be linked to the following 5 libraries.

* OpenGl
* GLEW 
* GLFW3
* GLM
* Eigen

### How to compile

CL:

     cl /O2  /EHsc   mesh_deformation.cpp shader.cpp  read_obj.cpp /openmp /link /NODEFAULTLIB:"LIBCMT"  "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\lib\glfw3.lib" "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\lib\glew32.lib" "C:\Program Files (x86)\Windows Kits\8.1\Lib\winv6.3\um\x86\OpenGL32.Lib"  "user32.lib" "gdi32.lib"  "shell32.lib" "msvcrt.lib" 

### To Run:

* Left click to select fixed vertices and handles.
* Right click to choose goal position.
* Enter to reset.




 

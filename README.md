## Interactive Computer Graphics
This repository gradually steps through the essentials in an interactive computer graphics application. Topics include rasterizers, OpenGL shaders, mesh data structure, GUIs, game engine, etc. The culmination is a interactive game "Mini-Minecraft".

All projects are written in C++ using Qt 5 framework. Each folder should be self-contained.

---------
### 1. Linear Algebra Library
This is a refresher exercise of C++, in which a compact library is developed in the style of GLSL (OpenGL Shading Language). Transformation is heavily involved in computer graphics.

### 2. Scene Graph
This is an exercise meant for getting familiar with transformations, scene graph traversal, and using Qt 5 GUIs.
 
### 3. Rasterizer
A rasterizer is implemented to simulate what's happening in shaders on the GPU side, which actually runs on CPU. Main features include:

- Convex polygon triangulation
- Barycentric interpolation, perspective-correct interpolation
- Z-buffering
- Lambertian reflection
- Perspective, interative camera
- Clever tricks for concave polygons triangulation

and many other features.

![alt text](https://raw.githubusercontent.com/DianCh/Interactive_Computer_Graphics/master/3_Rasterizer/result_images/2D_maple_leaf.bmp "concave")

![alt text](https://raw.githubusercontent.com/DianCh/Interactive_Computer_Graphics/master/3_Rasterizer/result_images/3D_wahoo.bmp "wahoo")

### 4. OpenGL Fun
Here we are getting into OpenGL shaders. This mainly walks through the pipeline of creating shader programs at CPU side and manipulating vertex shaders and fragment shaders at GPU side. Some fun transformations are ahieved.

![alt text](https://github.com/DianCh/Interactive_Computer_Graphics/blob/master/4_OpenGL_Fun/demo.png "fun")

### 5. OpenGL Even More Fun
Here we continue to play with OpenGL shaders. This time we retrieve what's rendered in the render buffer and use that as another texture to perform a second post-processing. In this way some even more fun effects can be achieved such as worley bubbles, worley mosaic and TV glitch.

![alt text](https://github.com/DianCh/Interactive_Computer_Graphics/blob/master/5_OpenGL_Even_More_Fun/demo1.png "fun1")

![alt text](https://github.com/DianCh/Interactive_Computer_Graphics/blob/master/5_OpenGL_Even_More_Fun/demo2.png "fun2")

### 6. Half Edge Mesh
This is a three-week project in which I've incrementally added functions to the application. Half edge mesh data structure is utilized intensively and I managed to efficiently traverse the entire mesh using QMap and some clever tricks.

![alt text](https://github.com/DianCh/Interactive_Computer_Graphics/blob/master/6_Half_Edge_Mesh/demo.png "mesh")

### 7. Mini-Minecraft
This is a teamwork by 3 people in three weeks. No more talks! Try it!


-------------------------

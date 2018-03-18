Author: Dian Chen
Hw03 for CIS560 17fall

IMPORTANT: 
Because the coordinates provided in 2D case and 3D case are different, I’ve implemented separate rasterizers for 2D and 3D respectively. 

If you want to test 2D, please go to mainwindow.cpp line 51 and comment out “rendered_image = rasterizer.RenderScene3D(myCamera);” and use line 52 instead. Use key “B” to load models. 

If you want to test 3D, please go to mainwindow.cpp line 52 and comment out “rendered_image = rasterizer.RenderScene();” and use line 51 instead. Use key “space” to load models.

Result pictures:
Please see the pictures for 2D, 3D and concave triangulation.

=============2D Triangle Rasterization Features======================
1. Convex polygon triangulation
In polygon.cpp, line 15 to 22

2. Line segments
Please check lineseg2d.h and lineseg2d.cpp files

3. Bounding box
In rasterizer.cpp, used both in:

Renderscene(), starting from line 45, relevant variables are boundXLeft, boundXRight, boundYUpper and boundYLower, and these are further used in startY, endY, startX and endX.

Renderscene3D(), starting from line 158, relevant variables are boundXLeft, boundXRight, boundYUpper and boundYLower, and these are further used in startY, endY, startX and endX.

4. Triangle rendering
Renderscene() starting from line 7 in rasterizer.cpp

5. Barycentric interpolation
The function ComputeBaryCords2D() is implemented in polygon.cpp starting from line 130.

6. Z-buffering
In 2D case the z-buffering is used in RenderScene() at line 28, 29 and line 99 to 101.

In 3D case the z-buffering is used in RenderScene3D() at line 133, 134 and line 228 to 231.

=============3D Triangle Rasterization Features======================
1. Perspective Camera class
Please check camera.h and camera.cpp files

2. Interactive camera
The functionality is achieved Camera class’s moveForward, moveRight, moveUp, rollZ, pitchX, yawY. And key events are linked to these functions in mainwindow.cpp starting from line 35 to line 47.

Detailed description:
W: move camera to its own forward 0.5 units
A: move camera to its own left 0.5 units
S: move camera to its own backward 0.5 units
D: move camera to its own right 0.5 units
E: move camera to its own up 0.5 units
Q: move camera to its own down 0.5 units

(Notice that the positive degree of rotation is defined using left hand, because camera frame is a left-handed frame.)
I: rotate around X axis -2.5 degrees. equivalent to tilt your head backwards
J: rotate around Y axis 2.5 degrees. equivalent to turn your head right
K: rotate around X axis 2.5 degrees. equivalent to tilt your head backwards
L: rotate around Y axis -2.5 degrees. equivalent to turn your head left
U: rotate around Z axis -2.5 degrees. equivalent to tilt your head to the right
O: rotate around Z axis 2.5 degrees. equivalent to tilt your head to the left

3. Texture mapping
Used in rasterizer.cpp starting from line 234 to 260. 

4. Perspective-correct interpolation
Used in rasterizer.cpp starting from line 219 to 225 and interpolation starting from line 234 to 246.

5. Lambertian reflection
Used in rasterizer.cpp starting from line 249 to 260.

=============Extra Credits===========================================
1. Concave polygon triangulation
Implemented in polygon.cpp EarClipTriangulate, with 4 other helper functions: FindMeAConvexPoint, ProjectToXY, isConvexPoint and isFeasible.

This functionality is successfully tested using a custom json file 2D_concave_polygon.json. And the ones provided in the package can also be triangulated in a way different than “fan” triangulation. Please see the outcome picture for visual effects. 

When testing this, please make sure that you:
1) Go to mainwindow.cpp line 51 and comment out “rendered_image = rasterizer.RenderScene3D(myCamera);” and use line 52 instead.
2) Use key “B” to load models. 
3) Go to polygon.cpp line 32 and line 54 and comment out the original “Triangulate();” and use “EarClipTriangulate();” instead.


Steps explanation:
1) For any given set polygon vertices (given that they are numbered counter-clockwise, such that the direction of front face norm is determined), first project them onto the plane that contains them all. For convenience the plane is set to XY plane. In order to do this we first pick 3 random vertices to compute the cross product and use it as the Z axis (may point to the positive or negative front face norm). We use Rodriguez matrix to do the conversion.
Related function: ProjectToXY

2) Now we are viewing all the vertices in their own plane. Pick the one with the largest x coordinate. This one is guaranteed to be a convex point.
Related function: FindMeAConvexPoint

3) The ear-clipping is only achievable for convex point. For a concave point we just jump to the next one.
Related function: isConvexPoint

4) For a convex point, the triangulation is only feasible if the triangle formed by itself and its two neighbors doesn’t include any other points. To test if other points lie inside this triangle we again use barycentric coordinates to see if they sum up to one. If they all sum up to greater than 1 for all other points then this triangle is a valid, feasible one.
Related function: isFeasible

5) Loop over all indices and continue the triangulation until there are no more than 3 vertices.
Main function: EarClipTriangulate

The idea of ear-clipping is learned online from this link: https://www.gamedev.net/articles/programming/graphics/polygon-triangulation-r3334/
The other helper function steps 1) to 4) are my own ideas.
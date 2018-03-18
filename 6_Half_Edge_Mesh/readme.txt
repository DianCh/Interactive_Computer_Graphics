=============================Homework 8 readme==============================
Author: Dian Chen
Hw08 for CIS560 17fall
Platform: macOS Sierra 10.12

===========================Joint Class========================

1. The Joint class is implemented in two new files “joint.h” and “joint.cpp”.

2. To draw the skeleton, a Skeleton class that inherits Drawable class is implemented in “skeleton.h” and “skeleton.cpp”. The Skeleton class contains a vector of pointers to the actual joints, a pointer to the root joint and a static vector of positions used in drawing wireframes.

===================Skeleton JSON File Reader==================

1. The reader is implemented in Skeleton class, specifically, Skeleton::setupJSON. This function first calls the utility function Skeleton::clearSkeleton which frees all the existing joints on the heap and then recursively calls Skeleton::setupOneJoint and creates new joints on the heap.

2. There’s a new button “load JSON” added to the GUI that triggers the file loading upon clicking.

=================Joint Influences on Vertices=================

1. Two member variable “jointIDs” and “jointWeights” are added to the Vertex class in “primitives.h” and “primitives.cpp”.

2. At the time of initialization or loading new meshes, the bind matrices and transformation matrices are set to identity matrices and sent to the VBOs, and the IDs and weights of vertices are set to (0, 0) and (1, 0) which make sense if the mesh is not yet affected by any skeleton.

=======================Skinning Function======================

1. The skinning function is implemented in Mesh class, specifically, Mesh::skinMesh. Given a skeleton, this function assigns proper IDs and weights to each of its vertex. I decided to only support influences from two vertices and linear blending.

2. A new button is added to the GUI. If there’s no skeleton currently loaded, clicking the button will not cause anything except simply bringing up a message that tells you no skeleton is loaded.

=====================Interactive Skeleton=====================

1. A bunch of new buttons are added to the GUI which are self-explanatory. The angle of rotation and distance of translation can be specified in the double-spinboxes.

2. The lines connecting joints have bright gold color on the parent joints, and gradually change to purple towards the child joints. The wireframes are cyan, the selected one being bright gold.

3. To make file loading more logical and natural, you’re allowed to load a new mesh and a new skeleton and bind again (click binding button again). Loading new JSON files or new OBJ files will break the binding (reset flags, bind and transformation matrices).

4. However, the program doesn’t support mesh operation after binding. That is, once a skeleton is bound to a mesh, any new vertices created from mesh operation will have whimsical behaviors that don’t make much sense.

=================Shader-based Skin Deformation================

1. Two shaders are added, as well as the proper handles, member variables and member functions.

2. Since light shadowing is complicated and not explained in the lecture, I decided to only support flat shading.




=============================Homework 7 readme==============================
Author: Dian Chen
Hw07 for CIS560 17fall
Platform: macOS Sierra 10.12

=======================Polar Spherical Camera=================

1. Two new functions are implemented in Camera class to achieve the spherical movement, which follow the interpretation of the member variables as the old functions.

2. To follow the old intepretations, a new member variable “world_right” is added which functions similarly as the “world_up” variable

=====================Catmull-Clark Subdivision================

1. The subdivision is implemented in Mesh::catmullclarckSubdivision, which will call two utility functions, Mesh::quadrangulateAFace that quadrangulates a particular face given a centroid, and Mesh::smoothVertices that smooth the position of all old vertices.

2. The process is well commented along the codes. You can look at the comments for fine details.

3. Subdividing a cow can be done almost instantly on my computer, right after a click.

=========================Extruding Faces======================

1. The face extrusion is implemented in Mesh::extrudeAFace, which can extrude a face by a distance given by the GUI.

2. The process is well commented along the codes. You can look at the comments for fine details.

========================OBJ File Importing====================

1. The OBJ file importing is implemented in Mesh::setupOBJ. This function will first call a utility function Mesh::clearPrimitives to clear all the faces, vertices and half edges the mesh currently owns on the heap, and then sets up the new primitives and their data structure according to the position/uv/norm OBJ file.

2. After this function is called, the actual slot in MyGL can set up the new pointers and VBOs like it did in initialization.

3. The process is well commented along the codes. You can look at the comments for fine details. A main feature is the usage of QMap.

4. Importing a cow OBJ file can be done within 1 second on my computer (the cube and dodecahedron almost instantly).

5. The file path I used in filename creation is “QString(":/")” since I’m a Mac user. You can change it in grading if any compatibility issue occurs.

========================Extra Credits=========================

1. Custom mesh operation — central subdivision: this is a mesh operation that I came up with, in which you can choose a face, and make a pointed rooftop like division from it. Specifically, given a N-gon:

1) Find its central point (the position is the average of all vertices).
2) Move that point along the surface normal by a distance given by the GUI.
3) Connect that point to all the vertices, from which N tilted facades are formed like a pointed rooftop.

The implementation is Mesh::centralSubdivision. The process (including the data structure setup) is well commented along the code.

2. GUI improvement:

1) You’re allowed to modify a face’s color.
2) You’re allowed to set the extruding distance and the height of the rooftop (EC 1).



=============================Homework 6 readme==============================
Author: Dian Chen
Hw06 for CIS560 17fall

=======================Mesh component classes=================

1. The Halfedge, Vertex, Face classes are put in “primitive.h” and “primitive.cpp”.

2. The ID of each instance (and thus their name to appear as a QWidgetListItem) is automatically taken care of by using a static member variable to keep count.

============================Mesh Class========================

1. The Mesh class is put in “mesh.h” and “mesh.cpp”

2. The norms are set up like this: 
For each face, we try to find a valid norm. The two vectors taking cross product to get this norm should not be parallel. This is to avoid the situation in which we add vertices successively to the same half edge and the starting point may have an invalid norm (the neighbor half edges are parallel).
Traverse all the half edges to compute the norms until we find a valid one. We use this same valid norm for each vertex belonging to this face. If the dot product are close enough to 1, the norm is treated as invalid.

===================Graphical User Interface===================

1. The classes to draw the selected elements are DrawableHalfEdge, DrawableVertex and DrawableFace, all put in “highlights.h” and “highlights.cpp”.

2. At setup, the selected half edge, vertex and face are by default initialized to the 0th one in the vector container of the mesh.

3. Click the items and the corresponding element is selected and highlighted. The coordinates of a vertex or the color of a face are changed to current values in the spinboxes correspondingly upon selection, and they can be changed (no smaller than -100 for a vertex coordinate and no greater than 1.0 for a face color).

4. After adding a vertex, or triangulating a face, the new vertices, half edges and faces will show up in the GUI list.

===================Visual Debugging Tools=====================

1. The classes to draw the selected elements are DrawableHalfEdge, DrawableVertex and DrawableFace, all put in “highlights.h” and “highlights.cpp”.

2. Key events setup is in MyGL class.

===================Topology Editing Functions=================

1. The function to add a new vertex in the middle is Mesh::addANewVertex. Related signals and slots are also defined in MainWindow and MyGL to make it function properly.

2. The function to triangulate a face is Mesh::triangulateAFace. Related signals and slots are also defined in MainWindow and MyGL to make it function properly.

========================Extra Credits=========================

1. The planarity test is implemented in Face::needTriangulation, and used at line 164-170 in Mesh::create before VBO setup. If you modify the vertex position of a planar square, you can immediately see the triangulated face.

The reasoning goes like this:
Any three points can form a plane, whose equation can have the form: x + Ay + Bz + C = 0. For any given face, we solve for A, B and C using the first three points from the known half edge, and for the remaining points we plug in their coordinates to the equation and examine if the results differs from 0 (by EPSILON for floating point comparison). If one is found then the face needs triangulation.

Note that when displacing a vertex of a square, the bending ridge should lie in between the displaced vertex and the the other side. In my way of setting up the fan triangulation indices, the ridge of the bending face is guaranteed to be the proper one.

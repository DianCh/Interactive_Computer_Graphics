Author: Dian Chen
Hw05 for CIS560 17fall

1. Greyscale shader, Gaussian blur shader, Sobel filter shader and fake bloom shader are implemented as required. Nothing particular about these shaders.

2. Custom noised-based shaders are:

=========Worley bubble shader (worleywarp.frag.glsl)==============

This shader uses worley noise to generate spheres (see line 64) that center around the random control point in each cell. Spheres touching each other makes bubble-like surface.

Height at each fragment and its neighbors is computed, and then the gradients (see line 78 to 85), which finally lead to surface normal (see line 88).

Surface normal is used for:
1) Faking Lambertian lighting (see line 90 to 103)
2) Faking Blinn-Phong effect (see line 90 to 103)
3) Computing (using Lambertian dot product) color palette to fake “rainbow” effect (see line 108 to 119)
4) Faking light refraction through bubbles by displacing uv coordinates (see line 106)

Random, smooth movement of the control points are added to make the bubbles move (see line 43 to 48).

============Worley mosaic shader (mosaic.frag.glsl)================

This shader uses worley noise. Each fragment finds its control point’s coordinate and use that coordinate to color itself (see line 58, 71, 74).

==============TV glitch shader (tvglitch.frag.glsl)================

This shader simulates the old tv glitches, in which some part of the screen will go sideways once a while.

Displacement is made within horizontal bands (see line 20 to 28) at different time intervals (see line 37 to 41) to make that visual effect.

# FdF
## ✨ Features
**Simple wireframe model software renderer.**
- Implements a software-based geometry pipeline similar to that of modern GPUs
- Uses the Liang-Barsky algorithm for line clipping and Bresenham’s algorithm for rasterizing the lines
- Follows an OpenGL-style right-handed coordinate convention, where Y is up, X is to the right, and in view space the camera looks down -Z
- Includes an orbiting camera controller that emulates the navigation style of 3D modeling software like Blender

<p align="center">
  <img src="doc/42.gif" alt="Demo" />
</p>

## 📖 Introduction

This is my favorite project from the 42 curriculum so far. I'm very excited to learn more about graphics programming, and I took extra time to complete this project. I had two clear goals in mind:

- To study the geometry processing pipeline of GPUs and build something that resembles it
- To implement a smooth orbiting camera controller, similar to the one used in Blender

This meant I also had to brush up on my knowledge of matrices and linear algebra. While we were allowed to use functions from `math.h`, I had to implement a number of additional utilities to support the program including matrix operations, 3D vector structures (vecs), and dynamically sized arrays (vectors).

The mandatory part of the project involved parsing a map file and rendering it in an isometric view: an orthographic projection from a specific camera angle.

For the bonus portion, several additional features were required:
- A different projection
- Translation
- Rotation
- Zooming
- One extra feature chosen by the programmer

At first, I didn’t implement proper clipping. I simply rejected a line segment entirely if either of its vertices were behind the camera in view space, to avoid division by zero during the perspective divide. However, after failing my first evaluation due to not closing a file descriptor on all error paths, I was given time to fix the issue and decided to improve other aspects of the project as well.

I attempted to implement the Cohen–Sutherland clipping algorithm in clip space, but quickly realized it’s not suitable for use with homogeneous coordinates. The algorithm got stuck in an infinite loop because it doesn't handle the w component or the 3D view frustum correctly. I switched to Liang–Barsky, which is not only more intuitive but also directly considers the w component, making it much more appropriate for 3D clipping in clip space. I also wrote a screen-space variant as a safeguard.

One interesting problem I encountered was an optical illusion, especially visible when using orthographic projection: at certain camera angles, the model appeared to flip direction. I spent hours debugging, convinced I had a sign error or an incorrect matrix element in my view or projection matrix. Only after discussing it with a couple of peers and being shown the spinning ballerina illusion, which can appear to spin clockwise or counterclockwise depending on the viewer, I understood that the illusion was in my perception.

To confirm that everything was working correctly, I implemented depth testing. Seeing correct pixel overwrites (where nearer geometry occluded farther geometry) gave me confidence that the model was being rendered and rotated properly, even if my brain occasionally failed to interpret its orientation.

Possible further improvements:
- Store line segments in a dedicated container for rendering to avoid duplicates (currently, for tringles in the last row or last column, edges are drawn for both triangles to ensure the boundary lines are rendered)
- Switch to quaternions for more consistent and stable rotations
- Refine panning behavior for better consistency across different projection modes

> [!IMPORTANT]
> [MLX42](https://github.com/codam-coding-college/MLX42), a minimal graphics library, is used in this project. It handles window creation and manages the framebuffer.
>
> The `Makefile` will download and build MLX42 for use in the program, but it requires certain dependencies to be installed, depending on your system. Please refer to their [installation documentation](https://github.com/codam-coding-college/MLX42?tab=readme-ov-file#for-linux) for more information.
>
> - (Optional) The project files include `mlx42.supp`, a suppression file to be used with valgrind. It will ignore the memory leaks originating from MLX42.

> [!TIP]
> ## 🚀 How to use
Run the following commands to clone the repository and create `fdf` program
``` git
git clone https://github.com/mordori/fdf.git fdf
cd fdf
make -j4
```
Execute the program with a map file as a parameter, for example
``` C
./fdf maps/test.fdf
```
To delete all of the compiled files and MLX42, use
``` Makefile
make fclean
```

> [!TIP]
> ## 🎮 Controls
> Isometric view is locked. To enable controls, switch to other view/projection using `P`.

| Input					| Description														|
|-----------------------|-------------------------------------------------------------------|
| `P`                 	| Switch view/projection (isometric -> orthographic -> perspective)	|
| `Left Mouse Button`	| Orbit camera														|
| `Middle Mouse Button`	| Pan camera														|
| `Right Mouse Button`	| Zoom camera in/out												|
| `W`, `A`, `S`, `D`	| Traslate object in X and Z axes									|
| `Arrow Keys`			| Rotate object around X and Z axes									|
| `R`					| Reset object and camera	transforms 										|
| `F`					| Frame model to fit window											|
| `C`					| Toggle rainbow color mode											|
| `SPACE`				| Toggle spinning mode												|
| `U`, `I`				| Decrease/increase camera FOV in perspective projection			|
| `Esc`					| Exit program														|

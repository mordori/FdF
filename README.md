# FdF
## ✨ Features
Simple wireframe model software renderer with limited file parsing.
- Implements a software-based geometry pipeline similar to that of modern GPUs.
- Uses the Liang-Barsky algorithm for line clipping and Bresenham’s algorithm for rasterizing the lines.
- Follows an OpenGL-style right-handed coordinate convention, where Y is up, X is to the right, and in view space the camera looks down -Z.
- Includes an orbiting camera controller that emulates the navigation style of 3D modeling software like Blender.

## 📖 Introduction

This is my favorite project from the 42 curriculum so far. I'm very excited to learn more about graphics programming, and I took extra time to complete this project. I had two clear goals in mind:

- To study the geometry processing pipeline of GPUs and build something that resembles it.
- To implement a smooth orbiting camera controller, similar to the one used in Blender.

This meant I also had to brush up on my knowledge of matrices and linear algebra. While we were allowed to use functions from `math.h`, I had to implement a number of additional utilities to support the program including matrix operations, 3D vector structures (vecs), and dynamically sized arrays (vectors).

The mandatory part of the project involved parsing a map file and rendering it in an isometric view: an orthographic projection from a specific camera angle.

For the bonus portion, several additional features were required:
- A different projection
- Translation
- Rotation
- Zooming
- One extra feature chosen by the programmer

> [!IMPORTANT]
> [MLX42](https://github.com/codam-coding-college/MLX42), a minimal graphics library, is used in this project. It handles window creation and manages the framebuffer.
> 
> The `Makefile` will download and build MLX42 for use in the program, but it requires certain dependencies to be installed, depending on your system. Please refer to their [installation documentation](https://github.com/codam-coding-college/MLX42?tab=readme-ov-file#for-linux) for more information.
>
> - (Optional) The project files include `mlx42.supp`, a suppression file to be used with valgrind. It will ignore the memory leaks originating from MLX42.

> [!TIP]
> ## 🚀 How to use
- Clone the repository
``` git
git clone https://github.com/mordori/fdf.git fdf && cd fdf
```
- Run the following command to create `fdf`
``` Makefile
make
```
- Execute the program with a map file as a parameter, for example
``` C
./fdf maps/test.fdf
```
- To delete all of the compiled files and MLX42, use
``` Makefile
make fclean
```

> [!TIP]
> ## 🎮 Controls

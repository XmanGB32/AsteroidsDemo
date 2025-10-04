# Asteroids Demo

A simple Asteroids-like game implemented in C++ using OpenGL, GLFW, GLAD, and GLM. Control a spaceship, avoid and destroy asteroids in a wrap-around space environment.


![ASTEROIDS SnipJPG](https://github.com/user-attachments/assets/f5dde39a-a298-40e1-b672-4d11bd4dec47)


*(Example screenshot of a similar OpenGL-based Asteroids game)*

## Description

This project is a basic clone of the classic Asteroids arcade game. It features a player-controlled ship that can rotate, thrust, and fire bullets (referred to as missiles in the code comments). Asteroids drift across the screen, and the game handles collisions between the ship and asteroids (resetting the ship on impact) as well as bullets destroying asteroids. The screen wraps around like a torus, providing endless movement. Bullets have a limited lifespan and inherit the ship's velocity for realistic physics.

The game is rendered using simple line drawings for the ship (a triangle) and asteroids (decagons approximating circles), with bullets as points. It's a great starting point for learning OpenGL basics, 2D game physics, and input handling.

## Features

- **Ship Controls**: Rotation, thrust with inertia, and friction for smooth movement.
- **Shooting**: Fire bullets in the direction the ship is facing, with a cooldown to prevent spamming.
- **Asteroids**: Random initial positions and velocities; destroyable by bullets.
- **Collisions**: Circle-based detection for ship-asteroid and bullet-asteroid interactions.
- **Screen Wrapping**: Objects wrap around the edges of the window for a seamless play area.
- **Rendering**: Uses shaders for transformations; dynamic vertex buffers for drawing.

## Requirements

- **Compiler**: C++11 or later (tested with MSVC in Visual Studio 2022).
- **Libraries**:
  - GLFW (for window and input management).
  - GLAD (OpenGL loader).
  - GLM (for vector math and transformations).
- **OpenGL**: Version 3.3 core profile.

Ensure the libraries are installed and configured in your project. For Visual Studio, add include directories and link against the .lib files.

## Building the Project

### Using Visual Studio 2022

1. Create a new Empty C++ Project.
2. Add the source file (e.g., `main.cpp`) containing the provided code.
3. Download and set up dependencies:
   - GLFW: Download pre-built binaries from [glfw.org](https://www.glfw.org/download.html) and add to your project.
   - GLAD: Generate and include the GLAD files from [glad.dav1d.de](https://glad.dav1d.de/).
   - GLM: Download from [glm.g-truc.net](https://glm.g-truc.net/) and include the headers.
4. Configure project properties:
   - Add include paths for GLFW, GLAD, and GLM.
   - Link against `glfw3.lib` and `opengl32.lib`.
5. Build the solution (Debug or Release mode).
6. Run the executable.

If you're using vcpkg for package management:
- Install dependencies: `vcpkg install glfw3 glad glm`.
- Integrate vcpkg with VS: `vcpkg integrate install`.

## Usage

Run the compiled executable. The game window will open with the ship in the center and a few asteroids floating around.

### Controls

- **Thrust**: `W` or `Down Arrow` - Accelerate forward.
- **Turn Left**: `A` or `Left Arrow` - Rotate counterclockwise.
- **Turn Right**: `D` or `Right Arrow` - Rotate clockwise.
- **Shoot**: `Space` - Fire a bullet (with 0.2s cooldown).
- **Quit**: `Esc` - Close the window.

Destroy all asteroids by shooting them. Colliding with an asteroid resets the ship to the center.

## Code Structure

- **Main Loop**: Handles input, updates positions/velocities, checks collisions, and renders.
- **Structures**: `Object` for ship/asteroids, `Bullet` for projectiles.
- **Shaders**: Basic vertex and fragment shaders for white line/point drawing.
- **Global Variables**: Ship, asteroids vector, bullets vector, etc.

## Improvements and Extensions

- Add more asteroids or spawning on destruction.
- Implement scoring, lives, or levels.
- Add sound effects or textures for better visuals.
- Handle multiple bullet-asteroid collisions more efficiently.

## License

This project is open-source and available under the MIT License. Feel free to modify and distribute.

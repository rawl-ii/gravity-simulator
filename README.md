# Gravity Simulator

A real-time 3D N-Body gravity simulation built with OpenGL. This project focuses on simulating orbital mechanics and gravitational interactions between celestial bodies.

![gravity_simulator](https://github.com/user-attachments/assets/2d0448ac-2012-4758-b552-2693569fcf32)

## Controls
* **WASD:** Move your camera.
* **Left shift:** Decrease camera speed.
* **Mouse:** Look around.
* **ESC:** Free your cursor.

## Features
* **Real-time physics:** Accurate gravitational pull calculations between bodies.
* **Custom Shaders:** Differentiated rendering for stars and planets.
* **JSON-driven scenes:** Easily create and share complex orbital systems.

## Configuration

You can customize the simulation behavior by modifying the JSON files in the `/config` directory:

![gravity_sim_config](https://github.com/user-attachments/assets/e6330853-201d-447c-848a-5b637a709245)

### Creating custom scenes
To create a custom scene, add a new JSON file, preferably to the `/scenes` directory, but it can be anywhere else, as long as you specify the path on the run command. 

![example_scenes](https://github.com/user-attachments/assets/5bf38821-c356-4fdc-b481-cf7164ebffd2)

#### Entity Properties:
| Property | Description |
| :--- | :--- |
| `position` | Array `[x, y, z]` defining the starting point in space. |
| `velocity` | Array `[vx, vy, vz]` for the initial velocity vector. |
| `mass` | Determines the gravitational pull and affects size. |
| `density` | Works with mass to determine the visual scale of the entity. |
| `color` | RGB values as normalized floats (0.0 to 1.0). |
| `isStar` | Boolean. If `true`, the entity uses a star-specific shader. |

**Example structure:**
```json
[
    {
        "position": [0.0, 0.0, 0.0],
        "velocity": [0.0, 0.0, 0.0],
        "mass": 1000.0,
        "density": 0.25,
        "color": [0.9, 0.55, 0.0],
        "isStar": true
    }
]
```

## How to run:
* CMake (3.22 or higher)
* OpenGL development headers
* GLFW 3 and PkgConfig (for library discovery)
* A C++ compiler with C++23 support (e.g., GCC 12+, Clang 15+, or MSVC 19.33+)

* Clone the repository:
```bash
git clone https://github.com/rawl_ii/gravity_simulatior.git
cd gravity-simulator
```
* Build it:
```bash
mkdir build && cd build
cmake ..
make
```
* Run the simulation from the build directory (required to load local config/scene files)::
```bash
./gravity_simulation /path/to/your_scene.json
```
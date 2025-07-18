# Planet Simulation

This project is a minimal OpenGL application written in C++ that simulates a planet orbiting a star using basic Newtonian physics.

## Building

The project uses CMake. You will need the following libraries installed on your system:

- [GLFW](https://www.glfw.org/) for window management
- [GLM](https://github.com/g-truc/glm) for mathematics
- [glad](https://glad.dav1d.de/) or another OpenGL loader

```bash
mkdir build
cd build
cmake ..
make
./planet
```

The simulation renders a sphere representing Earth orbiting a much more massive sun. It updates the planet's position every frame using the gravitational force between the two bodies.

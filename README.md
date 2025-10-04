# UAV simulator

This project is a UAV (Unmanned Areal Vehicle) simulator build in C++ using Raylib for graphics and Eigen for linear algebra.
The simulator is designed for manual control of a UAV using keyboard inputs, joystick inputs and custom control algorithms.

## Features

- Easily configurable UAV models, including physical properties and control parameters.
- Real-time simulation of UAV dynamics and control.
- Visualization of UAV state and environment using Raylib.
- Support for keyboard and joystick inputs for manual control.

## Requirements

- C++23 compatible compiler
- CMake 3.28.3 or higher
- [Eigen](https://eigen.tuxfamily.org/dox/GettingStarted.html) (header-only library)

CMake will automatically download and build the required dependencies:
- [Raylib](https://www.raylib.com/)
- [Json for Modern C++](https://github.com/nlohmann/json)

## Building the Project

```bash
git clone https://github.com/Fildo7525/RaylibSimulator.git
cd RaylibSimulator
make run
```


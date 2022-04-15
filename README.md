# Galaxy simulation
This is a fork, original project by: [**Angel Uriot**](https://github.com/angeluriot)

N-body type simulation using GPU acceleration. It is able to simulate galaxies, galaxy collisions and expanding universes. It has a menu to change the settings and an interactive camera.

⚠️ This repository contains **submodules**, add `--recurse-submodules` when cloning ⚠️

# Summary
* **[Install](#install)**
* **[Features](#features)**

# Install
## Extra libs
* Glew
* OpenCL with CUDA backend: 
```sudo apt-get install -y nvidia-opencl-dev```
## CMake
* Run unix script or build the CMakeList from IDE

# Features
The N-body sim is Newtonian

* A menu to change the settings of the simulation
* A camera that you can control with your mouse and your wheel
* It can simulate a unique galaxy :

	![](https://raw.githubusercontent.com/angeluriot/Galaxy_simulation/master/resources/misc/galaxy_2.png)

* It can also simulate a collision between two galaxies :

	![](https://raw.githubusercontent.com/angeluriot/Galaxy_simulation/master/resources/misc/galaxy_3.png)

* And it can simulate an entire expanding universe :

	![](https://raw.githubusercontent.com/angeluriot/Galaxy_simulation/master/resources/misc/galaxy_4.png)



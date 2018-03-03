## A lightweight Smoothed Particle Hydrodynamics

This is a header-only implementation of the Smoothed Particle Hydrodynamics.
Tested with valgrind, no memory leak.

#### Dependencies
On Ubuntu 16.04, install GLUT and OpenGL.
```bash
sudo apt install -y freeglut3-dev
sudo apt install -y mesa-utils
```

Tested by clang++ version 3.8.0 with C++14.

#### Running the demo

```bash
make 
./main
```

* Keyboard 'a' - Start animation.
* Keyboard space - Next Frame.
* Keyboard '1' - Dam break.
* Keyboard '2' - Water Drop.
* Keyboard '3' - Faucet.
* Keyboard 'g' - Switch Grid Visibility.

#### Sample solutions

* Result: Faucet 

![Faucet](sample_sols/faucet.gif)

* Result: Dam Break 

![Dam Break](sample_sols/dam_break.gif)

* Result: Water Drop 

![Water Drop](sample_sols/water_drop.gif)


#### Reference 

* Müller, Matthias, David Charypar, and Markus Gross. "Particle-based fluid simulation for interactive applications." In Proceedings of the 2003 ACM SIGGRAPH/Eurographics symposium on Computer animation, pp. 154-159. Eurographics Association, 2003.

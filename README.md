# Mesh-smoothing-tool

Author: Jan Jose Hurtado Jauregui

![Alt text](screenshot.png?raw=true "Mesh Smoothin Tool Screenshot")

[Mesh Smoothing Tool Demo](https://youtu.be/aUwoBEDocL8)

## Structure
* src: source code
* dependencies: third party libraries
* doc: project documentation
* add: dlls required in a Release version
* addDebug: dlls required in a Debug version
* bin: build - Windows x64 VS2013
  
## How to compile
* Build project using CMake on Mesh-smoothing-tool
* Compile the project using the corresponding compiler

## How to run

* Run the executable
* QT5 and OpenMesh required
* Copy corresponding dlls in the executable path (dlls generated in your installation)
* Add platforms/qwindows.dll from QT_DIR/plugins if it is required 

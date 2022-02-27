# GrizzlyBear-Engine

## Core Features
The focus of the engine lies on stability, performance, and platform independence.

- Direct3D, OpenGL, Vulkan and MoltenVK support
- Scripting language: At first lua, later probably Squirrel 

## Implemented Features 2D

- Basic engine fundament (window, logging, events, etc.)
- OpenGL support
- Windows support
- Linux support (X11)
- Batch renderer
- Particle system (Under construction)
- Asset manager
- Level system / Async streaming
- Level save / Serialization
- Animation support
- Scripting language: Lua support (partial)

## Target Features 2D - In near future
- Post processing
- Phys75 - Physics engine
- Complete editor
- macOS support (on hold until clang supports coroutines)

## Features 3D - Comming soon
There is a simple 3D editor mode, but further 3D support is not realized until the 2D version of the engine is ready.

# Install

Before installing please make sure that your machine meets the following system requirements.
As the development of the engine continues we are expecting the minimum system requirements to lower, due to our optimizations. 

Minimum system requirements:
 - Operating System: Windows 10, Linux (X11)
 - Graphics Card/Driver support: OpenGL 4.3 or higher
 - Processor: NA
 - RAM: NA
 - Disk space (Dev environment): 15 Gigabyte
 - Disk space (Binaries): 20 Megabyte


## Linux (Ubuntu 20.04LTS + Visual Studio Code )

### Required packages:
- make 
- g++-10 or higher
- libx11-dev
- libxcursor-dev
- libxrandr-dev
- libxinerama-dev
- libxi-dev
- libgl1-mesa-dev

### Build setup
Before compiling you need to install the project dependencies and generate the make files:  
```bash
cd scripts/linux/
./install_dependencies.sh
./generate_project-x11.sh gmake <true:false> #true = build the GrizzlyEditor (default:true)
cd ../..
```
Use make to compile the project:
```bash
make config=<release:debug> [-j]
```
Copy  `projects/GrizzlyEditor/editor` and  `projects/GrizzlyEditor/engine` in `bin/<Release:Debug>/linux/GrizzlyEditor`

### Debug example project launch file (gdb)
launch.json:
```JSON
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "(gdb) Launch",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/bin/Debug/linux/GrizzlyEditor/GrizzlyEditor",
            "args": [ 
                "${workspaceFolder}/example/TestProject.gprj"
            ],
            "stopAtEntry": false,
            "cwd": "${fileDirname}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "setupCommands": [
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ]
        }
    ]
}
```
### Known issues:
The ImGui (gui libary) docking feature tends to be a bit buggy under linux 

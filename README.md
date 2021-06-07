# Green Portal Engine

![](https://i.postimg.cc/PqK3mQgT/engine.png)
![](https://i.postimg.cc/xdtknXz6/game.png)


## Documents
[Production Pipeline document](https://docs.google.com/document/d/16_K5LYEYgaHoLp3JxO0UqVd54PJbOMVTss-FqXLqL3I/)  
[Architecture diagrams](https://drive.google.com/drive/folders/1BV3M4kYvesof0PHtpZnjXgYIOzirfpd5)  
[Google Drive](https://drive.google.com/drive/folders/13uqdCMKSp5BYv3FkD2sflDcJ6F_QaaVE)


## Getting started

1. Clone this repository with:
```batch
git clone --recurse-submodules https://github.com/GP-Engine-team/GP_Engine.git
```
If you already cloned the repository, you can use:
```
git submodule update
```

2. Download the assets [from the Drive](https://drive.google.com/drive/folders/1ElLiWl6cP_Z3WOdevMU3CpLjCIND17XD) and extract them from the root of your local copy of this repository
3. Open [projects/GPGame/GPGame.sln](projects/GPGame/GPGame.sln) in Visual Studio
4. **Either set the "GPLauncher" or "GPEditor" project as startup project** to launch the game alone, or with the editor, respectively
5. Choose either Debug or Release mode, and hit F5 to compile

Here is a [link toward a build](https://drive.google.com/file/d/1dIkGqgNC_5oT-BVQ8gtu9o-6xPqjjMRD/view?usp=sharing).


## Features
- [x] Fully-fledged game editor with various edition tools
- [x] Game launcher
- [x] Serialization
- [x] Reflexion
- [x] Resource manager
- [x] Custom resource and resource importation under multiple formats (.fbx, .obj, .wav, .jpeg, ...)
- [x] Custom prefab exportation and importation
- [x] Sound (Stereo, mono, spatialized)
- [x] In-game UI
- [x] Lights
- [x] Shadows
- [x] Scripts
- [x] Hot reload with Visual Studio
- [x] Particle system
- [x] Animation system
- [x] PhysX-based physics
- [x] Custom math library implemented with unit tests (see the [GPM repository](https://github.com/GP-Engine-team/gpm))
- [x] A game made entirely with the engine's editor


## Game controls

| Key         | Action           |
|-------------|------------------|
| S           | Move backward    |
| Z           | Move forward     |
| Q           | Strafe left      | 
| D           | Strafe right     | 
| R           | Reload           |
| Space       | Jump             |
| Shift       | Toggle sprint    |
| Right-click | Aim              |
| Left-click  | Fire             |
| Esc         | Exit the program |


## Known bugs
- Sounds sometimes stutter when there are several sources playing sound simultaneously
- On older/low-end machines, FPS drops may sometimes be experienced


## Repository folders architecture
- `editor/`: code relative to the editor
- `engine/`: the heart of the engine's C++ API
- `generator/`: reflexion with Refureku
- `launcher/`: game-only launcher code
- `projects/`: where the games made with the engine are edited

## Global architecture

The following diagram, as well as other more specific UML diagrams, are [available on the project's Google Drive](https://drive.google.com/drive/folders/1BV3M4kYvesof0PHtpZnjXgYIOzirfpd5).

Click the image to get a zoomable version of it.
[![Global architecture diagram](https://i.postimg.cc/yYQ1561h/Global-project-architecture.png)](https://postimg.cc/bsnfSh2d)

## Dependencies

- [GLFW 3.3.2](https://github.com/glfw/glfw)
- [Dear ImGui](https://github.com/ocornut/imgui)
- [OpenAL Soft](https://github.com/kcat/openal-soft)
- [Nvidia PhysX 4.1](https://github.com/NVIDIAGameWorks/PhysX)
- [Assimp](https://github.com/assimp/assimp)
- [OpenGL Core 4.6](https://www.khronos.org/registry/OpenGL/specs/gl/glspec46.core.pdf) & [GLSL 4.60.5](https://www.khronos.org/registry/OpenGL/specs/gl/GLSLangSpec.4.60.pdf)
- [glad](https://github.com/Dav1dde/glad) with [GL_KHR_Debug](https://www.khronos.org/registry/OpenGL/extensions/KHR/KHR_debug.txt)
- [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h)
- [stb_image_write](https://github.com/nothings/stb/blob/master/stb_image_write.h)
- [Refureku](https://github.com/jsoysouvanh/Refureku)
- [RapidXML](http://rapidxml.sourceforge.net/manual.html)
- [ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo)
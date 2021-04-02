# Green Portal Engine

## Documents
[Production Pipeline document](https://docs.google.com/document/d/16_K5LYEYgaHoLp3JxO0UqVd54PJbOMVTss-FqXLqL3I/)  
[Architecture diagrams](https://drive.google.com/drive/folders/1BV3M4kYvesof0PHtpZnjXgYIOzirfpd5)  
[Google Drive](https://drive.google.com/drive/folders/13uqdCMKSp5BYv3FkD2sflDcJ6F_QaaVE)  


## Getting started

1. Clone this repository with:
```batch
git clone --recurse-submodules https://github.com/GP-Engine-team/GP_Engine.git
```
2. Download the assets [from the Drive](https://drive.google.com/drive/folders/1ElLiWl6cP_Z3WOdevMU3CpLjCIND17XD) and extract them from the root of your local copy of this repository (individually, or 
3. Open [projects/GPGame/GPGame.sln](projects/GPGame/GPGame.sln) in Visual Studio
4. **Either set the "GPLauncher" or "GPEditor" project as startup project** to launch the game alone, or with the editor, respectively
5. Choose either Debug or Release mode, and hit F5 to compile


## Controls, features

| Key   | Action           |
|-------|------------------|
| S     | Move backward    |
| Z     | Move forward     |
| Q     | Strafe left      | 
| D     | Strafe right     | 
| Space | Jump             |
| Shift | Toggle sprint    |
| Esc   | Exit the program |


## Known bugs
- Sounds are sometimes played with lags, in a fragmented way
- Inputs declared in game-code collide with inputs declared in editor-code
- When the editor's window is minimized, OpenGL errors are raised and visible in the console
- The `ResourceManager` can't take a single template argument

## Repository folders architecture

- `editor/`: code relative to the editor
- `engine/`: the heart of the C++ API
- `generator/`: reflexion with Refureku
- `launcher/`: game-only launcher code
- `projects/`: where the games made with the engine are edited

## Global architecture

The following diagram, as well as other more specific UML diagrams, are [available on the project's Google Drive](https://drive.google.com/drive/folders/1BV3M4kYvesof0PHtpZnjXgYIOzirfpd5).

Click the image to get a zoomable version of it.
[![Global architecture diagram](https://i.postimg.cc/2ynPdCtG/Global-project-architecture.png)](https://postimg.cc/1V5Cs1sV)

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
- [stb_perlin](https://github.com/nothings/stb/blob/master/stb_perlin.h)
- [Refureku](https://github.com/jsoysouvanh/Refureku)
- [RapidXML](http://rapidxml.sourceforge.net/manual.html)

# GP_Engine

[Production Pipeline document](https://docs.google.com/document/d/16_K5LYEYgaHoLp3JxO0UqVd54PJbOMVTss-FqXLqL3I/edit)
[Google Drive of the engine](https://drive.google.com/drive/folders/13uqdCMKSp5BYv3FkD2sflDcJ6F_QaaVE)

## How to compile

Run this command to download all dependencies (this operation can take up to a few minutes)

```batch
git submodule update --init --recursive
```

 - 1 : Download cmake for windows : https://cmake.org/download/
 - 2 : Run installer and check : add Cmake to system path 
![](https://tulip.labri.fr/TulipDrupal/sites/default/files/uploadedFiles/users/6/screenshot_cmake01.png)
- 3 : Run [installer](install_dependancies.bat) in the project root to download dependancy (git submodule) and install it

## How to run with Editor

- Open GPGame.sln
- Set GPEditor as default project
- Compile and run

If you want to use hot reload, you have 2 choices:
- You can run without debug mode (Ctrl + F5) and then rebuild the dll (Ctrl + maj + B) each time you change it.
- You can run in debug mode (F5) and then rebuild the dll (Ctrl + maj + B) with another Visual Studio instance each time you change it.

## How to run in Release Mode

- Open GPGame.sln
- Set GPLauncher as default project
- Compile and run

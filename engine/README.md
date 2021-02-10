# GP_Engine

Link to Production Pipeline document : https://docs.google.com/document/d/16_K5LYEYgaHoLp3JxO0UqVd54PJbOMVTss-FqXLqL3I/edit
Link to Google Drive of our engine : https://drive.google.com/drive/folders/13uqdCMKSp5BYv3FkD2sflDcJ6F_QaaVE

## How to compile

Run this command to doawload all depedancy (this operation can take few minutes)

```batch
git submodule update --init --recursive
```

 - 1 : Download cmake for windows : https://cmake.org/download/
 - 2 : Run installer and check : add Cmake to system path 
![](https://tulip.labri.fr/TulipDrupal/sites/default/files/uploadedFiles/users/6/screenshot_cmake01.png)
- 3 : Run [installer](install_dependancies.bat) in the project root to download dependancy (git submodule) and install it
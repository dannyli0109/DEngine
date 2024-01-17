@echo off
echo Building the project...

:: Generate build files
cmake -S . -B build

:: Change this line according to your build system
:: For Visual Studio's MSBuild
cmake --build build
:: For NMake
:: cmake --build build -- -f "NMake Makefiles"

echo Running the application...

build\Debug\Opengl

echo Done.
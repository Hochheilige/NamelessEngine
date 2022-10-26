TITLE External Libraries Build

cd External

ECHO Building Assimp library

XCOPY "CMakeLists.assimp" "assimp\" /y

cd assimp

rm CMakeLists.txt

ren *.assimp *.txt

rm CMakeCache.txt
cmake CMakeLists.txt
cmake --build .

ECHO Updating CMakeLists for Bullet Physics
cd ..\
XCOPY "CMakeLists.txt" "bullet3\" /y

cd bullet3

ECHO Building Bullet Physics static libraries

rm CMakeCache.txt
cmake CMakeLists.txt
cmake --build .
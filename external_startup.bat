TITLE External Libraries Build

cd External\assimp

ECHO Building Assimp static library

rm CMakeCache.txt
cmake CMakeLists.txt
cmake --build .

ECHO Updating CMakeLists for Bullet Physics
cd ..\
XCOPY "CMakeLists.txt" "bullet3\" /y

cd bullet3

rm CMakeCache.txt
cmake CMakeLists.txt
cmake --build .
@echo off 

SET BUILD_DIR=build
SET BUILD_TYPE=Debug
SET SHARED_LIB=OFF

if exist %BUILD_DIR% ( rmdir /s/q %BUILD_DIR% )
mkdir %BUILD_DIR%
cd %BUILD_DIR%

cmake .. -G "Visual Studio 15 2017"           ^
	-DOPERATING_SYSTEM:STRING=WINDOWS         ^
	-DARCH_BITS:STRING=32                     ^
	-DBUILD_SHARED_LIBS:BOOL=%SHARED_LIB%     ^
	-DCMAKE_BUILD_TYPE:STRING=%BUILD_TYPE%    ^
	-DBUILD_TYPE:STRING=%BUILD_TYPE%          ^
	-DCMAKE_ENABLE_EXPORTS:BOOL=ON

cmake --build . --config %BUILD_TYPE%

cd ..\
if exist %BUILD_DIR% ( rmdir /s/q %BUILD_DIR% )
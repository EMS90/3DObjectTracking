@ECHO OFF
set SCRIPT_DIR=%~dp0
set SCRIPT_DIR=%SCRIPT_DIR:\=/%

REM Set paths for dependencies

echo VCPKG_ROOT=%VCPKG_ROOT%
set Eigen3_DIR=%VCPKG_ROOT:\=/%/installed/x64-windows/share/eigen3
set GLEW_DIR=%VCPKG_ROOT:\=/%/installed/x64-windows/share/glew
set glfw3_DIR=%VCPKG_ROOT:\=/%/installed/x64-windows/share/glfw3
REM Set OpenCV paths
set OpenCV_PLUGIN=%SCRIPT_DIR%../../../../../OpenCV/Source/ThirdParty/OpenCV
set OpenCV_INCLUDE_DIR=%OpenCV_PLUGIN%/include
set OpenCV_LIB_DIR=%OpenCV_PLUGIN%/lib/Win64
set OpenCV_DLL_DIR=%OpenCV_PLUGIN%/../../../Binaries/ThirdParty/Win64
set OpenCV_LIB_NAME_RELEASE=opencv_world455
set OpenCV_LIB_NAME_DEBUG=opencv_world455d

:: Create build directory
if not exist build md build

pushd build

echo Deleting existing build directories...
if exist x64 rd /s /q x64

:: Create x64 directory
IF NOT EXIST x64 (
	md x64
)
pushd x64

echo Configuring x64 build...
echo %SCRIPT_DIR%
cmake^
 -G "Visual Studio 17 2022"^
 -A x64^
 -S "%SCRIPT_DIR%"^
 -DCMAKE_INSTALL_PREFIX="%SCRIPT_DIR%install"^
 -DCMAKE_PREFIX_PATH="%Eigen3_DIR%","%GLEW_DIR%","%glfw3_DIR%"^
 -DOpenCV_INCLUDE_DIR="%OpenCV_INCLUDE_DIR%"^
 -DOpenCV_LIB_DIR="%OpenCV_LIB_DIR%"^
 -DOpenCV_DLL_DIR="%OpenCV_DLL_DIR%"^
 -DOpenCV_LIB_NAME_RELEASE="%OpenCV_LIB_NAME_RELEASE%"^
 -DOpenCV_LIB_NAME_DEBUG="%OpenCV_LIB_NAME_DEBUG%"^
 -DOpenCV_STATIC=OFF^
 -DUSE_AZURE_KINECT=OFF^
 -DUSE_REALSENSE=OFF^
 -DUSE_GTEST=OFF^
 -DCMAKE_CXX_FLAGS_RELEASE="/MD /Zi"^
 -DCMAKE_CXX_FLAGS_DEBUG="/MDd"

echo Building x64 Release build...
cmake --build . --config Release --target INSTALL -- /m:4

:: echo Building x64 Debug build... Currently not possible due to erros.
:: cmake --build . --config Debug --target m3t -- /m:4 

:: x64/..
popd

echo Moving outputs to destination folders...

set lib_path="%~dp0lib"

echo lib_path is %lib_path%

echo %lib_path%\Win64

IF NOT EXIST %lib_path%Win64 (
	md %lib_path%\Win64
)

move /y x64\src\Release\m3t*.* %lib_path%\Win64

echo Cleaning up...

:: build/..
popd

rd /s /q build

echo Build completed!
pause
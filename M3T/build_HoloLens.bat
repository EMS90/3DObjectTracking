@ECHO OFF
set SCRIPT_DIR=%~dp0
set SCRIPT_DIR=%SCRIPT_DIR:\=/%

REM Set paths for dependencies

echo VCPKG_ROOT=%VCPKG_ROOT%
set Eigen3_DIR=%VCPKG_ROOT:\=/%/installed/arm64-uwp/share/eigen3
REM Set OpenCV paths
set OpenCV_PLUGIN=%SCRIPT_DIR%../../../../../OpenCV/Source/ThirdParty/OpenCV
set OpenCV_INCLUDE_DIR=%OpenCV_PLUGIN%/include
set OpenCV_LIB_DIR=%OpenCV_PLUGIN%/lib/HoloLens
set OpenCV_LIB_NAME_RELEASE=opencv_world455
set OpenCV_LIB_NAME_DEBUG=opencv_world455d

:: Create build directory
if not exist build md build

pushd build

echo Deleting existing build directories...
if exist HoloLens rd /s /q HoloLens

:: Create HoloLens directory
IF NOT EXIST HoloLens (
    md HoloLens
)
pushd HoloLens

echo Configuring HoloLens build...
cmake^
 -G "Visual Studio 17 2022"^
 -A ARM64^
 -S "%SCRIPT_DIR%"^
 -DCMAKE_INSTALL_PREFIX="%SCRIPT_DIR%install"^
 -DCMAKE_PREFIX_PATH="%Eigen3_DIR%"^
 -DOpenCV_INCLUDE_DIR="%OpenCV_INCLUDE_DIR%"^
 -DOpenCV_LIB_DIR="%OpenCV_LIB_DIR%"^
 -DOpenCV_LIB_NAME_RELEASE="%OpenCV_LIB_NAME_RELEASE%"^
 -DOpenCV_LIB_NAME_DEBUG="%OpenCV_LIB_NAME_DEBUG%"^
 -DOpenCV_STATIC=OFF^
 -DUSE_AZURE_KINECT=OFF^
 -DUSE_REALSENSE=OFF^
 -DUSE_GTEST=OFF^
 -DCMAKE_CXX_FLAGS_RELEASE="/MD"^
 -DCMAKE_CXX_FLAGS_DEBUG="/MDd"^
 -DBUILD_UE_ONLY="ON"

echo Building HoloLens Release build...
cmake --build . --config Release --target m3t_UE -- /m:4

echo Building HoloLens Debug build...
:: cmake --build . --config Debug --target INSTALL -- /m:4

:: HoloLens/..
popd

echo Moving outputs to destination folders...

set lib_path="%~dp0lib"

echo lib_path is %lib_path%

echo %lib_path%\HoloLens

IF NOT EXIST %lib_path%\HoloLens (
    md %lib_path%\HoloLens
)

move /y HoloLens\src\Release\m3t*.lib %lib_path%\HoloLens

echo Cleaning up...

:: build/..
popd

rd /s /q build

echo Build completed!
pause
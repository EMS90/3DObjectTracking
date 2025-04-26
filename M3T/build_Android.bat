@ECHO OFF
set SCRIPT_DIR=%~dp0
set SCRIPT_DIR=%SCRIPT_DIR:\=/%

REM Set paths for dependencies

echo VCPKG_ROOT=%VCPKG_ROOT%
set Eigen3_DIR=%VCPKG_ROOT:\=/%/installed/arm64-android/share/eigen3
REM Set OpenCV paths
set OpenCV_PLUGIN=%SCRIPT_DIR%../../../../../OpenCV/Source/ThirdParty/OpenCV
set OpenCV_INCLUDE_DIR=%OpenCV_PLUGIN%/include
set OpenCV_LIB_DIR=%OpenCV_PLUGIN%/lib/Android
set OpenCV_LIB_NAME_RELEASE=opencv_world455
set OpenCV_LIB_NAME_DEBUG=opencv_world455d

:: Create build directory
if not exist build md build

pushd build

echo Deleting existing build directories...
if exist Android rd /s /q Android

:: Create Android directory
IF NOT EXIST Android (
	md Android
)
pushd Android
echo eigen3 "%Eigen3_DIR%"
echo Configuring Android build...
echo %SCRIPT_DIR%
cmake^
 -G "Visual Studio 17 2022"^
 -A ARM64^
 -S "%SCRIPT_DIR%"^
 -DCMAKE_INSTALL_PREFIX="%SCRIPT_DIR%install"^
 -DCMAKE_PREFIX_PATH="%Eigen3_DIR%"^
 -DEigen3_DIR="%Eigen3_DIR%"^
 -DOpenCV_INCLUDE_DIR="%OpenCV_INCLUDE_DIR%"^
 -DOpenCV_LIB_DIR="%OpenCV_LIB_DIR%"^
 -DOpenCV_LIB_NAME_RELEASE="%OpenCV_LIB_NAME_RELEASE%"^
 -DOpenCV_LIB_NAME_DEBUG="%OpenCV_LIB_NAME_DEBUG%"^
 -DOpenCV_STATIC=OFF^
 -DUSE_AZURE_KINECT=OFF^
 -DUSE_REALSENSE=OFF^
 -DUSE_GTEST=OFF^
 -DCMAKE_CXX_FLAGS_RELEASE="/MD -frtti -fexceptions -std=c++17"^
 -DCMAKE_CXX_FLAGS_DEBUG="/MDd -frtti -fexceptions -std=c++17"^
 -DCMAKE_SYSTEM_NAME=Android^
 -DANDROID_ABI=arm64-v8a^
 -DANDROID_SDK_TOOLS="%ANDROID_SDK_HOME%\cmdline-tools\8.0"^
 -DBUILD_ANDROID_PROJECTS=OFF^
 -DCMAKE_POLICY_VERSION_MINIMUM=3.5^
 -DBUILD_UE_ONLY="ON"

echo Building Android Release build...
cmake --build . --config Release --target m3t_UE -- /m:4 /p:TrackFileAccess=false

echo Building Android Debug build...
:: cmake --build . --config Debug --target m3t_UE -- /m:4 /p:TrackFileAccess=false

:: x64/..
popd

echo Moving outputs to destination folders...

set lib_path="%~dp0lib"

echo lib_path is %lib_path%

echo %lib_path%\Android

IF NOT EXIST %lib_path%\Android (
	md %lib_path%\Android
)

move /y Android\src\Release\libm3t*.a %lib_path%\Android

echo Cleaning up...

:: build/..
popd

rd /s /q build

echo Build completed!
pause
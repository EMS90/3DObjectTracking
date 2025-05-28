cmake_minimum_required(VERSION 3.10)

# Define headers and sources
# =============================================================================
set(SOURCES
        common.cpp
        body.cpp
        renderer_geometry.cpp
        renderer.cpp
        normal_renderer.cpp
        silhouette_renderer.cpp
        basic_depth_renderer.cpp
        model.cpp
        region_model.cpp
        depth_model.cpp
        camera.cpp
        loader_camera.cpp
        viewer.cpp
        image_viewer.cpp
        normal_viewer.cpp 
        color_histograms.cpp
        modality.cpp
        region_modality.cpp
        depth_modality.cpp
        texture_modality.cpp
        link.cpp
        constraint.cpp
        soft_constraint.cpp
        optimizer.cpp
        detector.cpp 
        static_detector.cpp
        manual_detector.cpp
        refiner.cpp
        publisher.cpp
        subscriber.cpp
        tracker.cpp)

set(HEADERS 
        ../include/m3t/common.h
        ../include/m3t/body.h
        ../include/m3t/renderer_geometry.h
        ../include/m3t/renderer.h
        ../include/m3t/normal_renderer.h
        ../include/m3t/silhouette_renderer.h
        ../include/m3t/basic_depth_renderer.h
        ../include/m3t/model.h
        ../include/m3t/region_model.h
        ../include/m3t/depth_model.h
        ../include/m3t/camera.h
        ../include/m3t/loader_camera.h
        ../include/m3t/viewer.h
        ../include/m3t/image_viewer.h
        ../include/m3t/normal_viewer.h
        ../include/m3t/color_histograms.h
        ../include/m3t/modality.h
        ../include/m3t/region_modality.h
        ../include/m3t/depth_modality.h
        ../include/m3t/texture_modality.h
        ../include/m3t/link.h
        ../include/m3t/constraint.h
        ../include/m3t/soft_constraint.h
        ../include/m3t/optimizer.h
        ../include/m3t/detector.h
        ../include/m3t/static_detector.h
        ../include/m3t/manual_detector.h
        ../include/m3t/refiner.h
        ../include/m3t/publisher.h
        ../include/m3t/subscriber.h
        ../include/m3t/tracker.h
        ../include/m3t/generator.h)

if (USE_AZURE_KINECT)
    list(APPEND SOURCES azure_kinect_camera.cpp)
    list(APPEND HEADERS ../include/m3t/azure_kinect_camera.h)
endif ()
if (USE_REALSENSE)
    list(APPEND SOURCES realsense_camera.cpp)
    list(APPEND HEADERS ../include/m3t/realsense_camera.h)
endif ()


# Define libraries
# =============================================================================
set(LIBRARIES Eigen3::Eigen OpenGL::GL GLEW::GLEW glfw ${OpenCV_LIBRARIES})
if (UNIX)
    list(APPEND LIBRARIES pthread stdc++fs)
endif ()
if (USE_AZURE_KINECT)
    list(APPEND LIBRARIES k4a::k4a)
endif ()
if (USE_REALSENSE)
    list(APPEND LIBRARIES ${realsense2_LIBRARY})
endif ()


# Define target
# =============================================================================
    add_library(m3t STATIC ${SOURCES} ${HEADERS})
    target_link_libraries(m3t PUBLIC ${LIBRARIES})
    target_compile_definitions(m3t PUBLIC USE_OPENGL=1)
    target_include_directories(m3t PUBLIC  
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
            $<INSTALL_INTERFACE:include>)
    target_include_directories(m3t PUBLIC 
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../third_party>
            $<INSTALL_INTERFACE:include>)
    set_target_properties(m3t PROPERTIES PUBLIC_HEADER "${HEADERS}")
endif ()


# Define executable for model generation
# =============================================================================
add_executable(generate_model generate_model.cpp)
# Link the executable with the m3t library and required dependencies
target_link_libraries(generate_model PRIVATE m3t ${LIBRARIES})
target_compile_definitions(generate_model PRIVATE USE_OPENGL=1)
# Add include directories for m3t and third-party libraries
target_include_directories(generate_model PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../third_party>
    $<INSTALL_INTERFACE:include>)
# Set the output directory for the generate_model executable
set_target_properties(generate_model PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")

    # List of required DLLs
    set(OpenCV_DLLS
        "${OpenCV_DLL_DIR}/${OpenCV_LIB_NAME_RELEASE}.dll"
    )

    # Copy DLLs to the runtime output directory
    add_custom_command(TARGET generate_model POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${OpenCV_DLLS}
        $<TARGET_FILE_DIR:generate_model>
    )
endif()


# Install
# =============================================================================
install(TARGETS m3t
        EXPORT m3t_target
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include/m3t)
install(DIRECTORY ../third_party/
        DESTINATION include)
install(EXPORT m3t_target 
        FILE M3TConfig.cmake
        DESTINATION lib/cmake/M3T)

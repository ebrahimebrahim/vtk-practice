cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(project_point)

set(VTK_DIR "/home/ebrahim/vtk/build/")

find_package(
    VTK COMPONENTS
    vtkCommonColor
    vtkCommonCore
    vtkCommonDataModel
    QUIET
)

if (NOT VTK_FOUND)
    message("Failure, aborting: ${VTK_NOT_FOUND_MESSAGE}")
    return ()
endif()

message(STATUS "VTK_VERSION: ${VTK_VERSION}")

if (VTK_VERSION VERSION_LESS "8.90.0")
    include(${VTK_USE_FILE})
    add_executable(project_point main.cpp)
    target_link_libraries(project_point PRIVATE ${VTK_LIBRARIES})
else ()
    add_executable(project_point main.cpp)
    target_link_libraries(project_point PRIVATE ${VTK_LIBRARIES})
    vtk_module_autoinit(
        TARGETS project_point
        MODULES ${VTK_LIBRARIES}
    )
endif ()

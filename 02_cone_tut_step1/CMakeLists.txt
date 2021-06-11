cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(tut_step1)

set(VTK_DIR "/home/ebrahim/vtk/build/")

find_package(
    VTK COMPONENTS
    vtkCommonColor
    vtkCommonCore
    vtkFiltersSources
    vtkInteractionStyle
    vtkRenderingContextOpenGL2
    vtkRenderingCore
    vtkRenderingFreeType
    vtkRenderingGL2PSOpenGL2
    vtkRenderingOpenGL2
    QUIET
)

if (NOT VTK_FOUND)
    message("Failure, aborting: ${VTK_NOT_FOUND_MESSAGE}")
    return ()
endif()

message(STATUS "VTK_VERSION: ${VTK_VERSION}")

if (VTK_VERSION VERSION_LESS "8.90.0")
    include(${VTK_USE_FILE})
    add_executable(tut_step1 tut.cpp)
    target_link_libraries(tut_step1 PRIVATE ${VTK_LIBRARIES})
else ()
    add_executable(tut_step1 tut.cpp)
    target_link_libraries(tut_step1 PRIVATE ${VTK_LIBRARIES})
    vtk_module_autoinit(
        TARGETS tut_step1
        MODULES ${VTK_LIBRARIES}
    )
endif ()
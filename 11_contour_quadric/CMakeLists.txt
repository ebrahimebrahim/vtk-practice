cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(contour)

set(VTK_DIR "/home/ebrahim/vtk/build/")

find_package(
    VTK COMPONENTS
    vtkCommonColor
    vtkCommonCore
    vtkFiltersModeling
    vtkImagingHybrid
    vtkInteractionStyle
    vtkRenderingContextOpenGL2
    vtkRenderingCore
    vtkRenderingFreeType
    vtkRenderingGL2PSOpenGL2
    vtkRenderingOpenGL2
    vtkIOGeometry
    vtkInteractionWidgets
    QUIET
)

if (NOT VTK_FOUND)
    message("Failure, aborting: ${VTK_NOT_FOUND_MESSAGE}")
    return ()
endif()

message(STATUS "VTK_VERSION: ${VTK_VERSION}")

if (VTK_VERSION VERSION_LESS "8.90.0")
    include(${VTK_USE_FILE})
    add_executable(contour main.cpp)
    target_link_libraries(contour PRIVATE ${VTK_LIBRARIES})
else ()
    add_executable(contour main.cpp)
    target_link_libraries(contour PRIVATE ${VTK_LIBRARIES})
    vtk_module_autoinit(
        TARGETS contour
        MODULES ${VTK_LIBRARIES}
    )
endif ()

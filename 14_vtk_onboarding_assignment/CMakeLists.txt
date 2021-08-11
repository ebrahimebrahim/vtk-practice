
cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(SmoothPolyDataFilter)

find_package(VTK COMPONENTS 
  vtkCommonColor
  vtkCommonCore
  vtkCommonDataModel
  vtkFiltersCore
  vtkInteractionStyle
  vtkRenderingContextOpenGL2
  vtkRenderingCore
  vtkRenderingFreeType
  vtkRenderingGL2PSOpenGL2
  vtkRenderingOpenGL2
  QUIET
)

message (STATUS "VTK_VERSION: ${VTK_VERSION}")

add_executable(SmoothPolyDataFilter SmoothPolyDataFilter.cxx)
target_link_libraries(SmoothPolyDataFilter PRIVATE ${VTK_LIBRARIES})

vtk_module_autoinit(
  TARGETS SmoothPolyDataFilter
  MODULES ${VTK_LIBRARIES}
)

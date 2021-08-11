
cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(vtk_onboarding_assignment)

find_package(VTK COMPONENTS 
  vtkCommonColor
  vtkCommonCore
  vtkCommonDataModel
  vtkFiltersCore
  vtkFiltersModeling
  vtkFiltersSources
  vtkInteractionStyle
  vtkRenderingContextOpenGL2
  vtkRenderingCore
  vtkRenderingFreeType
  vtkRenderingGL2PSOpenGL2
  vtkRenderingOpenGL2
  vtkIOGeometry
  QUIET
)

message (STATUS "VTK_VERSION: ${VTK_VERSION}")

add_executable(SmoothPolyDataFilter SmoothPolyDataFilter.cxx)
target_link_libraries(SmoothPolyDataFilter PRIVATE ${VTK_LIBRARIES})

add_executable(ComputeMass ComputeMass.cxx)
target_link_libraries(ComputeMass PRIVATE ${VTK_LIBRARIES})

vtk_module_autoinit(
  TARGETS SmoothPolyDataFilter ComputeMass
  MODULES ${VTK_LIBRARIES}
)

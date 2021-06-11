cmake_minimum_required(VERSION 3.16) # The lowest version I have tested on

project(vtk-practice VERSION 0.0
                     DESCRIPTION "practice with VTK"
                     LANGUAGES C CXX)

set(VTK_DIR "/home/ebrahim/vtk/build/")
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})


add_executable(test test.cpp)
target_compile_features(test PUBLIC cxx_std_17)
target_link_libraries(test ${VTK_LIBRARIES})

cmake_minimum_required(VERSION 3.15)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(TinyRenderer3D-Vulkan
        LANGUAGES CXX)

find_package(Vulkan REQUIRED)
find_package(glm REQUIRED)
find_package(SDL2 REQUIRED)

find_program(GLSLC_PROGRAM glslc REQUIRED)

aux_source_directory(src SRC)

add_library(tr3d SHARED ${SRC})
target_include_directories(
    tr3d
    PRIVATE tr3d
    PUBLIC .
)
target_compile_features(tr3d PUBLIC cxx_std_17)
target_link_libraries(tr3d PUBLIC Vulkan::Vulkan glm::glm SDL2::SDL2)

#######################
# test
#######################
enable_testing()
add_subdirectory(test)

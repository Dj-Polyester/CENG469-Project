cmake_minimum_required(VERSION 3.16)
project(testProj VERSION 0.0.1)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_GENERATOR Ninja)
add_compile_definitions(LITTLEBUG)
#add_compile_definitions(VERBOSE_ENABLED)
#add_compile_definitions(INFO_ENABLED)

include_directories(include)
file(GLOB_RECURSE SRC_FILES src/*)
file(GLOB_RECURSE HEADERS include/*)
add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADERS})
# GLFW
find_library(glfw NAMES glfw3 libglfw3)
# Vulkan
find_package(Vulkan REQUIRED)
target_link_libraries(${PROJECT_NAME} glfw ${Vulkan_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${Vulkan_INCLUDE_DIR})



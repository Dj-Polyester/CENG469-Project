cmake_minimum_required(VERSION 3.22)
project(testProj VERSION 0.0.1)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_GENERATOR Ninja)
add_compile_definitions(VALIDATION_ENABLED)
add_compile_definitions(VERBOSE_ENABLED)
#add_compile_definitions(INFO_ENABLED)

include_directories(include)
set (CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SRC_FILES src/*)
file(GLOB_RECURSE HEADERS include/*)
add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADERS})
# GLFW
if (WIN32)
    #do something
    include_directories("C:/GraphicsProjects/libraries/glfw-3.3.6/include")
    list(APPEND CMAKE_PREFIX_PATH "C:/GraphicsProjects/libraries")
    find_library(GLFW 
			NAMES glfw3.lib
			HINTS "C:/GraphicsProjects/libraries/glfw-3.3.6/lib-vc2022")
    target_include_directories(${PROJECT_NAME} PRIVATE "C:/GraphicsProjects/libraries/glfw-3.3.6/include")
    target_include_directories(${PROJECT_NAME} PRIVATE "C:/GraphicsProjects/libraries")
    target_link_libraries(${PROJECT_NAME} ${GLFW})
    find_package(Vulkan REQUIRED)
    target_link_libraries(${PROJECT_NAME} ${Vulkan_LIBRARIES})
    target_include_directories(${PROJECT_NAME} PUBLIC ${Vulkan_INCLUDE_DIR})


else()
    find_library(glfw NAMES glfw3 libglfw3 REQUIRED)
    find_package(Vulkan REQUIRED)
    target_link_libraries(${PROJECT_NAME} glfw ${Vulkan_LIBRARIES})
    target_include_directories(${PROJECT_NAME} PRIVATE ${Vulkan_INCLUDE_DIR})
endif (WIN32)
# Vulkan




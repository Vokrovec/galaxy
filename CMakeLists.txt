cmake_minimum_required(VERSION 3.14)
project(galaxy)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)

# 1. Automatically grab all .cpp files in src/ (and its subfolders)
file(GLOB_RECURSE MY_SOURCES 
    CONFIGURE_DEPENDS 
    "src/*.cpp"
    "include/*.hpp"
)

# --- 1. Fetch GLFW automatically ---
include(FetchContent)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8 # Or the latest stable version
)
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "")
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(glfw)

set(PROJECT_SOURCES
    ${MY_SOURCES}
    thirdparty/glad/src/glad.c
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glad/include
)


target_link_libraries(${PROJECT_NAME} glfw)
if(NOT APPLE)
    target_link_libraries(${PROJECT_NAME} OpenGL::GL)
else()
    find_library(OPENGL_FRAMEWORK OpenGL)
    target_link_libraries(${PROJECT_NAME} ${OPENGL_FRAMEWORK})
endif()

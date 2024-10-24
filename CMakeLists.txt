cmake_minimum_required(VERSION 3.10)

# Requirements of c++ standart
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# debug
if(1)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif()

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(EXT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ext")

project(dengine)

# TODO: platform deps
if (WIN32)
    set(PLATFORM "Windows")
elseif(UNIX)
    set(PLATFORM "Linux")
endif()

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Freetype REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Build glad
set(GLAD_DIR "${EXT_DIR}/glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")
#target_include_directories(${PROJECT_NAME} PRIVATE "${GLAD_DIR}/include")

set(src_core
    src/main.cpp
    src/core/Engine.cpp
    src/core/Console.cpp
    src/core/Input.cpp
    src/core/Logger.cpp
    src/core/Session.cpp
    src/core/WindowManager.cpp
    src/core/Camera.cpp
    src/core/Scene.cpp

    src/renderer/RenderEngine.cpp
    src/renderer/Texture.cpp
    src/renderer/Shader.cpp
    src/renderer/Mesh.cpp
    src/renderer/Model.cpp
    src/renderer/Light.cpp
    src/renderer/Material.cpp
    #src/renderer/Quad.cpp
)

include_directories("src/core")
include_directories("src/renderer")
include_directories("${GLAD_DIR}/include")
include_directories(${FREETYPE_INCLUDE_DIRS})

add_executable(${CMAKE_PROJECT_NAME} ${src_core})

# Link libraries
target_link_libraries(${PROJECT_NAME}
    dl
    Freetype::Freetype
    SDL2::SDL2
    "glad"
    OpenGL::GL
)

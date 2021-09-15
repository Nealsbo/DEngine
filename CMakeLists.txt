cmake_minimum_required(VERSION 3.10)

project(DEngine)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(src_core
	src/main.cpp
	src/core/Engine.cpp
	src/core/Logger.cpp
	src/core/WindowManager.cpp
)

include_directories("src/core")

add_executable(${CMAKE_PROJECT_NAME} ${src_core})
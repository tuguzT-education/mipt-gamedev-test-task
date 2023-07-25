cmake_minimum_required(VERSION 3.0)
project(Test_Task)

set(CMAKE_CXX_STANDARD 17)
set(SRC src/Engine.h src/Engine.cpp src/Game.cpp)

add_executable(Test_Task WIN32 ${SRC})

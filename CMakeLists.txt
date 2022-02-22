cmake_minimum_required(VERSION 3.0.0)
project(OpenGL VERSION 0.1.0)

add_subdirectory(MainProject)
add_subdirectory(Lib)

file (COPY ${OpenGL_SOURCE_DIR}/resources DESTINATION ${OpenGL_SOURCE_DIR}/build/)
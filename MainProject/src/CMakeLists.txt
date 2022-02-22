include_directories(${OpenGL_SOURCE_DIR}/MainProject/inc)
include_directories(${OpenGL_SOURCE_DIR}/Lib/inc)
link_directories(${OpenGL_SOURCE_DIR}/Lib/src)
file(GLOB all_SRC
     "*.c"
     "*.cpp"
)
add_executable(MainProject main.cpp ${all_SRC})
target_link_libraries(MainProject Lib GL GLU glfw3 m Xrandr Xi X11 Xxf86vm pthread dl Xinerama Xcursor)
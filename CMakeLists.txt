cmake_minimum_required(VERSION 3.26)
project(z3)

set(CMAKE_CXX_STANDARD 17)

add_executable(rgridtool main.cpp)
add_library(rgrid lib/Resistor.cpp
        lib/Resistor.h
        lib/GridPoint.cpp
        lib/GridPoint.h
        lib/Coordinate.h
        include/shortcuts.h
        lib/Equation.cpp
        lib/Equation.h
        lib/ResistorGrid.cpp
        lib/ResistorGrid.h)

target_link_libraries(rgridtool rgrid)

add_subdirectory(thirdparty/clipp)
target_link_libraries(rgridtool clipp)

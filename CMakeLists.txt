cmake_minimum_required(VERSION 3.10)
project(PlanetSimulation)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_executable(planet
    src/main.cpp
    src/Planet.cpp
)

target_include_directories(planet PRIVATE src)

find_library(GLAD_LIB NAMES glad)
if(GLAD_LIB)
    target_link_libraries(planet PRIVATE ${GLAD_LIB})
else()
    message(WARNING "glad not found; make sure to install glad")
endif()

target_link_libraries(planet PRIVATE glfw glm)

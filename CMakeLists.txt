cmake_minimum_required(VERSION 3.10.0)
project(elec1601 VERSION 0.1.0 LANGUAGES C)

find_package(SDL2 CONFIG REQUIRED)
set(SOURCES_FILE src)

add_executable(elec1601 
${SOURCES_FILE}/formulas.c
${SOURCES_FILE}/main.c 
${SOURCES_FILE}/robot.c 
${SOURCES_FILE}/wall.c
${SOURCES_FILE}/path_track.c)
target_include_directories(elec1601 PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    SDL2::SDL2
)

add_definitions(-DSDL_MAIN_HANDLED)

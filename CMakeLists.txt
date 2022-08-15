cmake_minimum_required(VERSION 3.18)
project(KustBot CXX)

file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

set(CMAKE_CXX_STANDARD 17)
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -ggdb)

find_package(Threads REQUIRED)
# find_package(SQLite3)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/lib/yaml-cpp/")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/lib/sqlitecpp/")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/lib/range-v3")

target_link_libraries(${PROJECT_NAME} PRIVATE yaml-cpp Threads::Threads SQLiteCpp sqlite3 range-v3)


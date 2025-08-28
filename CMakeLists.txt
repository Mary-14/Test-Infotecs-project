option(BUILD_SHARED_LIBS "Build shared libraries" ON)

cmake_minimum_required(VERSION 3.10)
project(infotecs_project)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES src/journal_lib.cpp)

add_library(JournalLib ${SOURCES})
target_include_directories(JournalLib PUBLIC src)

# Основной исполняемый файл
add_executable(infoprog main.cpp)

# Линковка с динамической или статической библиотекой
target_link_libraries(infoprog PRIVATE JournalLib)

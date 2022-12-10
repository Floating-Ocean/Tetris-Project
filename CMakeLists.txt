cmake_minimum_required(VERSION 3.23)
project(Tetris C)

set(CMAKE_C_STANDARD 23)

add_executable(Tetris icon/app-icon.o main.c module/Property.c module/Console.c module/File.c module/StdMethod.c module/Block.c collect/Structs.h module/Game.c module/Timer.c collect/Collection.h module/Font.c)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})

add_library(winpthread STATIC IMPORTED)
set_property(TARGET winpthread PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/libwinpthread.a)
target_link_libraries(Tetris winpthread)

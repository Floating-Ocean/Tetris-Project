cmake_minimum_required(VERSION 3.23)
project(Tetris C)

set(CMAKE_C_STANDARD 23)

add_executable(Tetris icon/app-icon.o main.c module/interface/Property.c module/util/Console.c module/util/File.c module/util/StdMethod.c module/game/Block.c collect/Structs.h module/interface/Game.c module/util/Timer.c collect/Collection.h module/util/Font.c module/util/ThemeCtrl.c collect/Colors.h module/game/Punishment.c module/game/Score.c)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})

add_library(winpthread STATIC IMPORTED)
set_property(TARGET winpthread PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/libwinpthread.a)
target_link_libraries(Tetris winpthread)

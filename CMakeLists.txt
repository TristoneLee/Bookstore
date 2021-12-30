cmake_minimum_required(VERSION 3.16)
project(Bookstore)

set(CMAKE_CXX_STANDARD 20)
#add_compile_options(-std=c++2a -fconcepts)

include_directories(
        ${PROJECT_SOURCE_DIR}/src/
        ${PROJECT_SOURCE_DIR}/bin/
)
set(src_dir
        ${PROJECT_SOURCE_DIR}/src/main.cpp
        ${PROJECT_SOURCE_DIR}/src/LogSystem.h
        ${PROJECT_SOURCE_DIR}/src/Bookstore.h
        ${PROJECT_SOURCE_DIR}/src/BookSystem.h
        ${PROJECT_SOURCE_DIR}/src/AccountSystem.h
        ${PROJECT_SOURCE_DIR}/src/MyString.h)
add_executable(code ${src_dir})

target_link_libraries(code ${PROJECT_SOURCE_DIR}/bin/libUll.a)
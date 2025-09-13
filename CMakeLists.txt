cmake_minimum_required(VERSION 3.16)

project(QLink)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Test Multimedia)

add_executable(QLink
    main.cpp
    gamewindow.cpp
    gamewindow.h
    linkjudger.cpp
    linkjudger.h
    resources.qrc
)

target_link_libraries(QLink Qt6::Widgets Qt6::Multimedia)

# 测试可执行文件
add_executable(QLinkTest
    simpletest.cpp
    simpletest.h
    linkjudger.cpp
    linkjudger.h
)

target_link_libraries(QLinkTest Qt6::Test Qt6::Widgets)


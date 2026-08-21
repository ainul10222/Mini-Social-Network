cmake_minimum_required(VERSION 3.10)
project(MiniSocialNetwork LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/GUI/include)

set(SRC
    GUI/src/main.cpp
    src/User.cpp
    src/Graph.cpp
    src/DataStore.cpp
)

set(CMAKE_AUTOMOC ON)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
set(UI_FILES GUI/forms/MainWindow.ui)
qt5_wrap_ui(UIC_HEADERS ${UI_FILES})
qt5_add_resources(QRC_SOURCES GUI/resources/resources.qrc)
list(APPEND SRC GUI/src/MainWindow.cpp GUI/include/MainWindow.h ${UIC_HEADERS} ${QRC_SOURCES})

add_executable(MiniSocialNetwork ${SRC})
target_include_directories(MiniSocialNetwork PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(MiniSocialNetwork Qt5::Widgets)

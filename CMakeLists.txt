cmake_minimum_required(VERSION 3.10)
project(MiniSocialNetwork LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_GUI "Build with Qt GUI" OFF)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/GUI/include)

set(SRC
    main.cpp
    src/User.cpp
    src/Graph.cpp
)

if(BUILD_GUI)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)
    set(UI_FILES GUI/forms/MainWindow.ui)
    qt5_wrap_ui(UIC_HEADERS ${UI_FILES})
    list(APPEND SRC GUI/src/MainWindow.cpp ${UIC_HEADERS})
    add_executable(MiniSocialNetwork ${SRC})
    target_link_libraries(MiniSocialNetwork Qt5::Widgets)
else()
    add_executable(MiniSocialNetwork ${SRC})
endif()

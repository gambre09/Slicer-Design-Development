cmake_minimum_required(VERSION 3.16)
project(SlicerQtApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Qt's automoc/autorcc/ autouic
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
# We won't use .ui files in this starter; everything is built in C++ for portability
set(CMAKE_AUTOUIC OFF)

# Find Qt (prefer Qt6, fall back to Qt5)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/MainWindow.h
    src/MainWindow.cpp
    src/CentralWidget.h
    src/CentralWidget.cpp
    resources/resources.qrc
)

target_include_directories(${PROJECT_NAME} PRIVATE src)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# On macOS and Windows, bundle the app
if (APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE TRUE)
elseif (WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

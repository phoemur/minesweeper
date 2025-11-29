cmake_minimum_required(VERSION 3.10)

project(minesweeper)
set(CMAKE_BUILD_TYPE Release)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(include)

find_package(Qt6Widgets REQUIRED)

set(QT6_LIBRARIES Qt6::Widgets)

set(TARGET ${CMAKE_PROJECT_NAME})

# Load image resources
set(RESOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/images/images.qrc
)
qt6_add_resources(RESOURCES_ADDED ${RESOURCE})

# Sources
set(SOURCES
    src/helper.cpp 
    src/save_highscore.cpp 
    src/highscore.cpp 
    src/mainwindow.cpp 
    src/minesweeper.cpp 
    src/qrightclickbutton.cpp
)

# Headers
set(HEADERS
    include/save_highscore.h 
    include/highscore.h 
    include/mainwindow.h 
    include/qrightclickbutton.h
)

# Generate MOC
qt6_wrap_cpp(minesweeper_SRC ${HEADERS})

# Add classes that doesn't need MOC
set(HEADERS ${HEADERS} include/helper.h)

# Compile / translate
add_executable(${TARGET} ${SOURCES} ${minesweeper_SRC} ${RESOURCES_ADDED})
target_compile_features(${TARGET} PUBLIC cxx_std_14)

# Link
target_link_libraries(${TARGET} ${QT6_LIBRARIES})

#Install
install (TARGETS ${TARGET} RUNTIME DESTINATION bin)

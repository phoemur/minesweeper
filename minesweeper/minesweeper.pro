######################################################################
# Automatically generated by qmake (3.1) Fri Dec 15 19:47:13 2017
######################################################################

TEMPLATE = app
TARGET = minesweeper
INCLUDEPATH += .

# The following define makes your compiler warn you if you use any
# feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
QT += widgets
QMAKE_CXXFLAGS += -std=c++11
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
HEADERS += highscore.h mainwindow.h qrightclickbutton.h
SOURCES += highscore.cpp mainwindow.cpp minesweeper.cpp qrightclickbutton.cpp

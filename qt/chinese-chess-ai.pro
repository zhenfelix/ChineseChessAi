#-------------------------------------------------
#
# Project created by QtCreator 2021-03-13T16:23:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = build/chinese-chess-ai
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++17

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS += -pg
QMAKE_LFLAGS += -pg


SOURCES += \
    main.cpp \
    src/display.cpp \
    src/stone.cpp \
    src/gameEngine.cpp \
    src/player.cpp \
    src/minimax.cpp \
    src/mcts.cpp

INCLUDEPATH += $$PWD/headers


HEADERS += \
    headers/display.h \
    headers/stone.h \
    headers/gameEngine.h \
    headers/player.h \
    headers/minimax.h \
    headers/mcts.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



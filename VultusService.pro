QT -= gui
QT += core network sql

CONFIG += c++17 console
CONFIG -= app_bundle

MOC_DIR = .build
OBJECTS_DIR = .build

SOURCES += \
        src/VultusDatabaseManager.cpp \
        src/VultusServiceCommandHandler.cpp \
        src/VultusServiceServer.cpp \
        src/main.cpp

HEADERS += \
    src/VultusDatabaseManager.h \
    src/VultusServiceCommandHandler.h \
    src/VultusServiceServer.h

#-------------------------------------------------
#
# Project created by QtCreator 2016-05-03T06:29:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = solar_system
TEMPLATE = app


SOURCES += \
    Coordinates.cpp \
    Functions.cpp \
    main.cpp \
    Planet.cpp \
    SolarInterface.cpp \
    SolarSystem.cpp \
    SpaceObject.cpp \
    Star.cpp

HEADERS  += \
    Coordinates.hpp \
    Functions.hpp \
    Planet.hpp \
    SolarInterface.hpp \
    SolarSystem.hpp \
    SpaceObject.hpp \
    Star.hpp

DESTDIR = build
OBJECTS_DIR = $${DESTDIR}/obj
MOC_DIR = $${DESTDIR}/moc
RCC_DIR = $${DESTDIR}/rcc
UI_DIR = $${DESTDIR}/ui


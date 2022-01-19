TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

target.path = /etc
INSTALLS += target

SOURCES += \
        airsensor.cpp \
        buffer.cpp \
        main.cpp

HEADERS += \
    airsensor.h \
    buffer.h

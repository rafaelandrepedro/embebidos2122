TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


SOURCES += \
        LDRsensor.cpp \
        WaterTempsensor.cpp \
        airsensor.cpp \
        buffer.cpp \
        heater.cpp \
        light.cpp \
        main.c \
        main.cpp \
        spiadc.cpp \
        stepmotor.cpp \
        waterpump.cpp

HEADERS += \
    LDRsensor.h \
    WaterTempsensor.h \
    airsensor.h \
    buffer.h \
    heater.h \
    light.h \
    spiadc.h \
    stepmotor.h \
    waterpump.h

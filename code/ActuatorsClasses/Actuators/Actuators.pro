TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        heater.cpp \
        light.cpp \
        main.cpp \
        stepmotor.cpp \
        waterpump.cpp

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Actuators.pro.user

HEADERS += \
    heater.h \
    light.h \
    stepmotor.h \
    waterpump.h

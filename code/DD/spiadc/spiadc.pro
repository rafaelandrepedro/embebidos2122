TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

SOURCES += \
    LDRsensor.cpp \
    WaterTempsensor.cpp \
    main.cpp \
    spiadc.cpp

DISTFILES += \
    spiadc.pro.user

HEADERS += \
    LDRsensor.h \
    WaterTempsensor.h \
    spiadc.h

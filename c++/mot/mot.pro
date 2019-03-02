TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    src/sessionparameters.cpp \
    src/main.cpp \
    src/cryptocontext_mot.cpp

HEADERS += \
    include/sessionparameters.h \
    include/cryptocontext_mot.h

DISTFILES += \
    LICENCE

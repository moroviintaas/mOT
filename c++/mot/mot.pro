TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -L/usr/local/lib -lgmpxx -lgmp -lcrypto

SOURCES += \
    src/cryptocontext_mot.cpp \
    src/main.cpp \
    src/sessionparameters.cpp

HEADERS += \
    include/cryptocontext_mot.h \
    include/sessionparameters.h

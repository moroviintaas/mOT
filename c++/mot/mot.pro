TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lgmp -lgmpxx -lcrypto

SOURCES += \
    src/cryptocontext_mot.cpp \
    src/main.cpp \
    src/protocolparameters.cpp

HEADERS += \
    include/cryptocontext_mot.h \
    include/protocolparameters.h

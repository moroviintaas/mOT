TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lgmp -lgmpxx -lcrypto -lpthread -lboost_system -lboost_program_options

SOURCES += \
    src/cryptocontext_mot.cpp \
    src/main.cpp \
    src/protocolparameters.cpp \
    src/serverinterface.cpp \
    src/clientinterface.cpp \
    src/protocolconstructs.cpp

HEADERS += \
    include/cryptocontext_mot.h \
    include/protocolparameters.h \
    include/serverinterface.h \
    include/clientinterface.h \
    include/protocolconstructs.h

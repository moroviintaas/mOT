IDIR = include
SRCDIR = src
CC = g++
CFLAGS=-I. -c
CXXFLAGS      = -pipe -g -std=gnu++11  -Wall -W -fPIC $(DEFINES)

ODIR = obj

LIBS= -L/usr/local/lib -lgmp -lgmpxx -lssl -lcrypto -lpthread -lboost_system -lboost_program_options

_DEPS =  protocolparameters.h cryptocontext_mot.h protocolconstructs.h serverinterface.h clientinterface.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ =  protocolparameters.o cryptocontext_mot.o protocolconstructs.o serverinterface.o clientinterface.o main.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


mot: $(OBJ)
	$(CC) -o $@ $^ $(LIBS)

$(OBJ): $(ODIR)/%.o: $(SRCDIR)/%.cpp $(DEPS)
	$(CC) $(CFLAGS) $(CXXFLAGS) -o $@ $< 

test: mot
	$(CC) -o test/userA/mot $(OBJ) $(LIBS)
	$(CC) -o test/userB/mot $(OBJ) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~

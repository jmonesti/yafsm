vpath %.cpp ./src

.PHONY: clean all

OS		= $(shell uname -s)
BIN		= ./bin/$(OS)

CXXFLAGS	+= -Iinclude
CXXFLAGS	+= -I../portable/include

all : myfsm.out

clean :
	$(RM) $(BIN)/* myfsm.out

$(BIN)/%.o : %.cpp
	-mkdir -p $(BIN)
	$(CXX) -c $(CXXFLAGS) $^ -o $@

myfsm.out : $(BIN)/main.o
	$(CXX) $(LDFLAGS) $^ -o $@

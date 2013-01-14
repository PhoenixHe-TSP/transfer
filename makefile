CXX= clang++
CXXFLAGS= -std=c++11 -O0
LIBS=stdc++ boost_system boost_thread boost_regex boost_random boost_date_time websocketpp pthread m
LDFLAGS=$(LIBS:%= -l%)

BIN=sev cli
OBJ=$(BIN:%=%.o)

all:$(BIN)

sev: sev.o
cli: cli.o

clean:
	rm -rf $(BIN) $(OBJ)

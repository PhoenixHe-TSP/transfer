CXX= clang++
CXXFLAGS=-std=c++11 -g -Wall
LIBS=websockets boost_system boost_thread boost_regex boost_random boost_date_time stdc++ m
LDFLAGS= $(LIBS:%= -l%) -pthread

BIN=server client
OBJ=$(BIN:%=%.o)

all:$(BIN)

server.o: server.cpp common.hpp libwebsockets_server.hpp tf_server.hpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)
client.o: client.cpp common.hpp libwebsockets_client.hpp tf_client.hpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)


server: server.o 
client: client.o

clean:
	rm -rf $(BIN) $(OBJ)

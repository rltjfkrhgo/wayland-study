CC = g++

all: server

server: server.cpp
	$(CC) -o $@ $< -lwayland-server

clean:
	rm -rf *.o server
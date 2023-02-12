CC = g++

all: server globals

server: server.cpp
	$(CC) -o $@ $< -lwayland-server

client: client.cpp
	$(CC) -o $@ $< -lwayland-client

globals: globals.cpp
	$(CC) -o $@ $< -lwayland-client

clean:
	rm -rf *.o server client globals
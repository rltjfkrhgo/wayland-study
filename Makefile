CC = g++

all: server2 globals

server: server.cpp
	$(CC) -o $@ $< -lwayland-server

client: client.cpp
	$(CC) -o $@ $< -lwayland-client

globals: globals.cpp
	$(CC) -o $@ $< -lwayland-client

server2: server2.cpp
	$(CC) -o $@ $< -lwayland-server

clean:
	rm -rf *.o server client globals server2
CC = g++

all: server client

server: server.cpp
	$(CC) -o $@ $< -lwayland-server

client: client.cpp
	$(CC) -o $@ $< -lwayland-client

clean:
	rm -rf *.o server client
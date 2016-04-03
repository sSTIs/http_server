all: server

server: server_socket.o server.o on_accept.o
	g++ server.o server_socket.o on_accept.o -o server

browser: server_socket.o browser.o on_accept.o
	g++ browser.o server_socket.o on_accept.o -o browser

server.o: server.cpp
	g++ -c server.cpp

server_socket.o: server_socket.cpp
	g++ -c server_socket.cpp

on_accept.o: on_accept.cpp
	g++ -c on_accept.cpp

browser.o: browser.cpp
	g++ -c browser.cpp

clean:
	rm -rf *.o server

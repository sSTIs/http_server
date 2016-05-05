all: server mjs

server: server_socket.o server.o on_accept.o cgi_handler.o
	g++ server.o server_socket.o on_accept.o cgi_handler.o -o server

browser: server_socket.o browser.o on_accept.o
	g++ browser.o server_socket.o on_accept.o -o browser

mjs: lex.o syntax.o mjs.o
	g++ lex.o syntax.o mjs.o -o mjs

mjs.o: mjs.cpp
	g++ -c mjs.cpp

syntax.o: syntax.cpp syntax.h
	g++ -c syntax.cpp

lex.o: lex.cpp lex.h
	g++ -c lex.cpp

server.o: server.cpp
	g++ -c server.cpp

server_socket.o: server_socket.cpp
	g++ -c server_socket.cpp

on_accept.o: on_accept.cpp
	g++ -c on_accept.cpp

cgi_handler.o: cgi_handler.cpp
	g++ -c cgi_handler.cpp

browser.o: browser.cpp
	g++ -c browser.cpp

clean:
	rm -rf *.o server

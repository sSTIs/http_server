Created by sSTIs

make all
run server: ./server
server_socket.h - header of server_socket.cpp - Socket classes
			 and on_accept.cpp - request processing;
browser.cpp - not need, just for tests;
Version 2:
	test with Yandex Browser and Safari
	Server can open .html .txt .jpg files 
has responses 
	200 OK
	403 wrong file type
	404 could’n open file 
	501 not GET or HEAD

Version 1:
	Browser sends const request (GET /), server sends (501 Not Implemented)
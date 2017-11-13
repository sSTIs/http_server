Created by sSTIs

make all
run server: ./server
server_socket.h - header of server_socket.cpp - Socket classes
			 and on_accept.cpp - request processing;
browser.cpp - not need, just for tests;
Version 3:
	run cgi programs with parametrs 
	ex: get /cgi-bin/test?asd=1&qwe=2
	makes env for these programs
Version 2:
	test with Yandex Browser and Safari
	Server can open .html .txt .jpg (.jpeg in v3 because I forget) .ico files 
has responses 
	200 OK
	403 wrong file type
	404 could’n open file 
	501 not GET or HEAD

Version 1:
	Browser sends const request (GET /), server sends (501 Not Implemented)
asd
zxc
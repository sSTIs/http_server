//
//  browser.cpp
//  
//
//  Created by sSTIs on 05.03.16.
//
//

#include <iostream>
#include "server_socket.h"

using namespace std;

//! Functions.
//!======================================================================================
void run_client();

//! Main.
//!======================================================================================
int main()
{
    
    run_client();
    
    return 0;
}

//!======================================================================================

void run_client()
{
    ConnectionSocket socket;
    SocketAddress address("192.168.1.100", 8000);
    socket.connect(&address);
    char *get = (char *) "GET/ HTTP/1.1 \r\n\r\n";
    socket.send(get, strlen(get));
    char buffer[10240];
    int n = socket.receive(buffer, sizeof(buffer));
    buffer[n]=0;
    cout << buffer;
    n = socket.receive(buffer, sizeof(buffer));
    buffer[n]=0;
    cout << buffer;

}

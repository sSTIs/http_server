//
//  test.cpp
//  
//
//  Created by sSTIs on 04.04.16.
//
//

#include <iostream>

using namespace std;

//! Functions.
//!======================================================================================

//! Main.
//!======================================================================================
int main()
{
    cout << "Content-type: text/html\r\n\r\n";
    cout << "<html><body>";
    cout << "Hello world\r\n";
    cout << "you have entered arguments : ";
    exit(10);
    cout << getenv("QUERY_STRING");

    cout << "</body></html>";

    return 0;
}

//!======================================================================================

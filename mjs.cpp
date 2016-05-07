//
//  mjs.cpp
//  
//
//  Created by sSTIs on 04.05.16.
//
//

#include <iostream>
#include "syntax.h"

using namespace std;

//! Main.
//!======================================================================================
int main(int argc, char **argv)
{
    if (argc != 2)
        exit(1);
    
    //cout << "Content-type: text/html\r\n\r\n"; //temporary for tests
    try
    {
        Parser parser(argv[1]);
        parser.analyze();
    }
    catch (string& s)
    {
        cerr << "Lex error: "<< s <<endl;
    }
    catch (Lex& l)
    {
        cerr << "Syntax error: " << l << endl;
    }
    catch (...)
    {
        cerr << "All very bad" << endl;
    }
    cout << "end of analyze"<< endl;
    //Lex analyze
    /*Scanner scanner(argv[1]);
    Lex a;
    try
    {
        do
        {
            a = scanner.get_lex();
            cout << a;
        }while (a.get_type() != LEX_FIN);
    }
    catch (string& s)
    {
        cerr << s;
    }*/
    return 0;
}

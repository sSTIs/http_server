//
//  mjs.cpp
//  
//
//  Created by sSTIs on 04.05.16.
//
//

#include <iostream>
#include "interpretator.h"

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
        //syntax analyze
        /*Parser parser(argv[1]);
        parser.analyze();*/
        Interpretator interpr(argv[1]);
        interpr.interpretation();
    }
    catch (string& s)
    {
        cerr << s <<endl;
    }
    catch (Lex& l)
    {
        cerr << "Syntax error: " << l << endl;
    }
    catch (...)
    {
        cerr << "All very bad" << endl;
    }
    //cout << "end of all program"<< endl;
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

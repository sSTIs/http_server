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
    try
    {
        Scanner scanner(argv[1]);
        Lex a(LEX_NULL);
        do
        {
            a = scanner.get_lex();
            cout << a.get_type() << " ";
            if (a.get_type() == LEX_NUM)
                cout << a.get_ivalue();
            else if ( a.get_type() == LEX_STR)
                cout << a.get_svalue();
            else if (a.get_type() == LEX_BOOL)
                cout << a.get_bvalue();
            cout << endl;
        }while (a.get_type() != LEX_FIN);
    }
    catch (string s)
    {
        cout << s;
    }
    
    return 0;
}

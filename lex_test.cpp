//
//  lex_test.cpp
//  
//
//  Created by sSTIs on 25.04.16.
//
//

#include <iostream>
#include "lex.h"

using namespace std;

//! Main.
//!======================================================================================
int main(int argc, char **argv)
{
    if (argc != 2)
        exit(1);
    Scanner scanner(argv[1]);
    Lex a;
    do
    {
        a = scanner.get_lex();
        cout << a.get_type() << " "<< a.get_value()<< endl;
    }while (a.get_type() != LEX_FIN);

    
    return 0;
}

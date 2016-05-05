//
//  syntax.h
//
//
//  Created by sSTIs on 01.05.16.
//
//

#include <lex.h>

using namespace std;

class Parser
{
    Lex curr_lex;
    type_of_lex curr_lex_type;
    Scanner scanner;
    stack <int, 1000> stack_int;
    stack <bool, 1000> stack_bool;
    stack <string, 1000> stack_string;
    stack <type_of_lex, 1000> stack_type;
    
    //recursive descent
    Program();
    Program1();
    Command();
    Expression();
    Expression1();
    Expression2();
    Expression3();
    Summand();
    Factor();
    
    /*//semantic actions
    void dec(type_of_lex type);
    void check_id();
    void check_op();
    void check_not();
    void eq_type();
    void eq_bool();
    void check_id_in_read();*/
    
    void get_lex();
    
public:
    //Poliz prog;
    Parser(const char *program);
    void analyze();
};

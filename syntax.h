//
//  syntax.h
//
//
//  Created by sSTIs on 01.05.16.
//
//

#include "lex.h"

using namespace std;

template <class T, int max_size >
class Stack
{
    T buf[max_size];
    int top;
public:
    Stack() { top = 0; }
    void reset() { top = 0; }
    void push(T i);
    T pop();
    bool is_empty(){ return top == 0; }
    bool is_full() { return top == max_size; }
};

class Parser
{
    Lex curr_lex;
    type_of_lex curr_lex_type;
    Scanner scanner;
    //Stack <Lex, 1000> stack_lex;
    Stack <type_of_lex, 1000> stack_type;
    
    //recursive descent
    void Program();
    void Program1();
    void Command();
    void Declaration();
    void Expression();
    void Expression1();
    void Expression2();
    void Expression3();
    void Summand();
    void Factor();
    
    //semantic actions
    //void dec(type_of_lex type);
    //void check_id();
    void check_op();
    //void check_not();
    //void eq_type();
    void eq_bool();
    
    void get_lex();
    
public:
    //Poliz prog;
    Parser(const char *program);
    void analyze();
};

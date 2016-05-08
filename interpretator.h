//
//  interpretator.h
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

// like verctor
class Poliz
{
    Lex *buf;
    int size;
    int free;
public:
    Poliz (int max_size)
    {
        buf = new Lex [size = max_size];
        free = 0;
    }
    ~Poliz() { delete []buf; };
    void put_lex(Lex l) { buf[free]=l; ++free; };
    void put_lex(Lex l, int place) { buf[place]=l; };
    void put_free_space() { ++free; };
    int get_free() { return free; };
    Lex& operator[] (int index);
    void print();
};

class Parser
{
    Lex curr_lex;
    type_of_lex curr_lex_type;
    Scanner scanner;
    
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
    void check_declare(int num_in_TID);
    void declare(int num_in_TID);
    
    void get_lex();
    
public:
    Poliz program_poliz;
    Parser(const char *program);
    void analyze();
};
//! types conversion
//!============================================================================
bool toBool(Lex lexem);

class Executer
{
    Lex curr_lex;
public:
    void execute( Poliz& poliz);
};

class Interpretator
{
    Parser parser;
    Executer executer;
public:
    Interpretator(char *program): parser(program) {};
    void interpretation();
};

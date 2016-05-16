//
//  interpretator.h
//
//
//  Created by sSTIs on 01.05.16.
//
//

#include "lex.h"

using namespace std;

// like verctor
class Poliz
{
    Lex *buf;
    int size;
    int second_free;// one position for main, other for functions
    int free;
public:
    Poliz (int max_size);
    
    ~Poliz() { delete []buf; };
    void put_lex(Lex l) { buf[free]=l; ++free; };
    void put_lex(Lex l, int place) { buf[place]=l; };
    void put_free_space() { ++free; };
    int get_free() { return free; };
    void change_free(); //change main <-> function
    Lex& operator[] (int index);
    void print(); // works if free on main
};

class Parser
{
    Lex curr_lex;
    type_of_lex curr_lex_type;
    Scanner scanner;
    int in_cycle; // deep of cycle
    Stack <int, 100> cycle_begin;
    
    //recursive descent
    void Program();
    void Program1();
    void Command();
    void Cycle();
    void Declaration();
    void Expression();
    void Expression1();
    void Expression2();
    void Expression3();
    void Expression4();
    void Expression5();
    void Summand();
    void Factor();
    
    //semantic actions
    void check_declare(int num_in_TID);
    void declare(int num_in_TID);
    void undeclare();
    
    void get_lex();
    
public:
    int area_visibility;
    Poliz program_poliz;
    Parser(const char *program);
    bool analyze();// 1 if not end 0 if EOF
};

class Executer
{
    Lex curr_lex;
    int area_visibility;
    Stack <Lex, 500> return_address;
    int index;
public:
    Executer();
    void execute(Poliz& poliz);
    
    //types conversation
    //get LEX_NUM ... LEX_STR from LEX_ADDRESS
    void from_adreess_to_value(Lex& lexem);
    bool toBool(Lex lexem);
    int toInt(Lex lexem);
    string toString(Lex lexem);
};

class Interpretator
{
    Parser parser;
    Executer executer;
public:
    Interpretator(char *program): parser(program) {};
    void interpretation();
};

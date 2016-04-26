//
//  lex.h
//
//
//  Created by sSTIs on 23.04.16.
//
//

#include <iostream>
#include <cstring>
#include <cstdio>
#include <string>

using namespace std;

enum type_of_lex
{
    LEX_NULL, //0
    LEX_BOOL, //1
    LEX_NUM, //2
    LEX_STR, //3
    LEX_ID, //4
    LEX_ADD, //5
    LEX_SUB, //6
    LEX_MUL, //7
    LEX_DIV, //8
    LEX_PERCENT, //9
    LEX_ASSIGN, //10
    LEX_INC,  //11
    LEX_DEC,  //12
    LEX_AND, //13
    LEX_OR, //14
    LEX_EQ, //15
    LEX_NEQ, //16
    LEX_G, //17
    LEX_GE,  //18
    LEX_L, //19
    LEX_LE, //20
    LEX_FUNC, //21
    LEX_VAR, //22
    LEX_IF, //23
    LEX_WHILE, //24
    LEX_FOR, //25
    LEX_DO, //26
    LEX_IN, //27
    LEX_BREAK, //28
    LEX_CONTINUE, //29
    LEX_RETURN, //30
    LEX_TYPEOF, //31
    LEX_DOT, //32
    LEX_COMMA, //33
    LEX_SEMICOLON, //34
    LEX_LPAREN, //35
    LEX_RPAREN, //36
    LEX_LPAREN_SQ, //37
    LEX_RPAREN_SQ, //38
    LEX_WRITE, //39
    LEX_READ, //40
    LEX_BEGIN, //41
    LEX_END, //42
    LEX_FIN //43
};

class Lex //lexem, that has type and value
{
    type_of_lex t_lex;
    int iv_lex;
    bool bv_lex;
    string sv_lex;
public:
    Lex(type_of_lex t = LEX_NULL, int iv = 0);
    Lex(type_of_lex t = LEX_NULL, bool bv = false);
    Lex(type_of_lex t = LEX_NULL, string sv = string(""));
    
    type_of_lex get_type();
    int get_ivalue();
    string get_svalue();
    bool get_bvalue();
    friend ostream& operator << (ostream& stream, Lex lexem);
};

class Ident //identificator
{
    char *name;
    bool declare;
    type_of_lex type;
    bool assign;
    int value;
public:
    Ident();
    char *get_name();
    void put_name(char *n);
    bool get_declare();
    void put_declare();
    type_of_lex get_type();
    void put_type(type_of_lex t);
    bool get_assign();
    void put_assign();
    int get_value();
    void put_value(int v);
};

class Table_ident //table of idents
{
    Ident *idents;
    int size;
    int top;
public:
    Table_ident(int max_size);
    ~Table_ident();
    Ident& operator[](int index);
    int put(char *buf);
};

class Scanner //Scan file and make table of idents, lexems
{
    enum state {H, IDENT, NUMB, NEQ,COMPARE, DELIM, COMM, COMM2, COMM3,
                DOUBLE, STR1, STR2};
    static char *TW[]; //table of words
    static type_of_lex words[]; //type of words from TW
    static char *TD[]; //table of delims
    static type_of_lex delims[];
    state curr_state;
    FILE *program_file;
    char c;
    char buf[80];
    int buf_top;
    
    void clear();
    void add();
    int look(const char *buf, char **list);
    void gc();
public:
    Scanner(const char *program);
    Lex get_lex();
};

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
    LEX_NOT, //15
    LEX_EQ, //16
    LEX_NEQ, //17
    LEX_G, //18
    LEX_GE,  //19
    LEX_L, //20
    LEX_LE, //21
    LEX_FUNC, //22
    LEX_VAR, //23
    LEX_IF, //24
    LEX_ELSE, //25
    LEX_WHILE, //26
    LEX_FOR, //27
    LEX_DO, //28
    LEX_IN, //29
    LEX_BREAK, //30
    LEX_CONTINUE, //31
    LEX_RETURN, //32
    LEX_TYPEOF, //33
    LEX_DOT, //34
    LEX_COMMA, //35
    LEX_SEMICOLON, //36
    LEX_LPAREN, //37
    LEX_RPAREN, //38
    LEX_LPAREN_SQ, //39
    LEX_RPAREN_SQ, //40
    LEX_WRITE, //41
    LEX_READ, //42
    LEX_GETENV, //43
    LEX_LEN, //44
    LEX_BEGIN, //45
    LEX_END, //46
    LEX_FIN //47
};

class Lex //lexem, that has type and value
{
    type_of_lex t_lex;
    int type_of_value;// 0 1i 2b 3s
    int iv_lex;
    bool bv_lex;
    string sv_lex;
public:
    Lex(type_of_lex t, int iv);
    Lex(type_of_lex t, bool bv);
    Lex(type_of_lex t, string sv);
    explicit Lex(type_of_lex t);
    Lex(const Lex& l);
    
    type_of_lex get_type() const;
    int get_type_of_value() const;
    int get_ivalue() const;
    string get_svalue() const;
    bool get_bvalue() const;
    //friend ostream& operator << (ostream& stream, Lex lexem);
};

class Ident //identificator
{
    char *name;
    bool declare;
    type_of_lex type;
    bool assign;
public:
    int ivalue;
    bool bvalue;
    string svalue;
    int type_of_value;
    
    Ident();
    char *get_name();
    void put_name(char *n);
    bool get_declare();
    void put_declare();
    type_of_lex get_type();
    void put_type(type_of_lex t);
    bool get_assign();
    void put_assign();
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

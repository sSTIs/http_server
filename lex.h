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
    LEX_NULL,
    LEX_UNDEF,
    LEX_BOOL,
    LEX_NUM,
    LEX_STR,
    LEX_ID,
    LEX_ADD,
    LEX_SUB,
    LEX_MUL,
    LEX_DIV,
    LEX_PERCENT,
    LEX_ASSIGN,
    LEX_INC,
    LEX_DEC,
    LEX_AND,
    LEX_OR,
    LEX_NOT,
    LEX_EQ,
    LEX_NEQ,
    LEX_G,
    LEX_GE,
    LEX_L,
    LEX_LE,
    LEX_FUNC,
    LEX_VAR,
    LEX_IF,
    LEX_ELSE,
    LEX_WHILE,
    LEX_FOR,
    LEX_DO,
    LEX_IN,
    LEX_BREAK,
    LEX_CONTINUE,
    LEX_RETURN,
    LEX_TYPEOF,
    LEX_DOT,
    LEX_COMMA,
    LEX_SEMICOLON,
    LEX_LPAREN,
    LEX_RPAREN,
    LEX_LPAREN_SQ,
    LEX_RPAREN_SQ,
    LEX_WRITE,
    LEX_READ,
    LEX_ENV,
    LEX_LEN,
    LEX_BEGIN,
    LEX_END,
    LEX_FIN,
    POLIZ_GO,
    POLIZ_FGO,
    POLIZ_ADDRESS,
    POLIZ_LABEL
};

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
    bool is_empty() { return top == 0; }
    bool is_full() { return top == max_size; }
};

class Lex //lexem, that has type and value
{
    type_of_lex t_lex;
    int type_of_value;// 0 undef 1i 2b 3s ?4 null?
    int iv_lex;
    bool bv_lex;
    string sv_lex;
public:
    bool number_NaN;
    
    Lex(type_of_lex t, int iv);
    Lex(type_of_lex t, bool bv);
    Lex(type_of_lex t, string sv);
    Lex();
    explicit Lex(type_of_lex t);
    Lex(const Lex& l);
    Lex& operator =(const Lex& l);
    friend ostream& operator << (ostream& os, const Lex& l);
    
    type_of_lex get_type() const;
    int get_type_of_value() const;
    int get_ivalue() const;
    string get_svalue() const;
    bool get_bvalue() const;
};

class Ident //identificator
{
    char *name;
public:
    int declare; //area of visibility -1 no 0 main  ... functions
    Stack<int,20> prev_declare;
    Stack<Lex,20> prev_value;
    int type_of_value;// ... 5 function  ivalue = label of function
    int ivalue;
    bool bvalue;
    string svalue;
    
    Ident();
    ~Ident();
    char *get_name();
    void put_name(char *n);
    type_of_lex get_type();
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
    int get_top() const;
};

class Scanner //Scan file and make table of idents, lexems
{
    enum state {OUT, H, IDENT, NUMB, NEQ, COMPARE, LESS, DELIM, COMM, COMM2, COMM3,
                DOUBLE, STR1, STR2};
    static char *TW[]; //table of words
    static type_of_lex words[]; //type of words from TW
    static char *TD[]; //table of delims
    static type_of_lex delims[];
    state curr_state;
    FILE *program_file;
    char c;
    char buf[1000];
    int buf_top;
    
    void clear();
    void add();
    int look(const char *buf, char **list);
    void gc();
public:
    Scanner(const char *program);
    Lex get_lex();
};

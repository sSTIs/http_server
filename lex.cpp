//
//  lex.cpp
//
//
//  Created by sSTIs on 23.04.16.
//
//

#include "lex.h"

using namespace std;

//! class Lex
//!======================================================================================
Lex::Lex(type_of_lex t, int iv)
{
    t_lex = t;
    iv_lex = iv;
}

Lex::Lex(type_of_lex t, string sv)
{
    t_lex = t;
    sv_lex = sv;
}

Lex::Lex(type_of_lex t, bool bv)
{
    t_lex = t;
    bv_lex = bv;
}

type_of_lex Lex::get_type()
{
    return t_lex;
}

int Lex::get_ivalue()
{
    return iv_lex;
}

string Lex::get_svalue()
{
    return sv_lex;
}

bool Lex::get_bvalue()
{
    return bv_lex;
}
//! class Ident
//!======================================================================================
Ident::Ident()
{
    declare = false;
    assign = false;
}

char *Ident::get_name()
{
    return name;
}

void Ident::put_name(char *n)
{
    name = new char [strlen(n) + 1];
    strcpy(name, n);
}

bool Ident::get_declare()
{
    return declare;
}

void Ident::put_declare()
{
    declare = true;
}

type_of_lex Ident::get_type()
{
    return type;
}

void Ident::put_type(type_of_lex t)
{
    type = t;
}

bool Ident::get_assign()
{
    return assign;
}

void Ident::put_assign()
{
    assign = true;
}

int Ident::get_value()
{
    return value;
}

void Ident::put_value(int v)
{
    value = v;
}

//! class table_ident
//!======================================================================================
Table_ident::Table_ident(int max_size)
{
    idents = new Ident[size = max_size];
    top = 1;
}

Table_ident::~Table_ident()
{
    delete []idents;
}

Ident& Table_ident::operator[](int index)
{
    return idents[index];
}

int Table_ident::put(char *buf)
{
    for (int j = 1; j < top; j++)
        if (!strcmp(buf, idents[j].get_name()))
            return j;
    idents[top].put_name(buf);
    ++top;
    return top-1;
}

//! class Scanner
//!======================================================================================
void Scanner::clear()
{
    buf_top = 0;
    for (int j = 0; j < 80; j++)
        buf[j] = '\0';
}

void Scanner::add()
{
    buf[buf_top++] = c;
}

int Scanner::look(const char *buf, char **list)
{
    int i = 0;
    while(list[i])
    {
        if (!strcmp(buf, list[i]))
            return i;
        ++i;
    }
    return 0;
}

void Scanner::gc()
{
    c = fgetc(program_file);
}

Scanner::Scanner(const char *program)
{
    program_file = fopen(program, "r");
    if (program_file == NULL)
        exit(2);
    curr_state = H;
    clear();
    gc();
}

Table_ident TID(100);

char * Scanner::TW[] =
{
    "",
    "function",
    "var",
    "if",
    "while",
    "for",
    "do",
    "in",
    "break",
    "continue",
    "return",
    "typeof",
    "write",
    "read",
    "true",
    "false",
    NULL
};

type_of_lex Scanner::words[] =
{
    LEX_NULL, LEX_FUNC, LEX_VAR, LEX_IF,
    LEX_WHILE, LEX_FOR, LEX_DO, LEX_IN, LEX_BREAK, LEX_CONTINUE, LEX_RETURN,
    LEX_TYPEOF, LEX_WRITE, LEX_READ, LEX_BOOL, LEX_BOOL, LEX_NULL

};

char *Scanner::TD[] =
{
    "","+","-","*","/","%","=",
    "++","--","&&","||",
    "==","!=",">",">=","<","<=",
    ".",",",";",
    "(",")","[","]",
    "{","}",""
};

type_of_lex Scanner::delims[] =
{
    LEX_NULL, LEX_ADD, LEX_SUB, LEX_MUL, LEX_DIV, LEX_PERCENT, LEX_ASSIGN,
    LEX_INC, LEX_DEC, LEX_AND, LEX_OR,
    LEX_EQ, LEX_NEQ, LEX_G, LEX_GE, LEX_L, LEX_LE,
    LEX_DOT, LEX_COMMA, LEX_SEMICOLON,
    LEX_LPAREN,LEX_RPAREN, LEX_LPAREN_SQ, LEX_RPAREN_SQ,
    LEX_BEGIN, LEX_END,
    LEX_NULL
};

Lex Scanner::get_lex()
{
    int digit, j;
    curr_state = H;
    do
    {
        switch (curr_state)
        {
            case H:
                if (c == ' ' || c == '\n' || c == '\r' || c == '\t')
                    gc();
                else if (isalpha(c))
                {
                    clear();
                    add();
                    gc();
                    curr_state = IDENT;
                }
                else if (isdigit(c))
                {
                    digit = c - '0';
                    gc();
                    curr_state = NUMB;
                }
                else if (c == '!')
                {
                    clear();
                    add();
                    gc();
                    curr_state = NEQ;
                }
                else if (c == '=' || c == '>' || c == '<')
                {
                    clear();
                    add();
                    gc();
                    curr_state = COMPARE;
                }
                else if (c == EOF)
                    return Lex(LEX_FIN, 0);
                else if (strchr((char *)"+-&|", c))
                {
                    clear();
                    add();
                    gc();
                    curr_state = DOUBLE;
                }
                else if (c == '"')
                {
                    clear();
                    gc();
                    curr_state = STR1;
                }
                else
                    curr_state = DELIM;
                break;
                
            case IDENT:
                if (isalpha(c) || isdigit(c))
                {
                    add();
                    gc();
                }
                else
                {
                    j = look(buf, TW);
                    if (j)
                    {
                        if (!strcmp(buf, "true"))
                            return Lex(words[j], true);
                        else if (!strcmp(buf, "false"))
                            return Lex(words[j], false);
                            
                        return Lex(words[j], j);
                    }
                    else
                    {
                        j = TID.put(buf);
                        return Lex(LEX_ID, j);
                    }
                }
                break;
                
            case NUMB:
                if (isdigit(c))
                {
                    digit = digit * 10 + (c - '0');
                    gc();
                }
                else
                    return Lex(LEX_NUM, digit);
                break;
                
            case NEQ:
                if (c == '=')
                {
                    add();
                    gc();
                    return Lex(LEX_NEQ, 0);
                }
                else
                    throw "!= expected\n";
                break;
                
            case COMPARE:
                if (c == '=')
                {
                    add();
                    gc();
                    j = look(buf, TD);
                    return Lex(delims[j], j);
                }
                else
                {
                    j = look(buf, TD);
                    return Lex(delims[j], j);
                }
                break;
                
            case DELIM:
                if (strchr((char *)"*%.,;[]{}()", c))
                {
                    clear();
                    add();
                    gc();
                    j = look(buf, TD);
                    return Lex(delims[j], j);
                }
                else if (c == '/')
                {
                    clear();
                    add();
                    gc();
                    curr_state = COMM;
                }
                else
                    throw string("unexpected symbol ") + c + string("\n");
                break;
                
            case COMM:
                if (c == '*')
                {
                    clear();
                    gc();
                    curr_state = COMM2;
                }
                else
                {
                    j = look(buf, TD);
                    return Lex(delims[j], j);
                }
                break;
                
            case COMM2:
                if (c == EOF)
                    throw "close comment\n";
                else if (c == '*')
                {
                    gc();
                    curr_state = COMM3;
                }
                else
                    gc();
                break;
                
            case COMM3:
                if (c == '/')
                {
                    gc();
                    curr_state = H;
                }
                else
                    curr_state = COMM2;
                break;
                
            case DOUBLE:
                if (buf[0] == c)
                {
                    add();
                    gc();
                    j = look(buf, TD);
                    return Lex(delims[j], j);
                }
                else
                {
                    j = look(buf, TD);
                    return Lex(delims[j], j);
                }
                break;
                
            case STR1:
                if (c == '\\')
                {
                    gc();
                    curr_state = STR2;
                }
                else if (c == EOF)
                {
                    throw "close \"\n";
                }
                else if (c == '\"')
                {
                    gc();
                    return Lex(LEX_STR, string(buf));
                }
                else
                {
                    add();
                    gc();
                }
                break;
                
            case STR2:
                if (c == EOF)
                    throw string("close \"\n");
                else
                {
                    add();
                    gc();
                    curr_state = STR1;
                }
                break;
        }
    } while (true);
}

//
//  lex.cpp
//
//
//  Created by sSTIs on 23.04.16.
//
//

#include "lex.h"

using namespace std;

//! class Stack
//!======================================================================================
template <class T, int max_size >
void Stack <T, max_size >::push(T i) {
    if ( !is_full() )
    {
        buf[top] = i;
        ++top;
    }
    else throw string("Stack_is_full");
}

template <class T, int max_size >
T Stack <T, max_size >::pop() {
    if ( !is_empty() )
    {
        --top;
        return buf[top];
    }
    else throw string("Stack_is_empty");
}

//! class Lex
//!======================================================================================
Lex::Lex()
{
    t_lex = LEX_NULL;
    iv_lex = 0;
    bv_lex = false;
    sv_lex = string("");
    type_of_value = 0;
}
Lex::Lex(type_of_lex t, int iv)
{
    t_lex = t;
    iv_lex = iv;
    bv_lex = false;
    sv_lex = string("");
    type_of_value = 1;
}

Lex::Lex(type_of_lex t, string sv)
{
    t_lex = t;
    sv_lex = sv;
    iv_lex = 0;
    bv_lex = false;
    type_of_value = 3;
}

Lex::Lex(type_of_lex t, bool bv)
{
    t_lex = t;
    bv_lex = bv;
    iv_lex = 0;
    sv_lex = string("");
    type_of_value = 2;
}

Lex::Lex(type_of_lex t)
{
    t_lex = t;
    bv_lex = false;
    iv_lex = 0;
    sv_lex = string("");
    type_of_value = 0;
}

Lex::Lex(const Lex& l)
{
    t_lex = l.get_type();
    type_of_value = l.get_type_of_value();
    iv_lex = l.get_ivalue();
    bv_lex = l.get_bvalue();
    sv_lex = l.get_svalue();
}

Lex& Lex::operator =(const Lex& l)
{
    if (this != &l)
    {
        t_lex = l.get_type();
        type_of_value = l.get_type_of_value();
        iv_lex = l.get_ivalue();
        bv_lex = l.get_bvalue();
        sv_lex = l.get_svalue();
        return *this;
    }
    else return *this;
}

char *lexems_in_string[] =
{
    "null", "undefined", "bool", "number", "string", "identificator", "+", "-", "*","/","%","=","++","--",
    "&&","||","!","==","!=",">",">=","<","<=","function","var","if","else","while","for","do",
    "in","break","continue","return","typeof",".",",",";","(",")","[","]","write",
    "read","Environment", "length","{","}", "part final","final", "Poliz go", "Poliz false go",
    "Poliz address", "Poliz label", "Poliz call"
};

ostream& operator << (ostream& os, const Lex&l)
{
    cout << lexems_in_string[(int) l.t_lex]<< " ";
    if (l.type_of_value == 1)
        cout << l.iv_lex;
    else if (l.type_of_value == 2)
        cout << l.bv_lex;
    else if (l.type_of_value == 3)
        cout << l.sv_lex;
    cout << endl;
    return os;
}

type_of_lex Lex::get_type() const
{
    return t_lex;
}

int Lex::get_type_of_value() const
{
    return type_of_value;
}

int Lex::get_ivalue() const
{
    return iv_lex;
}

string Lex::get_svalue() const
{
    return sv_lex;
}

bool Lex::get_bvalue() const
{
    return bv_lex;
}

//! class Ident
//!======================================================================================
Ident::Ident()
{
    declare = -1;
    type_of_value = 0;
    parametrs_number = 0;
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

Ident::~Ident()
{
    delete []name;
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

int Table_ident::put(char *buf, int area_visibility)
{
    for (int j = 1; j < top; j++)
        if (idents[j].declare == area_visibility && !strcmp(buf, idents[j].get_name()))
            return j;
    idents[top].put_name(buf);
    idents[top].declare = area_visibility;
    ++top;
    return top-1;
}

int Table_ident::get_top() const
{
    return top;
}

void Table_ident::undeclare(int area_visibility)
{
    for (int i = 1; i < top; ++i)
        if ((*this)[i].declare == area_visibility)
            (*this)[i].type_of_value = 0;
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
    {
        cerr << "File didn't open" << endl;
        exit(2);
    }
    curr_state = OUT;
    while (c != '\n')
        gc();
    clear();
    gc();
}

Table_ident TID(1000);

char * Scanner::TW[] =
{
    "",
    "function",
    "var",
    "if",
    "else",
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
    "Environment",
    "length",
    "true",
    "false",
    NULL
};

type_of_lex Scanner::words[] =
{
    LEX_NULL, LEX_FUNC, LEX_VAR, LEX_IF, LEX_ELSE,
    LEX_WHILE, LEX_FOR, LEX_DO, LEX_IN, LEX_BREAK, LEX_CONTINUE, LEX_RETURN,
    LEX_TYPEOF, LEX_WRITE, LEX_READ, LEX_ENV, LEX_LEN, LEX_BOOL, LEX_BOOL, LEX_NULL

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
    if (curr_state != OUT)
        curr_state = H;
    int in_script_tag = 0;// 1 if in <SCRIPT ...> 2 if in </SCRIPT>
    do
    {
        switch (curr_state)
        {
            case OUT: //special case to detect script
                if (c == EOF)
                {
                    return Lex(LEX_FIN);
                }
                else if (c == '<' && in_script_tag == 0)
                {
                    clear();
                    add();
                    gc();
                    in_script_tag = 1;
                }
                else if (c == '>' && in_script_tag != 0)
                {
                    add();
                    gc();
                    if (in_script_tag == 1)
                    {
                        if (!strcmp(buf, "<SCRIPT LANGUAGE=\"mjs\" RUNAT=\"server\">"))
                        {
                            clear();
                            curr_state = H;
                        }
                        else
                        {
                            cout << buf;
                        }
                    }
                    else if (in_script_tag == 2)
                    {
                        if (!strcmp(buf, "</SCRIPT>"))
                        {
                            in_script_tag = 0;
                            clear();
                            return Lex(LEX_FIN_PART);
                        }
                        else
                        {
                            cerr << "Lex error </";
                        }
                    }
                    in_script_tag = 0;
                }
                else if (in_script_tag != 0)
                {
                    add();
                    gc();
                }
                else if (in_script_tag == 0)
                {
                    cout << c;
                    gc();
                }
                break;
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
                else if (c == '=' || c == '>')
                {
                    clear();
                    add();
                    gc();
                    curr_state = COMPARE;
                }
                else if (c == '<')
                {
                    clear();
                    add();
                    gc();
                    curr_state = LESS;
                }
                else if (c == EOF)
                    return Lex(LEX_FIN);
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
                            
                        return Lex(words[j]);
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
                    return Lex(LEX_NEQ);
                }
                else
                    return Lex(LEX_NOT);
                break;
                
            case COMPARE:
                if (c == '=')
                {
                    add();
                    gc();
                    j = look(buf, TD);
                    return Lex(delims[j]);
                }
                else
                {
                    j = look(buf, TD);
                    return Lex(delims[j]);
                }
                break;
                
            case LESS:
                if (c == '/')
                {
                    add();
                    gc();
                    curr_state = OUT;
                    in_script_tag = 2;
                }
                else
                {
                    curr_state = COMPARE;
                }
                break;
                
            case DELIM:
                if (strchr((char *)"*%.,;[]{}()", c))
                {
                    clear();
                    add();
                    gc();
                    j = look(buf, TD);
                    return Lex(delims[j]);
                }
                else if (c == '/')
                {
                    clear();
                    add();
                    gc();
                    curr_state = COMM;
                }
                else
                    throw string("Lex error: unexpected symbol ") + c + string("\n");
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
                    return Lex(delims[j]);
                }
                break;
                
            case COMM2:
                if (c == EOF)
                    throw string("Lex error: close comment\n");
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
                    return Lex(delims[j]);
                }
                else
                {
                    j = look(buf, TD);
                    return Lex(delims[j]);
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
                    throw string("Lex error: close \"\n");
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
                    throw string("Lex error: close \"\n");
                else
                {
                    if (c == 'n')
                        c = '\n';
                    else if (c == 't')
                        c = '\t';
                    else if (c == 'r')
                        c = '\r';
                    else if (c == '0')
                        c = '\0';
                    else if (c == 'a')
                        c = '\a';
                    else if (c == 'b')
                        c = '\b';
                    else if (c == 'f')
                        c = '\f';
                    else if (c == 'v')
                        c = '\v';
                       
                    add();
                    gc();
                    curr_state = STR1;
                }
                break;
        }
    } while (true);
}

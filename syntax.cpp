//
//  syntax.cpp
//
//
//  Created by sSTIs on 01.05.16.
//
//

#include "syntax.h"

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
    else throw "Stack_is_full";
}

template <class T, int max_size >
T Stack <T, max_size >::pop() {
    if ( !is_empty() )
    {
        --top;
        return buf[top];
    }
    else throw "Stack_is_empty";
}

extern Table_ident TID;

//! class Parser - semantic
//!======================================================================================
void Parser::eq_bool()
{
    type_of_lex res_expression;
    res_expression = stack_type.pop();
    if (res_expression != LEX_BOOL && res_expression != LEX_NUM)
        throw string("expression is not boolean");
}

void Parser::check_op()
{
    type_of_lex operand1, operatr, operand2;
    operand1 = stack_type.pop();
    operatr = stack_type.pop();
    operand2 = stack_type.pop();
    
    
}

//! class Parser - main functions
//!======================================================================================
void Parser::analyze()
{
    get_lex();
    Program();
    //program.print();
    cout << endl << "Syntax Yes" << endl;
}

void Parser::get_lex()
{
    curr_lex = scanner.get_lex();
    curr_lex_type = curr_lex.get_type();
}

Parser::Parser(const char * program) :scanner(program)
{
    //nothing else
}
//! class Parser - recursive descent
//!======================================================================================
void Parser::Program()
{
    Program1();
    if (curr_lex_type != LEX_FIN)
        throw curr_lex;
}

void Parser::Program1()
{
    if (curr_lex_type == LEX_BEGIN)
    {
        get_lex();
        while (curr_lex_type != LEX_END && curr_lex_type != LEX_FIN)
        {
            Command();
        }
        if (curr_lex_type == LEX_END)
        {
            get_lex();
        }
        else throw curr_lex;
    }
    else throw curr_lex;
}

void Parser::Command()
{
    if (curr_lex_type == LEX_IF)
    {
        get_lex();
        if (curr_lex_type == LEX_LPAREN)
        {
            get_lex();
            Expression();
            eq_bool();
            //pl2 = prog.get_free();
            //prog.blank();
            //prog.put_lex(Lex(POLIZ_FGO);
            if (curr_lex_type == LEX_RPAREN)
            {
                get_lex();
                Command();
                //prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl2);
                if (curr_lex_type == LEX_ELSE)
                {
                    get_lex();
                    Command();
                    //pl3 = prog.get_free();
                    //prog.blank();
                    //prog.put_lex(Lex(POLIZ_GO));
                    //prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl2);
                }
            }
            else throw curr_lex;
        }
        else throw curr_lex;
    }
    else if (curr_lex_type == LEX_WHILE)
    {
        //pl0 = prog.get_free();
        get_lex();
        if (curr_lex_type == LEX_LPAREN)
        {
            Expression();
            eq_bool();
            //pl1 = prog.get_free();
            //prog.blank();
            //prog.put_lex(Lex(POLIZ_FGO));
            if (curr_lex_type == LEX_RPAREN)
            {
                get_lex();
                Command();
                //prog.put_lex(Lex(POLIZ_LABEL, pl0));
                //prog.put_lex(Lex(POLIZ_GO));
                //prog.put_lex(Lex(POLIZ_LABEL, prog.get_free(), pl1);
            }
        }
        else throw curr_lex;
    }
    else if (curr_lex_type == LEX_DO)
    {
        get_lex();
        Command();
        if (curr_lex_type == LEX_WHILE)
        {
            get_lex();
            if (curr_lex_type == LEX_LPAREN)
            {
                get_lex();
                Expression();
                eq_bool();
                if (curr_lex_type == LEX_RPAREN)
                {
                    get_lex();
                    if (curr_lex_type == LEX_SEMICOLON)
                    {
                        get_lex();
                    }
                    else throw curr_lex;
                }
                else throw curr_lex;
            }
            else throw curr_lex;
        }
        else throw curr_lex;
    }
    else if (curr_lex_type == LEX_WRITE)
    {
        get_lex();
        if (curr_lex_type == LEX_LPAREN)
        {
            get_lex();
            Expression();
            stack_type.pop();
            while (curr_lex_type == ',')
            {
                get_lex();
                Expression();
                stack_type.pop();
            }
            if (curr_lex_type == LEX_RPAREN)
            {
                get_lex();
                //prog.put_lex(LEX_WRITE));
                if (curr_lex_type == LEX_SEMICOLON)
                {
                    get_lex();
                }
                else throw curr_lex;
            }
            else throw curr_lex;
        }
        else throw curr_lex;
    }
    else if (curr_lex_type == LEX_ID)
    {
        int num_in_TID = curr_lex.get_ivalue();
        get_lex();
        if (curr_lex_type == LEX_ASSIGN)
        {
            get_lex();
            Expression();
            TID[num_in_TID].put_type(stack_type.pop());
            if (curr_lex_type == LEX_SEMICOLON)
            {
                get_lex();
            }
            else throw curr_lex;
        }
        else throw curr_lex;
    }
    else if (curr_lex_type == LEX_ID || curr_lex_type == LEX_STR ||
             curr_lex_type == LEX_NUM || curr_lex_type == LEX_BOOL)
    {
        Expression();
        stack_type.pop();
        if (curr_lex_type == LEX_SEMICOLON)
        {
            get_lex();
        }
        else throw curr_lex;
    }
    else if (curr_lex_type == LEX_VAR)
    {
        get_lex();
        do
        {
            Declaration();
        } while (curr_lex_type == LEX_COMMA && curr_lex_type != LEX_FIN);
        if (curr_lex_type == LEX_SEMICOLON)
        {
            get_lex();
        }
        else throw curr_lex;
    }
    else if (curr_lex_type == LEX_GETENV)
    {
        get_lex();
        if (curr_lex_type == LEX_LPAREN_SQ)
        {
            get_lex();
            if (curr_lex_type == LEX_STR)
            {
                //???
                get_lex();
                if (curr_lex_type == LEX_RPAREN_SQ)
                {
                    get_lex();
                    //prog.put_lex(Lex(LEX_GETENV));???
                    if (curr_lex_type == LEX_SEMICOLON)
                    {
                        get_lex();
                    }
                    else throw curr_lex;
                }
                else throw curr_lex;
            }
            else throw curr_lex;
        }
        else throw curr_lex;
    }
    else Program1();
}

void Parser::Declaration()
{
    if (curr_lex_type == LEX_ID)
    {
        int num_in_TID = curr_lex.get_ivalue();
        get_lex();
        if (curr_lex_type == LEX_ASSIGN)
        {
            get_lex();
            if (curr_lex_type == LEX_ID || curr_lex_type == LEX_STR ||
                curr_lex_type == LEX_NUM || curr_lex_type == LEX_BOOL)
            {
                get_lex();
            }
            else throw curr_lex;
        }
        else throw curr_lex;
    }
    else throw curr_lex;
}

void Parser::Expression()
{
    Expression1();
    if (curr_lex_type == LEX_OR)
    {
        stack_type.push(curr_lex_type);
        get_lex();
        Expression1();
        check_op();
    }
}

void Parser::Expression1()
{
    Expression2();
    if (curr_lex_type == LEX_AND)
    {
        stack_type.push(curr_lex_type);
        get_lex();
        Expression2();
        check_op();
    }
}

void Parser::Expression2()
{
    Expression3();
    if (curr_lex_type == LEX_EQ || curr_lex_type == LEX_NEQ)
    {
        stack_type.push(curr_lex_type);
        get_lex();
        Expression3();
        check_op();
    }
}

void Parser::Expression3()
{
    Summand();
    if (curr_lex_type == LEX_ADD || curr_lex_type == LEX_SUB)
    {
        stack_type.push(curr_lex_type);
        get_lex();
        Summand();
        check_op();
    }
}

void Parser::Summand()
{
    Factor();
    if (curr_lex_type == LEX_MUL || curr_lex_type == LEX_DIV
        || curr_lex_type == LEX_PERCENT)
    {
        stack_type.push(curr_lex_type);
        get_lex();
        Factor();
        check_op();
    }
}

void Parser::Factor()
{
    if (curr_lex_type == LEX_LPAREN)
    {
        get_lex();
        Expression();
        if (curr_lex_type == LEX_RPAREN)
        {
            get_lex();
        }
        else throw curr_lex;
    }
    else if (curr_lex_type == LEX_ID)
    {
        //check_id();
        stack_type.push((TID[curr_lex.get_ivalue()]).get_type());
        //prog.put_lex(curr_lex);
        get_lex();
    }
    else if (curr_lex_type == LEX_NUM || curr_lex_type == LEX_BOOL ||
             curr_lex_type == LEX_STR)
    {
        stack_type.push(curr_lex_type);
        //prog.put_lex(curr_lex);
        get_lex();
    }
    else if (curr_lex_type == LEX_INC || curr_lex_type == LEX_DEC)
    {
        stack_type.push(LEX_NUM);
        if (curr_lex_type == LEX_INC)
            stack_type.push(LEX_ADD);
        else
            stack_type.push(LEX_SUB);
        get_lex();
        Factor();
        check_op();
    }
    else if (curr_lex_type == LEX_NOT)
    {
        get_lex();
        Factor();
        //check_not();
    }
    else if (curr_lex_type == LEX_SUB)
    {
        stack_type.push(LEX_NUM);
        stack_type.push(LEX_SUB);
        get_lex();
        if (curr_lex_type == LEX_NUM || curr_lex_type == LEX_BOOL ||
            curr_lex_type == LEX_STR)
        {
            stack_type.push(curr_lex_type);
            check_op();
            get_lex();
        }
    }
    else throw curr_lex;
}

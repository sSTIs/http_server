//
//  syntax.cpp
//
//
//  Created by sSTIs on 01.05.16.
//
//

#include <stack>
#include "syntax.h"

using namespace std;

//! class Parser - semantic
//!======================================================================================

//! class Parser - main functions
//!======================================================================================
void Parser::analyze()
{
    get_lex();
    Program();
    //program.print();
    cout << endl; << "Yes" << endl;
}

void Parse::get_lex()
{
    curr_lex = scanner.get_lex();
    curr_lex_type = curr_lex.get_type();
}

Parser::Parser : scanner(program) {}
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
        Command();
        while (curr_lex_type == LEX_SEMICOLON)
        {
            get_lex();
            Command();
        }
        if (curr_lex_type == LEX_END)
            get_lex();
        else throw curr_lex;
    }
    else throw curr_lex;
}

void Parser::Command()
{
    if (curr_lex_type == LEX_IF)
    {
        get_lex();
        Expression();
        //eq_bool();
        //pl2 = prog.get_free();
        //prog.blank();
        //prog.put_lex(Lex(POLIZ_FGO);
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
        else throw curr_lex;
    }
    else if (curr_lex_type == LEX_WHILE)
    {
        //pl0 = prog.get_free();
        get_lex();
        Expression();
        //eq_bool();
        //pl1 = prog.get_free();
        //prog.blank();
        //prog.put_lex(Lex(POLIZ_FGO));
        Command();
        //prog.put_lex(Lex(POLIZ_LABEL, pl0));
        //prog.put_lex(Lex(POLIZ_GO));
        //prog.put_lex(Lex(POLIZ_LABEL, prog.get_free(), pl1);
    }
    else if (curr_lex_type == LEX_DO)
    {
        
    }
    else if (curr_lex_type == LEX_READ)
    {
        get_lex();
        if (curr_lex_type == LEX_LPAREN)
        {
            if (curr_lex_type == LEX_ID)
            {
                //check_id_in_read();
                //prog.put_lex(Lex(POLIZ_ADDRESS, val));???
                get_lex();
            }
            else throw curr_lex;
            if (curr_lex_type == LEX_RPAREN)
            {
                get_lex();
                //prog.put_lex(Lex(LEX_READ));
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
            if (curr_lex_type == LEX_RPAREN)
            {
                get_lex();
                //prog.put_lex(LEX_WRITE));
            }
            else throw curr_lex;
        }
        else throw curr_lex;
    }
    else if (curr_lex_type == LEX_ID)
    {
        
    }
    else if (curr_lex_type == LEX_VAR)
    {
        
    }
    else if (curr_lex_type == LEX_GETENV)
    {
        get_lex();
        if (curr_lex_type == LEX_LPAREN)
        {
            if (curr_lex_type == LEX_STRING)
            {
                //???
                get_lex();
            }
            else throw curr_lex;
            if (curr_lex_type == LEX_RPAREN)
            {
                get_lex();
                //prog.put_lex(Lex(LEX_GETENV));???
            }
            else throw curr_lex;
        }
        else throw curr_lex;

    }
    else Program1();
}

void Parser::Expression()
{
    Expression1();
    if (curr_lex_type == LEX_OR)
    {
        stack_type.push(curr_lex_type);
        get_lex();
        Expression1();
        //check_op();
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
        //check_op();
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
        //check_op();
    }
}

void Expression3()
{
    Summand();
    if (curr_lex_type == LEX_ADD || curr_lex_type == LEX_SUB)
    {
        stack_type.push(curr_lex_type);
        get_lex();
        Summand();
        //check_op();
    }
}

void Summand()
{
    Factor();
    if (curr_lex_type == LEX_MUL || curr_lex_type == LEX_DIV || curr_lex_type == LEX_PERC)
    {
        stack_type.push(curr_lex_type);
        get_lex();
        Factor();
        //check_op();
    }
}

void Factor()
{
    if (curr_lex_type == LEX_LPAREN)
    {
        get_lex();
        Expression();
        if (curr_lex_type != LEX_RPAREN)
            throw curr_lex;
    }
    else if (curr_lex_type == LEX_ID)
    {
        //check_id();
        //prog.put_lex(curr_lex);
        get_lex();
    }
    else if (curr_lex_type == LEX_NUM || curr_lex_type == LEX_BOOL ||
             curr_lex_type == LEX_STRING)
    {
        //stack_type.push(curr_lex_type);
        //prog.put_lex(curr_lex);
        get_lex();
    }
    else if (curr_lex_type == LEX_INC || curr_lex_type == LEX_DEC)
    {
        //???
        get_lex();
        Factor();
    }
    else if (curr_lex_type == LEX_NOT)
    {
        get_lex();
        Factor();
        //check_not();
    }
    else throw curr_lex;
}


















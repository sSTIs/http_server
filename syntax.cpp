//
//  syntax.cpp
//
//
//  Created by sSTIs on 01.05.16.
//
//

#include "interpretator.h"

using namespace std;

extern Table_ident TID;

//! class Parser - semantic
//!======================================================================================
void Parser::declare(int num_in_TID)
{
    if (TID[num_in_TID].get_declare())
        throw "Semantic error: " + string(TID[num_in_TID].get_name()) +" declared twice";
    else
        TID[num_in_TID].put_declare();
}

void Parser::check_declare(int num_in_TID)
{
    if (!(TID[num_in_TID].get_declare()))
        throw "Semantic error: " + string(TID[num_in_TID].get_name()) +" not declared";
    else
        TID[num_in_TID].put_declare();
}

//! class Parser - main functions
//!======================================================================================
void Parser::analyze()
{
    get_lex();
    Program();
    program_poliz.print();
    cout << endl << "Syntax Yes" << endl;
}

void Parser::get_lex()
{
    curr_lex = scanner.get_lex();
    curr_lex_type = curr_lex.get_type();
}

Parser::Parser(const char * program) :scanner(program), program_poliz(1000)
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
            int fgo_label = program_poliz.get_free();
            program_poliz.put_free_space();
            program_poliz.put_lex(Lex(POLIZ_FGO));
            if (curr_lex_type == LEX_RPAREN)
            {
                get_lex();
                Command();
                if (curr_lex_type == LEX_ELSE)
                {
                    int else_lable = program_poliz.get_free();
                    program_poliz.put_free_space();
                    program_poliz.put_lex(Lex(POLIZ_GO));
                    program_poliz.put_lex(Lex(POLIZ_LABEL, program_poliz.get_free()), fgo_label);
                    get_lex();
                    Command();
                    program_poliz.put_lex(Lex(POLIZ_LABEL, program_poliz.get_free()), else_lable);
                }
                else
                    program_poliz.put_lex(Lex(POLIZ_LABEL, program_poliz.get_free()), fgo_label);
            }
            else throw curr_lex;
        }
        else throw curr_lex;
    }
    else if (curr_lex_type == LEX_WHILE)
    {
        get_lex();
        if (curr_lex_type == LEX_LPAREN)
        {
            get_lex();
            int while_label = program_poliz.get_free();
            Expression();
            int cycle_lable = program_poliz.get_free();
            program_poliz.put_free_space();
            program_poliz.put_lex(Lex(POLIZ_FGO));
            
            if (curr_lex_type == LEX_RPAREN)
            {
                get_lex();
                Command();
                program_poliz.put_lex(Lex(POLIZ_LABEL, while_label));
                program_poliz.put_lex(Lex(POLIZ_GO));
                program_poliz.put_lex(Lex(POLIZ_LABEL, program_poliz.get_free()), cycle_lable);
            }
            else throw curr_lex;
        }
        else throw curr_lex;
    }
    else if (curr_lex_type == LEX_DO)
    {
        get_lex();
        int cycle_lable = program_poliz.get_free();
        Command();
        if (curr_lex_type == LEX_WHILE)
        {
            get_lex();
            if (curr_lex_type == LEX_LPAREN)
            {
                get_lex();
                Expression();
                if (curr_lex_type == LEX_RPAREN)
                {
                    get_lex();
                    program_poliz.put_lex(Lex(POLIZ_LABEL, cycle_lable));
                    program_poliz.put_lex(Lex(POLIZ_FGO));
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
            program_poliz.put_lex(Lex(LEX_WRITE));
            //stack_type.pop();
            while (curr_lex_type == LEX_COMMA)
            {
                get_lex();
                Expression();
                program_poliz.put_lex(Lex(LEX_WRITE));
                //stack_type.pop();
            }
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
    else if (curr_lex_type == LEX_ID)
    {
        int num_in_TID = curr_lex.get_ivalue();
        check_declare(num_in_TID);
        program_poliz.put_lex(Lex(POLIZ_ADDRESS, num_in_TID));
        get_lex();
        if (curr_lex_type == LEX_ASSIGN)
        {
            get_lex();
            Expression();
            //TID[num_in_TID].put_type(stack_type.pop());
            if (curr_lex_type == LEX_SEMICOLON)
            {
                get_lex();
                program_poliz.put_lex(Lex(LEX_ASSIGN));
            }
            else throw curr_lex;
        }
        else throw curr_lex;
    }
    else if (curr_lex_type == LEX_STR ||
             curr_lex_type == LEX_NUM || curr_lex_type == LEX_BOOL)
    {
        Expression();
        //stack_type.pop();
        if (curr_lex_type == LEX_SEMICOLON)
        {
            program_poliz.put_lex(Lex(LEX_WRITE));
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
    else if (curr_lex_type == LEX_SEMICOLON)
    {
        get_lex();
    }
    else Program1();
}

void Parser::Declaration()
{
    if (curr_lex_type == LEX_ID)
    {
        int num_in_TID = curr_lex.get_ivalue();
        declare(num_in_TID);
        program_poliz.put_lex(Lex(POLIZ_ADDRESS, num_in_TID));
        get_lex();
        if (curr_lex_type == LEX_ASSIGN)
        {
            get_lex();
            Expression();
            program_poliz.put_lex(Lex(LEX_ASSIGN));
        }
        else throw curr_lex;
    }
    else throw curr_lex;
}

void Parser::Expression()
{
    Expression1();
    while (curr_lex_type == LEX_OR)
    {
        //stack_type.push(curr_lex_type);
        get_lex();
        Expression1();
        program_poliz.put_lex(Lex(LEX_OR));
    }
}

void Parser::Expression1()
{
    Expression2();
    while (curr_lex_type == LEX_AND)
    {
        //stack_type.push(curr_lex_type);
        get_lex();
        Expression2();
        program_poliz.put_lex(Lex(LEX_AND));
    }
}

void Parser::Expression2()
{
    Expression3();
    while (curr_lex_type == LEX_EQ || curr_lex_type == LEX_NEQ)
    {
        type_of_lex op = curr_lex_type;
        //stack_type.push(curr_lex_type);
        get_lex();
        Expression3();
        program_poliz.put_lex(Lex(op));
    }
}

void Parser::Expression3()
{
    Summand();
    while (curr_lex_type == LEX_ADD || curr_lex_type == LEX_SUB)
    {
        type_of_lex op = curr_lex_type;
        //stack_type.push(curr_lex_type);
        get_lex();
        Summand();
        program_poliz.put_lex(Lex(op));
    }
}

void Parser::Summand()
{
    Factor();
    while (curr_lex_type == LEX_MUL || curr_lex_type == LEX_DIV
        || curr_lex_type == LEX_PERCENT)
    {
        type_of_lex op = curr_lex_type;
        //stack_type.push(curr_lex_type);
        get_lex();
        Factor();
        program_poliz.put_lex(Lex(op));
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
        //stack_type.push((TID[curr_lex.get_ivalue()]).get_type());
        check_declare(curr_lex.get_ivalue());
        program_poliz.put_lex(curr_lex);
        get_lex();
    }
    else if (curr_lex_type == LEX_NUM || curr_lex_type == LEX_BOOL ||
             curr_lex_type == LEX_STR)
    {
        //stack_type.push(curr_lex_type);
        program_poliz.put_lex(curr_lex);
        get_lex();
    }
    else if (curr_lex_type == LEX_INC || curr_lex_type == LEX_DEC)
    {
        type_of_lex op = curr_lex_type;
        /*stack_type.push(LEX_NUM);
        if (curr_lex_type == LEX_INC)
            stack_type.push(LEX_ADD);
        else
            stack_type.push(LEX_SUB);*/
        get_lex();
        Factor();
        program_poliz.put_lex(Lex(LEX_NUM,1));
        program_poliz.put_lex(Lex(op));
    }
    else if (curr_lex_type == LEX_NOT)
    {
        get_lex();
        Factor();
        program_poliz.put_lex(Lex(LEX_NOT));
    }
    else if (curr_lex_type == LEX_SUB)
    {
        get_lex();
        if (curr_lex_type == LEX_NUM || curr_lex_type == LEX_BOOL ||
            curr_lex_type == LEX_STR)
        {
            //stack_type.push(curr_lex_type);
            //stack_type.push(LEX_SUB);
            //stack_type.push(LEX_NUM);
            get_lex();
            program_poliz.put_lex(Lex(LEX_NUM, 0));
            Factor();
            program_poliz.put_lex(Lex(LEX_SUB));
        }
    }
    else if (curr_lex_type == LEX_ENV)
    {
        get_lex();
        if (curr_lex_type == LEX_LPAREN_SQ)
        {
            get_lex();
            if (curr_lex_type == LEX_STR)
            {
                program_poliz.put_lex(curr_lex);
                get_lex();
                if (curr_lex_type == LEX_RPAREN_SQ)
                {
                    get_lex();
                    program_poliz.put_lex(Lex(LEX_ENV));
                }
                else throw curr_lex;
            }
            else throw curr_lex;
        }
        else throw curr_lex;
    }
    else throw curr_lex;
}

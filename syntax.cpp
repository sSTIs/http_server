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

//! class Parser - semantic
//!======================================================================================
void Parser::declare(int num_in_TID)
{
    if (TID[num_in_TID].type_of_value == 5)
        throw string("Semantic: redeclaration of function");
    if (TID[num_in_TID].declare == area_visibility)
        throw "Semantic error: " + string(TID[num_in_TID].get_name()) +" declared twice";
    else
    {
        TID[num_in_TID].prev_declare = TID[num_in_TID].declare;
        TID[num_in_TID].declare = area_visibility;
    }
}

void Parser::check_declare(int num_in_TID)
{
    if (TID[num_in_TID].declare != area_visibility)
        throw "Semantic error: " + string(TID[num_in_TID].get_name()) +" not declared";
}

void Parser::undeclare()
{
    for (int i = 0; i < TID.get_top(); ++i)
    {
        if (TID[i].declare == area_visibility)
            TID[i].declare = TID[i].prev_declare;
    }
}

//! class Parser - main functions
//!======================================================================================
bool Parser::analyze()
{
    get_lex();
    Program();
    //cout << endl << "Syntax Yes" << endl;
    return (curr_lex_type != LEX_FIN);
}

void Parser::get_lex()
{
    curr_lex = scanner.get_lex();
    curr_lex_type = curr_lex.get_type();
}

Parser::Parser(const char * program) :scanner(program), program_poliz(1000)
{
    area_visibility = 0;
    in_cycle = 0;
}
//! class Parser - recursive descent
//!======================================================================================
void Parser::Program()
{
    while (curr_lex_type != LEX_FIN_PART && curr_lex_type != LEX_FIN)
    {
        Command();
    }
    if (curr_lex_type == LEX_FIN)
        program_poliz.put_lex(Lex(LEX_FIN));
    else if (curr_lex_type == LEX_FIN_PART)
        program_poliz.put_lex(Lex(LEX_FIN_PART));
    else throw curr_lex;
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
    else if (curr_lex_type == LEX_FOR || curr_lex_type == LEX_WHILE ||
             curr_lex_type == LEX_DO)
    {
        ++in_cycle;
        Cycle();
        for (int i = cycle_begin.pop(); i < program_poliz.get_free(); ++i)
            if (program_poliz[i].get_type() == LEX_BREAK)
                program_poliz.put_lex(Lex(POLIZ_LABEL, program_poliz.get_free()), i);
        --in_cycle;
    }
    else if (curr_lex_type == LEX_BREAK)
    {
        if (in_cycle == 0)
            throw curr_lex;
        program_poliz.put_lex(Lex(LEX_BREAK));
        program_poliz.put_lex(Lex(POLIZ_GO));
        get_lex();
        if (curr_lex_type == LEX_SEMICOLON)
        {
            get_lex();
        }
        else throw curr_lex;
    }
    else if (curr_lex_type == LEX_CONTINUE)
    {
        if (in_cycle == 0)
            throw curr_lex;
        program_poliz.put_lex(Lex(POLIZ_LABEL, cycle_begin.top_element()));
        program_poliz.put_lex(Lex(POLIZ_GO));
        get_lex();
        if (curr_lex_type == LEX_SEMICOLON)
        {
            get_lex();
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
    else if (curr_lex_type == LEX_STR || curr_lex_type == LEX_ID ||
             curr_lex_type == LEX_NUM || curr_lex_type == LEX_BOOL ||
             curr_lex_type == LEX_INC || curr_lex_type == LEX_DEC ||
             curr_lex_type == LEX_NOT || curr_lex_type == LEX_SUB)
    {
        Expression();
        //stack_type.pop();
        if (curr_lex_type == LEX_SEMICOLON)
        {
            program_poliz.put_lex(Lex(LEX_SEMICOLON));
            get_lex();
        }
        else throw curr_lex;
    }
    else if (curr_lex_type == LEX_VAR)
    {
        do
        {
            get_lex();
            Declaration();
            program_poliz.put_lex(Lex(LEX_SEMICOLON));// to clear stack
        } while (curr_lex_type == LEX_COMMA);
        if (curr_lex_type == LEX_SEMICOLON)
        {
            get_lex();
        }
        else throw curr_lex;
    }
    else if (curr_lex_type == LEX_FUNC && area_visibility == 0)
    {
        get_lex();
        program_poliz.change_free(); //free to functions place
        if (curr_lex_type != LEX_ID)
            throw curr_lex;
        //if ((TID[curr_lex.get_ivalue()]).declare != -1)
            //throw string("Semantic: redeclare of variable by function\n");
        int num_in_TID_func =curr_lex.get_ivalue();
        (TID[num_in_TID_func]).ivalue = program_poliz.get_free();
        (TID[num_in_TID_func]).type_of_value = 5;
        area_visibility = curr_lex.get_ivalue();
        get_lex();
        if (curr_lex_type != LEX_LPAREN)
            throw curr_lex;
        get_lex();
        if (curr_lex_type == LEX_RPAREN)
        {
            get_lex();
        }
        else
        {
            if (curr_lex_type != LEX_ID)
                throw curr_lex;
            declare(curr_lex.get_ivalue());
            (TID[num_in_TID_func]).parametrs_number += 1;
            program_poliz.put_lex(Lex(POLIZ_ADDRESS, curr_lex.get_ivalue()));
            get_lex();
            while (curr_lex_type == LEX_COMMA)
            {
                get_lex();
                if (curr_lex_type != LEX_ID)
                    throw curr_lex;
                declare(curr_lex.get_ivalue());
                (TID[num_in_TID_func]).parametrs_number += 1;
                program_poliz.put_lex(Lex(POLIZ_ADDRESS, curr_lex.get_ivalue()));
                get_lex();
            }
            if (curr_lex_type != LEX_RPAREN)
                throw curr_lex;
            get_lex();
        }
        program_poliz.put_lex(Lex(LEX_FUNC));
        Command();
        program_poliz.put_lex(Lex(LEX_UNDEF));
        program_poliz.put_lex(Lex(LEX_RETURN));
        program_poliz.change_free(); // free to main place
        undeclare();
        area_visibility = 0;
    }
    else if (curr_lex_type == LEX_FUNC && area_visibility != 0)
        throw curr_lex;
    else if (curr_lex_type == LEX_RETURN && area_visibility != 0)
    {
        get_lex();
        if (curr_lex_type == LEX_SEMICOLON)
            program_poliz.put_lex(Lex(LEX_UNDEF));
        else
            Expression();
        program_poliz.put_lex(Lex(LEX_RETURN));
        if (curr_lex_type == LEX_SEMICOLON)
        {
            get_lex();
        }
        else throw curr_lex;
    }
    else if (curr_lex_type == LEX_RETURN && area_visibility == 0)
        throw curr_lex;
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
            program_poliz.put_lex(Lex(LEX_VAR));
        }
    }
    else throw curr_lex;
}

void Parser::Cycle()
{
    if (curr_lex_type == LEX_FOR)
    {
        int e2_label, e3_label, check_label, to_command_label, command_label;
        get_lex();
        if (curr_lex_type != LEX_LPAREN)
            throw curr_lex;
        get_lex();
        if (curr_lex_type != LEX_SEMICOLON)
        {
            Expression();
            program_poliz.put_lex(Lex(LEX_SEMICOLON));
        }
        if (curr_lex_type != LEX_SEMICOLON)
            throw curr_lex;
        get_lex();
        e2_label = program_poliz.get_free();
        if (curr_lex_type != LEX_SEMICOLON)
        {
            Expression();
        }
        else
            program_poliz.put_lex(Lex(LEX_BOOL, true));
        if (curr_lex_type != LEX_SEMICOLON)
            throw curr_lex;
        check_label = program_poliz.get_free();
        program_poliz.put_free_space();
        program_poliz.put_lex(Lex(POLIZ_FGO));
        to_command_label = program_poliz.get_free();
        program_poliz.put_free_space();
        program_poliz.put_lex(Lex(POLIZ_GO));
        get_lex();
        
        e3_label = program_poliz.get_free();
        cycle_begin.push(e3_label);
        if (curr_lex_type != LEX_RPAREN)
        {
            Expression();
            program_poliz.put_lex(Lex(LEX_SEMICOLON));
        }
        if (curr_lex_type != LEX_RPAREN)
            throw curr_lex;
        program_poliz.put_lex(Lex(POLIZ_LABEL, e2_label));
        program_poliz.put_lex(Lex(POLIZ_GO));
        get_lex();
        
        command_label = program_poliz.get_free();
        Command();
        program_poliz.put_lex(Lex(POLIZ_LABEL, e3_label));
        program_poliz.put_lex(Lex(POLIZ_GO));
        program_poliz.put_lex(Lex(POLIZ_LABEL, program_poliz.get_free()), check_label);
        program_poliz.put_lex(Lex(POLIZ_LABEL, command_label), to_command_label);
    }
    else if (curr_lex_type == LEX_WHILE)
    {
        get_lex();
        if (curr_lex_type == LEX_LPAREN)
        {
            get_lex();
            int while_label = program_poliz.get_free();
            cycle_begin.push(while_label);
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
    else if (curr_lex_type == LEX_DO)// no
    {
        get_lex();
        int cycle_lable = program_poliz.get_free();
        cycle_begin.push(cycle_lable);
        Command();
        if (curr_lex_type == LEX_WHILE)
        {
            get_lex();
            if (curr_lex_type == LEX_LPAREN)
            {
                get_lex();
                Expression();
                program_poliz.put_lex(Lex(LEX_NOT));
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
}
//! expression
//!======================================================================================
void Parser::Expression()
{
    Expression1();
    while (curr_lex_type == LEX_ASSIGN)
    {
        //stack_type.push(curr_lex_type);
        if ((program_poliz[program_poliz.get_free() - 1]).get_type() != POLIZ_ADDRESS)
            throw curr_lex;
        get_lex();
        Expression1();
        program_poliz.put_lex(Lex(LEX_ASSIGN));
    }
}
void Parser::Expression1()
{
    Expression2();
    while (curr_lex_type == LEX_OR)
    {
        //stack_type.push(curr_lex_type);
        get_lex();
        Expression2();
        program_poliz.put_lex(Lex(LEX_OR));
    }
}

void Parser::Expression2()
{
    Expression3();
    while (curr_lex_type == LEX_AND)
    {
        //stack_type.push(curr_lex_type);
        get_lex();
        Expression3();
        program_poliz.put_lex(Lex(LEX_AND));
    }
}

void Parser::Expression3()
{
    Expression4();
    while (curr_lex_type == LEX_GE || curr_lex_type == LEX_G ||
           curr_lex_type == LEX_LE || curr_lex_type == LEX_L)
    {
        type_of_lex op = curr_lex_type;
        //stack_type.push(curr_lex_type);
        get_lex();
        Expression4();
        program_poliz.put_lex(Lex(op));
    }
}

void Parser::Expression4()
{
    Expression5();
    while (curr_lex_type == LEX_EQ || curr_lex_type == LEX_NEQ)
    {
        type_of_lex op = curr_lex_type;
        //stack_type.push(curr_lex_type);
        get_lex();
        Expression5();
        program_poliz.put_lex(Lex(op));
    }
}

void Parser::Expression5()
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
        if (TID[curr_lex.get_ivalue()].type_of_value != 5)
        {
            //stack_type.push((TID[curr_lex.get_ivalue()]).get_type());
            check_declare(curr_lex.get_ivalue());
            program_poliz.put_lex(Lex(POLIZ_ADDRESS, curr_lex.get_ivalue()));
            get_lex();
            if (curr_lex_type == LEX_INC)
            {
                program_poliz.put_lex(Lex(LEX_INC));
                get_lex();
            }
            else if (curr_lex_type == LEX_DEC)
            {
                program_poliz.put_lex(Lex(LEX_DEC));
                get_lex();
            }
        }
        else
        {
            int function_address = TID[curr_lex.get_ivalue()].ivalue;
            int parametrs_number = TID[curr_lex.get_ivalue()].parametrs_number;
            int curr_parametrs_number = 0;
            get_lex();
            if (curr_lex_type != LEX_LPAREN)
                throw curr_lex;
            get_lex();
            if (curr_lex_type == LEX_RPAREN)
            {
                get_lex();
            }
            else
            {
                Expression();
                ++curr_parametrs_number;
                while (curr_lex_type == LEX_COMMA)
                {
                    get_lex();
                    Expression();
                    ++ curr_parametrs_number;
                }
                if (curr_lex_type != LEX_RPAREN)
                    throw curr_lex;
                get_lex();
            }
            if (curr_parametrs_number != parametrs_number)
                throw string("Semantic: wrong number of parametrs");
            program_poliz.put_lex(Lex(POLIZ_LABEL, program_poliz.get_free() + 3));
            program_poliz.put_lex(Lex(LEX_NUM, parametrs_number));//number of parametrs
            program_poliz.put_lex(Lex(POLIZ_CALL, function_address));
        }
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
        if (curr_lex_type == LEX_ID)
        {
            if (TID[curr_lex.get_ivalue()].type_of_value != 5)
            {
                program_poliz.put_lex(Lex(POLIZ_ADDRESS, curr_lex.get_ivalue()));
                program_poliz.put_lex(Lex(POLIZ_ADDRESS, curr_lex.get_ivalue()));
                program_poliz.put_lex(Lex(LEX_NUM,1));
                if (op == LEX_INC)
                    program_poliz.put_lex(Lex(LEX_ADD));
                else
                    program_poliz.put_lex(Lex(LEX_SUB));
                program_poliz.put_lex(Lex(LEX_ASSIGN));
                get_lex();
            }
            else throw curr_lex;
        }
        else throw curr_lex;
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
            curr_lex_type == LEX_STR || curr_lex_type == LEX_ID)
        {
            //stack_type.push(curr_lex_type);
            //stack_type.push(LEX_SUB);
            //stack_type.push(LEX_NUM);
            program_poliz.put_lex(Lex(LEX_NUM, 0));
            Factor();
            program_poliz.put_lex(Lex(LEX_SUB));
        }
        else
        {
            program_poliz.put_lex(Lex(LEX_NUM, 0));
            Expression();
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

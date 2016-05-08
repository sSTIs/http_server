//
//  executer.cpp
//
//
//  Created by sSTIs on 08.05.16.
//
//

#include "interpretator.h"
#include <stdlib.h>

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

//! class Poliz
//!======================================================================================
Lex& Poliz::operator[] (int index)
{
    if (index > size)
        throw "POLIZ:out of array";
    else
    {
        if ( index > free )
            throw "POLIZ: indefinite element of array";
        else return buf[index];
    }
}

void Poliz::print()
{
    for (int i = 0; i < free; ++i)
        cout << i << " " << buf[i];
}

extern Table_ident TID;

//! types conversion
//!======================================================================================
bool toBool(Lex lexem)
{
    switch (lexem.get_type_of_value()) {
        case 0:
        case 4:
            return false;
            break;
        case 1:
            return (lexem.get_ivalue() != 0);
            break;
        case 2:
            return lexem.get_bvalue();
            break;
        case 3:
            return (lexem.get_svalue() != string(""));
            break;
            
        default:
            return true;
            break;
    }
}

//! class Executer
//!======================================================================================
void Executer::execute(Poliz& poliz)
{
    Stack <Lex, 1000> args;
    int index = 0;
    int size = poliz.get_free();
    int index_in_TID;
    Lex check, arg, to, from;
    char temp_str[100] = "";
    
    while (index < size)
    {
        curr_lex = poliz[index];
        switch (curr_lex.get_type())
        {
            case LEX_UNDEF:
            case LEX_NUM:
            case LEX_BOOL:
            case LEX_STR:
            case POLIZ_LABEL:
            case POLIZ_ADDRESS:
                args.push(curr_lex);
                break;
            case LEX_ID:
                index_in_TID = curr_lex.get_ivalue();
                switch (TID[index_in_TID].type_of_value)
                {
                case 0:
                    args.push(Lex(LEX_UNDEF));
                    break;
                case 1:
                    args.push(Lex(LEX_NUM, TID[index_in_TID].ivalue));
                    break;
                case 2:
                    args.push(Lex(LEX_BOOL, TID[index_in_TID].bvalue));
                    break;
                case 3:
                    args.push(Lex(LEX_STR, TID[index_in_TID].svalue));
                    break;
                }
                break;
            case POLIZ_GO:
                index = (args.pop()).get_ivalue() - 1;
                break;
            case POLIZ_FGO:
                check = args.pop();
                if (!toBool(args.pop()))
                    index = check.get_ivalue() - 1;
                break;
            case LEX_WRITE:
                arg = args.pop();
                switch (arg.get_type_of_value())
                 {
                 case 0:
                    cout << "NaN";
                    break;
                 case 1:
                    cout << arg.get_ivalue();
                    break;
                 case 2:
                    if (arg.get_bvalue())
                        cout << "true";
                    else
                        cout << "false";
                    break;
                 case 3:
                    cout << arg.get_svalue();
                    break;
                 }
                break;
            case LEX_ASSIGN:
                from = args.pop();
                to = args.pop();
                index_in_TID = to.get_ivalue();
                (TID[index_in_TID]).type_of_value = from.get_type_of_value();
                switch (from.get_type_of_value())
                {
                case 1:
                        (TID[index_in_TID]).ivalue = from.get_ivalue();
                        break;
                case 2:
                        (TID[index_in_TID]).bvalue = from.get_bvalue();
                        break;
                case 3:
                        (TID[index_in_TID]).svalue = from.get_svalue();
                        break;
                }
                break;
            case LEX_ENV:
                strcpy(temp_str, ((args.pop()).get_svalue()).c_str());
                args.push(Lex(LEX_STR, string(getenv(temp_str))));
                break;
            default:
                cout << curr_lex;
                throw string("Executer: Unexpected Poliz Lexem");
        }
        ++index;
    }
    cout << "Execute has finished"<<endl;
}

//! class Executer
//!======================================================================================
void Interpretator::interpretation()
{
    parser.analyze();
    executer.execute(parser.program_poliz);
}

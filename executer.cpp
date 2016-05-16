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
        throw string("POLIZ:out of array");
    else
    {
        if ( index > free )
            throw string("POLIZ: indefinite element of array");
        else return buf[index];
    }
}

void Poliz::print() // works if free on main
{
    for (int i = 0; i < free; ++i)
        cout << i << " " << buf[i];
    cout <<endl<< "functions" <<endl;
    change_free();
    for (int i = size / 2; i < free; ++i)
        cout << i << " " << buf[i];
    change_free();
}

void Poliz::change_free()
{
    int temp = free;
    free = second_free;
    second_free = temp;
}

Poliz::Poliz(int max_size)
{
    buf = new Lex [size = max_size];
    free = 0;
    second_free = max_size / 2;
}

extern Table_ident TID;

//! Executer types conversion
//!=====================================================================================
void Executer::from_adreess_to_value(Lex& lexem)
{
    if (lexem.get_type() == POLIZ_ADDRESS)
    {
        int index_in_TID = lexem.get_ivalue();
        if (area_visibility != 0)
        {
            index_in_TID = TID.put(TID[index_in_TID].get_name(), area_visibility);
        }
        switch (TID[index_in_TID].type_of_value)
        {
            case 0:
                lexem = Lex(LEX_UNDEF);
                break;
            case 1:
                lexem = Lex(LEX_NUM, TID[index_in_TID].ivalue);
                break;
            case 2:
                lexem = Lex(LEX_BOOL, TID[index_in_TID].bvalue);
                break;
            case 3:
                lexem = Lex(LEX_STR, TID[index_in_TID].svalue);
                break;
        }
    }
}

bool Executer::toBool(Lex lexem)
{
    from_adreess_to_value(lexem);
    switch (lexem.get_type_of_value())
    {
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

int Executer::toInt(Lex lexem)
{
    from_adreess_to_value(lexem);
    int num_from_str;
    switch (lexem.get_type_of_value())
    {
    case 1:
        return (lexem.get_ivalue());
        break;
    case 2:
        return (lexem.get_bvalue() != false);
        break;
    case 3:
        return 0;
        //return (strtol((lexem.get_svalue()).c_str()),NULL,10);//no
        break;
    case 4:
        return 0;
        break;
    default:
        return 0;//no
        break;
    }
}

string Executer::toString(Lex lexem)
{
    from_adreess_to_value(lexem);
    char temp_str[100];
    switch (lexem.get_type_of_value())
    {
        case 0:
            return string("undefined");
            break;
        case 1:
            sprintf(temp_str, "%d", lexem.get_ivalue());
            return string(temp_str);
            //return string("");
            break;
        case 2:
            if (lexem.get_bvalue())
                return string("true");
            else
                return string("false");
            break;
        case 3:
            return (lexem.get_svalue());
            break;
        case 4:
            return string("null");
        default:
            return string("");//no
            break;
    }
}

//! class Executer
//!======================================================================================
Executer::Executer()
{
    area_visibility = 0;
    index = 0;
}
void Executer::execute(Poliz& poliz)
{
    Stack <Lex, 1000> args;
    int index_in_TID;
    Lex check, arg, to, from, op1, op2;
    char temp_str[100] = "";
    int parametrs_number;
    
    for (;;)
    {
        curr_lex = poliz[index];
        if (curr_lex.get_type() == LEX_FIN || curr_lex.get_type() == LEX_FIN_PART)
        {
            ++index;
            break;
        }
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
                from_adreess_to_value(arg);
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
            case LEX_VAR:
            case LEX_ASSIGN:
                from = args.pop();
                to = args.pop();
                index_in_TID = to.get_ivalue();
                index_in_TID = TID.put(TID[index_in_TID].get_name(), area_visibility);
                from_adreess_to_value(from);
                (TID[index_in_TID]).type_of_value = from.get_type_of_value();
                switch (from.get_type_of_value())
                {
                case 1:
                    (TID[index_in_TID]).ivalue = from.get_ivalue();
                    args.push(Lex(LEX_NUM, (TID[index_in_TID]).ivalue));
                    break;
                case 2:
                    (TID[index_in_TID]).bvalue = from.get_bvalue();
                    args.push(Lex(LEX_BOOL, (TID[index_in_TID]).bvalue));
                    break;
                case 3:
                    (TID[index_in_TID]).svalue = from.get_svalue();
                    args.push(Lex(LEX_STR, (TID[index_in_TID]).svalue));
                    break;
                }
                break;
            case LEX_ENV:
                strcpy(temp_str, ((args.pop()).get_svalue()).c_str());
                args.push(Lex(LEX_STR, string(getenv(temp_str))));
                break;
            case POLIZ_CALL:
                ++area_visibility;
                if (area_visibility == 1)
                    poliz.change_free();
                index = curr_lex.get_ivalue();
                parametrs_number = args.pop().get_ivalue();
                return_address.push(args.pop());
                for (int i = 0; i < parametrs_number; ++i)
                {
                    from = args.pop();
                    to = poliz[index + parametrs_number - 1 - i];
                    index_in_TID = to.get_ivalue();
                    index_in_TID = TID.put(TID[index_in_TID].get_name(), area_visibility);
                    --area_visibility;
                    from_adreess_to_value(from);
                    ++area_visibility;
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
                }
                index += parametrs_number;
                break;
            case LEX_RETURN:
                index = return_address.pop().get_ivalue() - 1;
                if (area_visibility == 1)
                    poliz.change_free();
                TID.undeclare(area_visibility);
                --area_visibility;
                break;
            case LEX_ADD:
                op2 = args.pop();
                from_adreess_to_value(op2);
                op1 = args.pop();
                from_adreess_to_value(op1);
                switch (op1.get_type_of_value())
                {
                    case 1:
                        args.push(Lex(LEX_NUM, toInt(op1)+toInt(op2)));
                        break;
                    case 3:
                        args.push(Lex(LEX_STR, toString(op1)+toString(op2)));
                        break;
                    default:
                        throw string("Execute: wrong operands +");
                        break;
                }
                break;
            case LEX_SUB:
                op2 = args.pop();
                from_adreess_to_value(op2);
                op1 = args.pop();
                from_adreess_to_value(op1);
                switch (op1.get_type_of_value())
                {
                    case 1:
                        args.push(Lex(LEX_NUM, toInt(op1)-toInt(op2)));
                        break;
                    default:
                        throw string("Execute: wrong operands -\n");
                        break;
                }
                break;
            case LEX_MUL:
                op2 = args.pop();
                from_adreess_to_value(op2);
                op1 = args.pop();
                from_adreess_to_value(op1);
                switch (op1.get_type_of_value()) {
                    case 1:
                        args.push(Lex(LEX_NUM, toInt(op1)*toInt(op2)));
                        break;
                    default:
                        throw string("Execute: wrong operands *\n");
                        break;
                }
                break;
            case LEX_DIV:
                op2 = args.pop();
                from_adreess_to_value(op2);
                op1 = args.pop();
                from_adreess_to_value(op1);
                switch (op1.get_type_of_value()) {
                    case 1:
                        args.push(Lex(LEX_NUM, toInt(op1)/toInt(op2)));
                        break;
                    default:
                        throw string("Execute: wrong operands /\n");
                        break;
                }
                break;
            case LEX_PERCENT:
                op2 = args.pop();
                from_adreess_to_value(op2);
                op1 = args.pop();
                from_adreess_to_value(op1);
                switch (op1.get_type_of_value()) {
                    case 1:
                        args.push(Lex(LEX_NUM, toInt(op1)%toInt(op2)));
                        break;
                    default:
                        throw string("Execute: wrong operands %\n");
                        break;
                }
                break;
            case LEX_SEMICOLON:
                args.pop();
                break;
            case LEX_NOT:
                args.push(Lex(LEX_BOOL, !toBool(args.pop())));
                break;
            case LEX_INC: // i++ only ++i as i = i+1;
                arg = args.pop();
                index_in_TID = arg.get_ivalue();
                index_in_TID = TID.put(TID[index_in_TID].get_name(), area_visibility);
                if ((TID[index_in_TID]).type_of_value == 1)
                {
                    args.push(Lex(LEX_NUM, (TID[index_in_TID]).ivalue));
                    (TID[index_in_TID]).ivalue += 1;
                }
                else throw string("Execute: wrond operand i++");
                break;
            case LEX_DEC: // i-- only
                arg = args.pop();
                index_in_TID = arg.get_ivalue();
                index_in_TID = TID.put(TID[index_in_TID].get_name(), area_visibility);
                if ((TID[index_in_TID]).type_of_value == 1)
                {
                    args.push(Lex(LEX_NUM, (TID[index_in_TID]).ivalue));
                    (TID[index_in_TID]).ivalue -= 1;
                }
                else throw string("Execute: wrond operand i--");
                break;
            case LEX_EQ:
                op2 = args.pop();
                from_adreess_to_value(op2);
                op1 = args.pop();
                from_adreess_to_value(op1);
                switch (op1.get_type_of_value())
                {
                    case 0:
                        args.push(Lex(LEX_BOOL, (toBool(op1) == toBool(op2))));
                        break;
                    case 1:
                        args.push(Lex(LEX_BOOL, (toInt(op1) == toInt(op2))));
                        break;
                    case 2:
                        args.push(Lex(LEX_BOOL, (toBool(op1) == toBool(op2))));
                        break;
                    case 3:
                        args.push(Lex(LEX_BOOL, (toString(op1) == toString(op2))));
                        break;
                    default:
                        throw string("Execute: wrong operands ==");
                        break;
                }
                break;
            case LEX_NEQ:
                op2 = args.pop();
                from_adreess_to_value(op2);
                op1 = args.pop();
                from_adreess_to_value(op1);
                switch (op1.get_type_of_value())
                {
                case 0:
                    args.push(Lex(LEX_BOOL, (toBool(op1) != toBool(op2))));
                    break;
                case 1:
                    args.push(Lex(LEX_BOOL, (toInt(op1) != toInt(op2))));
                    break;
                case 2:
                    args.push(Lex(LEX_BOOL, (toBool(op1) != toBool(op2))));
                    break;
                case 3:
                    args.push(Lex(LEX_BOOL, (toString(op1) != toString(op2))));
                    break;
                default:
                    throw string("Execute: wrong operands !=");
                    break;
                }
                break;
            case LEX_G:
                op2 = args.pop();
                from_adreess_to_value(op2);
                op1 = args.pop();
                from_adreess_to_value(op1);
                switch (op1.get_type_of_value())
                {
                case 0:
                    args.push(Lex(LEX_BOOL, (toBool(op1) > toBool(op2))));
                    break;
                case 1:
                    args.push(Lex(LEX_BOOL, (toInt(op1) > toInt(op2))));
                    break;
                case 2:
                    args.push(Lex(LEX_BOOL, (toBool(op1) > toBool(op2))));
                    break;
                case 3:
                    args.push(Lex(LEX_BOOL, (toString(op1) > toString(op2))));
                    break;
                default:
                    throw string("Execute: wrong operands >");
                    break;
                }
                break;
            case LEX_GE:
                op2 = args.pop();
                from_adreess_to_value(op2);
                op1 = args.pop();
                from_adreess_to_value(op1);
                switch (op1.get_type_of_value())
                {
                case 0:
                    args.push(Lex(LEX_BOOL, (toBool(op1) >= toBool(op2))));
                    break;
                case 1:
                    args.push(Lex(LEX_BOOL, (toInt(op1) >= toInt(op2))));
                    break;
                case 2:
                    args.push(Lex(LEX_BOOL, (toBool(op1) >= toBool(op2))));
                    break;
                case 3:
                    args.push(Lex(LEX_BOOL, (toString(op1) >= toString(op2))));
                    break;
                default:
                    throw string("Execute: wrong operands >=");
                    break;
                }
                break;
            case LEX_L:
                op2 = args.pop();
                from_adreess_to_value(op2);
                op1 = args.pop();
                from_adreess_to_value(op1);
                switch (op1.get_type_of_value())
                {
                case 0:
                    args.push(Lex(LEX_BOOL, (toBool(op1) < toBool(op2))));
                    break;
                case 1:
                    args.push(Lex(LEX_BOOL, (toInt(op1) < toInt(op2))));
                    break;
                case 2:
                    args.push(Lex(LEX_BOOL, (toBool(op1) < toBool(op2))));
                    break;
                case 3:
                    args.push(Lex(LEX_BOOL, (toString(op1) < toString(op2))));
                    break;
                default:
                    throw string("Execute: wrong operands <");
                    break;
                }
                break;
            case LEX_LE:
                op2 = args.pop();
                from_adreess_to_value(op2);
                op1 = args.pop();
                from_adreess_to_value(op1);
                switch (op1.get_type_of_value())
                {
                case 0:
                    args.push(Lex(LEX_BOOL, (toBool(op1) <= toBool(op2))));
                    break;
                case 1:
                    args.push(Lex(LEX_BOOL, (toInt(op1) <= toInt(op2))));
                    break;
                case 2:
                    args.push(Lex(LEX_BOOL, (toBool(op1) <= toBool(op2))));
                    break;
                case 3:
                    args.push(Lex(LEX_BOOL, (toString(op1) <= toString(op2))));
                    break;
                default:
                    throw string("Execute: wrong operands <=");
                    break;
                }
                break;
            case LEX_OR:
                op2 = args.pop();
                from_adreess_to_value(op2);
                op1 = args.pop();
                from_adreess_to_value(op1);
                args.push(Lex(LEX_BOOL, (toBool(op1) || toBool(op2))));
                break;
            case LEX_AND:
                op2 = args.pop();
                from_adreess_to_value(op2);
                op1 = args.pop();
                from_adreess_to_value(op1);
                args.push(Lex(LEX_BOOL, (toBool(op1) && toBool(op2))));
                break;
            default:
                cout << curr_lex;
                throw string("Executer: Unexpected Poliz Lexem");
        }
        ++index;
    }
    //cout << "Execute has finished"<<endl;
}

//! class Executer
//!======================================================================================
void Interpretator::interpretation()
{
    for (;;)
    {
        if (parser.analyze())
        {
            executer.execute(parser.program_poliz);
        }
        else break;
    }
    //parser.program_poliz.print();
}

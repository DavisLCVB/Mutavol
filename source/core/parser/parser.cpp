#include "parser.hpp"

namespace mtv
{
    std::unique_ptr<Parser> Parser::instance = nullptr;

    Parser::Parser()
    {
        // Inicializacion del estado actual del automata a pilas
        this->p_state = "q0";
        // Inicializacion del estado de error
        this->error = false;
        // Inicializacion del parser de variables
        this->afdVars[L"q0"][L"DTYPE"] = L"q1";
        this->afdVars[L"q1"][L"IDENTIFIER"] = L"q2";
        this->afdVars[L"q2"][L";"] = L"qf";
        this->afdVars[L"q2"][L"="] = L"q3";
        this->afdVars[L"q2"][L","] = L"q1";
        this->afdVars[L"q3"][L"IDENTIFIER"] = L"q4";
        this->afdVars[L"q3"][L"LITERAL"] = L"q4";
        this->afdVars[L"q4"][L";"] = L"qf";
        this->afdVars[L"q4"][L","] = L"q1";

        // Inicializacion del parser de funciones
        this->afdFuncs[L"q0"][L"DTYPE"] = L"q1";
        this->afdFuncs[L"q1"][L"IDENTIFIER"] = L"q2";
        this->afdFuncs[L"q2"][L"("] = L"q3";
        this->afdFuncs[L"q3"][L"DTYPE"] = L"q4";
        this->afdFuncs[L"q3"][L")"] = L"qf";
        this->afdFuncs[L"q4"][L"IDENTIFIER"] = L"q5";
        this->afdFuncs[L"q5"][L","] = L"q6";
        this->afdFuncs[L"q5"][L")"] = L"qf";
        this->afdFuncs[L"q6"][L"DTYPE"] = L"q4";

        // Inicializacion del parser de llamadas
        this->afdCalls[L"q0"][L"("] = L"q2";
        this->afdCalls[L"q2"][L"IDENTIFIER"] = L"q3";
        this->afdCalls[L"q2"][L"LITERAL"] = L"q3";
        this->afdCalls[L"q2"][L"EXM"] = L"q3";
        this->afdCalls[L"q2"][L")"] = L"q5";
        this->afdCalls[L"q3"][L","] = L"q4";
        this->afdCalls[L"q3"][L")"] = L"q5";
        this->afdCalls[L"q4"][L"IDENTIFIER"] = L"q3";
        this->afdCalls[L"q4"][L"LITERAL"] = L"q3";
        this->afdCalls[L"q4"][L"EXM"] = L"q3";
        this->afdCalls[L"q5"][L";"] = L"qf";

        // Inicializacion del parser de for
        this->afdFor[L"q0"][L"for"] = L"q1";
        this->afdFor[L"q1"][L"("] = L"q2";
        this->afdFor[L"q2"][L"DTYPE"] = L"q3";
        this->afdFor[L"q3"][L"IDENTIFIER"] = L"q4";
        this->afdFor[L"q4"][L"="] = L"q5";
        this->afdFor[L"q5"][L"LITERAL"] = L"q6";
        this->afdFor[L"q6"][L";"] = L"q7";
        this->afdFor[L"q7"][L"IDENTIFIER"] = L"q8";
        this->afdFor[L"q8"][L"OPERATORCOMP"] = L"q9";
        this->afdFor[L"q9"][L"LITERAL"] = L"q10";
        this->afdFor[L"q10"][L";"] = L"q11";
        this->afdFor[L"q11"][L"IDENTIFIER"] = L"q12";
        this->afdFor[L"q12"][L"OPERATORDOBLE"] = L"q13";
        this->afdFor[L"q13"][L")"] = L"qf";
    }

    Parser &Parser::get_instance()
    {
        if (instance == nullptr)
        {
            init_parser();
        }
        return *instance;
    }

    void Parser::init_parser()
    {
        instance.reset(new Parser());
    }

    void Parser::evaluate_whit_afd(const State &afd)
    {
        std::wstring state = L"q0";
        while (state != L"qf")
        {
            if (this->current_token.lexem.empty())
            {
                this->error = true;
                break;
            }

            auto transition = afd.find(state);
            if (transition == afd.end())
            {
                // Estado no encontrado
                this->error = true;
                break;
            }

            auto next_state = transition->second.find(this->current_token.lexem);
            if (next_state == transition->second.end())
            {
                next_state = transition->second.find(type_wstr(this->current_token.type));
                // Estado no encontrado
                if (next_state == transition->second.end())
                {
                    this->error = true;
                    break;
                }
            }
            state = next_state->second;
            if (state != L"qf")
                this->current_token = mtv::Scanner::get();
        }

        if (this->error || state != L"qf")
        {
            std::wcout << L"Error en la linea: " << this->current_token.pos.row << L" columna: " << this->current_token.pos.column << L"\n";
            std::wcout << L"Error en la lexema: " << this->current_token.lexem << L" type:" << type_wstr(this->current_token.type) << L"\n";
            std::wcout << L"Error en el estado: " << state << L"\n";
        }
    }

    void Parser::parse()
    {
        this->current_token = mtv::Scanner::get();

        while (this->current_token.lexem != L"$")
        {
            if (this->p_state == "q0")
            {
                evaluate_whit_afd(this->afdFuncs);
                this->p_state = "q1";
            }
            else if (this->p_state == "q1")
            {
                if (this->current_token.lexem == L"{")
                    this->p_state = "q2";
                else
                    this->error = true;
            }
            else if (this->p_state == "q2")
            {
                if (this->current_token.type == TokenType::DTYPE)
                {
                    evaluate_whit_afd(this->afdVars);
                }
                else if (this->current_token.type == TokenType::KEYWORD)
                {
                    if (this->current_token.lexem == L"for")
                        evaluate_whit_afd(this->afdFor);
                    else if (this->current_token.lexem == L"if" || this->current_token.lexem == L"while")
                        // Se analiza la expresion logica
                        break;
                }
                else if (this->current_token.type == TokenType::IDENTIFIER)
                {
                    this->current_token = mtv::Scanner::get();
                    if (this->current_token.lexem == L"(")
                    {
                        evaluate_whit_afd(this->afdCalls);
                    }
                    else if (this->current_token.type == TokenType::OPERATORDOBLE || this->current_token.lexem == L"=")
                    {
                        // Se analiza la expresion matematica
                        break;
                    }
                }
                else if (this->current_token.lexem == L"}")
                {
                    this->p_state = "qf";
                }
                else
                {
                    this->error = true;
                }
            }
            this->current_token = mtv::Scanner::get();
            if (this->error == true)
                break;
        }

        if (this->error == false)
        {
            std::wcout << L"Pertenece al lenguaje\n";
        }
        else
        {
            std::wcout << L"Error en la linea: " << this->current_token.pos.row << L" columna: " << this->current_token.pos.column << L"\n";
            std::wcout << L"Error en la lexema: " << this->current_token.lexem << L" type:" << type_wstr(this->current_token.type) << L"\n";
            std::cout << "Error en el estado de pila: " << this->p_state << "\n";
        }
    }
} // namespace mtv
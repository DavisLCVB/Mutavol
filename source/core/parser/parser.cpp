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
        this->afdVars[L"q2"][L"\n"] = L"qf";
        this->afdVars[L"q2"][L"$"] = L"qf";
        this->afdVars[L"q2"][L"="] = L"q3";
        this->afdVars[L"q2"][L","] = L"q1";
        this->afdVars[L"q3"][L"IDENTIFIER"] = L"q4";
        this->afdVars[L"q3"][L"LITERAL"] = L"q4";
        this->afdVars[L"q4"][L"\n"] = L"qf";
        this->afdVars[L"q4"][L"$"] = L"qf";
        this->afdVars[L"q4"][L","] = L"q1";

        // Inicializacion del parser de funciones
        this->afdFuncs[L"q0"][L"("] = L"q1";
        this->afdFuncs[L"q1"][L")"] = L"q4";
        this->afdFuncs[L"q1"][L"DTYPE"] = L"q2";
        this->afdFuncs[L"q2"][L"IDENTIFIER"] = L"q3";
        this->afdFuncs[L"q2"][L"LITERAL"] = L"q3";
        this->afdFuncs[L"q3"][L")"] = L"q4";
        this->afdFuncs[L"q3"][L","] = L"q5";
        this->afdFuncs[L"q4"][L"\n"] = L"qf";
        this->afdFuncs[L"q4"][L"$"] = L"qf";
        this->afdFuncs[L"q5"][L"DTYPE"] = L"q2";
        

        // Inicializacion del parser de llamadas
        this->afdCalls[L"q0"][L"IDENTIFIER"] = L"q1";
        this->afdCalls[L"q0"][L"LITERAL"] = L"q1";
        this->afdCalls[L"q1"][L"\n"] = L"qf";
        this->afdCalls[L"q1"][L"$"] = L"qf";
        this->afdCalls[L"q1"][L","] = L"q2";
        this->afdCalls[L"q2"][L"IDENTIFIER"] = L"q1";
        this->afdCalls[L"q2"][L"LITERAL"] = L"q1";

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
                if (this->current_token.type == TokenType::DTYPE)
                {
                    this->p_state = "q1";
                    this->current_token = mtv::Scanner::get();
                }
            }
            else if (this->p_state == "q1")
            {
                if (this->current_token.type == TokenType::IDENTIFIER)
                {
                    // TODO: Se debe implementar el analisis con el automata a pilas
                    this->p_state = "q2";
                    this->current_token = mtv::Scanner::get();
                    evaluate_whit_afd(this->afdFuncs);
                }
            }
            else if (this->p_state == "q2")
            {
                if (this->current_token.type == TokenType::DTYPE)
                {
                    evaluate_whit_afd(this->afdVars);
                }
                else if (this->current_token.type == TokenType::KEYWORD)
                {
                    // Se analiza la condicional
                    break;
                }
                else if(this->current_token.type == TokenType::IDENTIFIER){
                    this->current_token = mtv::Scanner::get();
                    if(this->current_token.type == TokenType::IDENTIFIER){
                        evaluate_whit_afd(this->afdCalls);
                    }else if(this->current_token.lexem == L"="){
                        // Se analiza la expresion matematica
                        break;
                    }
                }
                else
                {
                    this->error = true;
                }
            }
            else
            {
                this->error = true;
            }

            if (this->error == true)
                break;
        }

        if (this->error == false)
        {
            std::wcout << L"Pertenece al lenguaje\n";
        }
    }
} // namespace mtv
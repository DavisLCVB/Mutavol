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

        // Inicializacion del automata a pila
        this->apd["q0"][L"{"][L"P0"][L"P0"] = ResultAPD{"q1", L"{", L"-"};
        this->apd["q1"][L"{"][L"{"][L"P0"] = ResultAPD{"q1", L"-", L"{"};
        this->apd["q1"][L"{"][L"{"][L"{"] = ResultAPD{"q1", L"-", L"{"};
        this->apd["q1"][L"}"][L"{"][L"{"] = ResultAPD{"q1", L"-", L"d"};
        this->apd["q1"][L"}"][L"{"][L"P0"] = ResultAPD{"q0", L"d", L"-"};
        this->apd["q0"][L"$"][L"P0"][L"P0"] = ResultAPD{"qf", L"-", L"-"};
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
                // Simbolo de salida no encontrado
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

    void Parser::evaluate_apd()
    {
#if DEBUG
        std::wcout << L"---------Antes de Actualizar: " << L"\n";
        std::wcout << L"Token: " << this->current_token.lexem << L"\n";
        std::cout << "Estado de pila: " << this->p_state << "\n";
        std::wcout << L"Pila de funciones: " << evaluate_pila(this->stack_func) << L"\n";
        std::wcout << L"Pila de bloques: " << evaluate_pila(this->stack_bloq) << L"\n";
#endif
        if (this->current_token.lexem.empty())
        {
            this->error = true;
            return;
        }

        /* auto simbol = this->apd.find(this->p_state);
        if (simbol == this->apd.end())
        {
            this->error = true;
            return;
        }
 */

        Simbol simbol;
        if (!sgetValues(simbol, this->apd, this->p_state)) return;

        P1Result likely_state_p1;
        if (!wsgetValues(likely_state_p1, simbol, this->current_token.lexem)) return;

        P2Result likely_state_p2;
        if (!wsgetValues(likely_state_p2, likely_state_p1, evaluate_pila(this->stack_func))) return;

        ResultAPD resultapd;
        if (!wsgetValues(resultapd, likely_state_p2, evaluate_pila(this->stack_bloq))) return;

        this->p_state = resultapd.nextState;
        this->pila_action(this->stack_func, resultapd.actionP1);
        this->pila_action(this->stack_bloq, resultapd.actionP2);
#if DEBUG
        std::wcout << L"---------Despues de Actualizar: " << L"\n";
        std::wcout << L"Token: " << this->current_token.lexem << L"\n";
        std::cout << "Estado de pila: " << this->p_state << "\n";
        std::wcout << L"Pila de funciones: " << evaluate_pila(this->stack_func) << L"\n";
        std::wcout << L"Pila de bloques: " << evaluate_pila(this->stack_bloq) << L"\n";
#endif
    }

    std::wstring Parser::evaluate_pila(std::stack<std::wstring> &pila)
    {
        if (pila.empty())
            return L"P0";
        return pila.top();
    }

    void Parser::pila_action(std::stack<std::wstring> &pila, std::wstring action)
    {
        if (action == L"d")
        {
            if (pila.empty())
            {
                this->error = true;
                return;
            }
            pila.pop();
        }
        else if (action == L"-")
            return;
        else
            pila.push(action);
    }

    void Parser::parse()
    {
        this->current_token = mtv::Scanner::get();

        while (this->p_state != "qf" && this->error == false)
        {
            if (this->p_state == "q0")
            {
                if (this->current_token.type == TokenType::DTYPE)
                {
                    evaluate_whit_afd(this->afdFuncs);
                    this->current_token = mtv::Scanner::get();
                }
                evaluate_apd();
            }
            else if (this->p_state == "q1")
            {
                // Es declaracion de variables
                if (this->current_token.type == TokenType::DTYPE)
                {
                    evaluate_whit_afd(this->afdVars);
                }
                // Es declaracion de bloques (for, if, while)
                else if (this->current_token.type == TokenType::KEYWORD)
                {
                    if (this->current_token.lexem == L"for")
                        evaluate_whit_afd(this->afdFor);
                    else if (this->current_token.lexem == L"if" || this->current_token.lexem == L"while")
                        // Se analiza la expresion logica
                        break;
                    this->current_token = mtv::Scanner::get();
                    evaluate_apd();
                }
                // Es una llamada a funcion o una expresion matematica
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
                // Es cerrado de bloque de instrucciones
                else if (this->current_token.lexem == L"}")
                {
                    evaluate_apd();
                }
                else
                {
                    this->error = true;
                }
            }

            if (this->error == true)
                break;
            else
                this->current_token = mtv::Scanner::get();
        }

        if (this->error == false && this->p_state == "qf")
            std::wcout << L"Pertenece al lenguaje\n";
        else
        {
            std::wcout << L"Error en la linea: " << this->current_token.pos.row << L" columna: " << this->current_token.pos.column << L"\n";
            std::wcout << L"Error en la lexema: " << this->current_token.lexem << L" type:" << type_wstr(this->current_token.type) << L"\n";
            std::cout << "Error en el estado de pila: " << this->p_state << "\n";
        }
    }

    bool Parser::wsgetValues(auto &putIn, auto &map, std::wstring key)
    {
        auto value = map.find(key);
        if (value == map.end()){
            this->error = true;
            return false;
        }
        putIn = value->second;
        return true;
    }

    bool Parser::sgetValues(auto &putIn, auto &map, std::string key)
    {
        auto value = map.find(key);
        if (value == map.end()){
            this->error = true;
            return false;
        }
        putIn = value->second;
        return true;
    }
} // namespace mtv
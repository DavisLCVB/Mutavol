#include "parser.hpp"
#include <locale>
#include <codecvt>

namespace mtv
{
    std::unique_ptr<Parser> Parser::instance = nullptr;

    Parser::Parser()
    {
        // Inicializacion del flag de if
        this->ifflag = false;
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

        // control de la estructura if
        this->apd["q1"][L"}"][L"{"][L"if"] = ResultAPD{"q1", L"-", L"d"};
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

    void Parser::get_next_token()
    {
        this->current_token = Scanner::get();
        std::wcout << L"Token: " << this->current_token.lexem << L"\n";
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
            if (transition == afd.end()) {
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
                get_next_token();
        }

        if (this->error || state != L"qf")
        {
            std::wcout << L"Error en la linea: " << this->current_token.pos.row << L" columna: " << this->current_token.pos.column << L"\n";
            std::wcout << L"Error en la lexema: " << this->current_token.lexem << L" type:" << type_wstr(this->current_token.type) << L"\n";
            std::wcout << L"Error en el estado: " << state << L"\n";
        }
    }

    bool Parser::evaluate_conditional()
    {
        try
        {
            get_next_token();
            X();
            return true;
        }
        catch (const std::runtime_error &e)
        {
            std::wcout << e.what() << L"\n";
            return false;
        }
    }

    void Parser::X()
    {
        if (current_token.lexem == L"(")
        {

            get_next_token();
            S();
            if (current_token.lexem != L")")
                throw std::runtime_error("Error");

            /*             this->current_token = Scanner::get();
                        if (current_token.lexem != L"{")
                            throw std::runtime_error("Token esperado: '{'"); */
        }
        else
        {
            throw std::runtime_error("token esperado al inicio de la condicional: '('");
        }
    }

    void Parser::S()
    {
        if (current_token.lexem == L"(")
        {

            get_next_token();
            S();
            if (current_token.lexem != L")")
                throw std::runtime_error("token esperado1: ')'");
            this->current_token = mtv::Scanner::get();
            Y();
        }
        else
        {
            H();
            Y();
        }
    }

    void Parser::H()
    {
        if (current_token.type == TokenType::IDENTIFIER || current_token.type == TokenType::LITERAL)
        {
            get_next_token();

            if (current_token.type != TokenType::OPERATORCOMP)
                throw std::runtime_error("Token de comparación esperado");

            get_next_token();

            if (current_token.type != TokenType::IDENTIFIER && current_token.type !=
                TokenType::LITERAL)
                throw std::runtime_error("Identificador o literal esperado");

            get_next_token();
        }
        else if (current_token.lexem == L"(")
        {

            get_next_token();
            S();
            if (current_token.lexem != L")")
                throw std::runtime_error("Token esperado2: ')'");

            get_next_token();
        }
        else
        {
            std::string lexem_str = wstring_to_string(current_token.lexem);
            throw std::runtime_error("Token inesperado:" + lexem_str);
        }
    }

    void Parser::Y()
    {
        if (current_token.lexem == L"&&" || current_token.lexem == L"||")
        {
            get_next_token();
            S();
            Y();
        }
        else if (current_token.type == TokenType::OPERATORCOMP)
        {
            get_next_token();
            H();
            Y();
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
        if (!sgetValues(simbol, this->apd, this->p_state))
            return;

        P1Result likely_state_p1;
        if (!wsgetValues(likely_state_p1, simbol, this->current_token.lexem))
            return;

        P2Result likely_state_p2;
        if (!wsgetValues(likely_state_p2, likely_state_p1, evaluate_pila(this->stack_func)))
            return;

        ResultAPD resultapd;
        if (!wsgetValues(resultapd, likely_state_p2, evaluate_pila(this->stack_bloq)))
            return;

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
            if (pila.top() == L"if")
                this->ifflag = false;
            std::wstring top = pila.top();
            std::wcout << L"Se elimino: " << top << L"\n";
            pila.pop();
        }
        else if (action == L"-")
            return;
        else
        {
            pila.push(action);
            std::wcout << L"Se agrego: " << action << L"\n";
        }
    }

    void Parser::parse()
    {
        get_next_token();

        while (this->p_state != "qf" && this->error == false && !this->current_token.lexem.empty())
        {
            if (this->p_state == "q0")
            {
                if (this->current_token.type == TokenType::DTYPE)
                {
                    evaluate_whit_afd(this->afdFuncs);
                    get_next_token();
                    F();
                }
                else if (this->current_token.lexem == L"$")
                {
                    evaluate_apd();
                    break;
                }
                else
                    this->error = true;
            }
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

    bool Parser::eval_struct_condition()
    {
        try
        {
            Z();
            return true;
        }
        catch (const std::runtime_error &e)
        {
            std::string mess = e.what();
            if (mess == "Intentional error")
                return true;
            std::wcout << e.what() << L"\n";
            return false;
        }
    }

    void Parser::Z()
    {
        if (this->current_token.lexem == L"if")
        {
            if (!evaluate_conditional())
                throw std::runtime_error("Error en la condicional");
            get_next_token();
            F();
            //get_next_token();
            W();
        }
        else
        {
            throw std::runtime_error("Token esperado: 'if'");
        }
    }

    void Parser::V()
    {
        if (this->current_token.lexem == L"if")
        {
            Z();
        }
        else if (this->current_token.lexem == L"{")
        {
            F();
            //get_next_token();
        }
        else
        {
            throw std::runtime_error("Token esperado: '{'");
        }
    }

    void Parser::W()
    {
        if (this->current_token.lexem == L"else")
        {
            get_next_token();
            V();
        }
    }

    void Parser::F()
    {
        if (this->current_token.lexem == L"{")
        {
            evaluate_apd();
            get_next_token();
            T();
            evaluate_apd();
            if (this->current_token.lexem != L"}")
            {
                throw std::runtime_error("Token esperado: '}'");
            }
            get_next_token();
        }
        else
        {
            throw std::runtime_error("Token esperado: '{'");
        }
    }

    void Parser::T() {
        if(this->current_token.type == TokenType::DTYPE || this->current_token.type == TokenType::IDENTIFIER
            || this->current_token.type == TokenType::KEYWORD) {
            I();
            T();
        } else if(this->current_token.lexem == L"}") {
            //lambda
        } else {
            throw std::runtime_error("Error en T");
        }
    }


    void Parser::I()
    {
        if (this->current_token.type == TokenType::IDENTIFIER)
        {
            get_next_token();
            A();
            get_next_token();
        }
        else if (this->current_token.type == TokenType::DTYPE)
        {
            B();
            get_next_token();
        }
        else if (this->current_token.type == TokenType::KEYWORD)
        {
            C();
        } else {

        }
    }

    void Parser::A()
    {
        if (this->current_token.lexem == L"(")
        {
            evaluate_whit_afd(this->afdCalls);
        }
        else if (this->current_token.lexem == L"=")
        {
            get_next_token();
            evaluate_math_exp();
        }
        else
        {
            throw std::runtime_error("Token esperado: '(' o '='");
        }
    }

    void Parser::B()
    {
        if (this->current_token.type == TokenType::DTYPE)
        {
            evaluate_whit_afd(this->afdVars);
        }
        else
        {
            throw std::runtime_error("Token esperado: DTYPE");
        }
    }

    void Parser::C()
    {
        if (this->current_token.lexem == L"for")
        {
            evaluate_whit_afd(this->afdFor);
            get_next_token();
            F();
            //get_next_token();
        }
        else if (this->current_token.lexem == L"if")
        {
            Z();
        }
        else if (this->current_token.lexem == L"while")
        {
            if (!evaluate_conditional())
                throw std::runtime_error("Error en la condicional");
            std::wcout << L"Luego de evaluar la condicional: "<< this->current_token.lexem << L"\n";
            get_next_token();
            F();
            //get_next_token();
        }
        else
        {
            throw std::runtime_error("Token esperado: 'for', 'if' o 'while'");
        }
    }
    bool Parser::wsgetValues(auto &putIn, auto &map, std::wstring key)
    {
        auto value = map.find(key);
        if (value == map.end())
        {
            this->error = true;
            return false;
        }
        putIn = value->second;
        return true;
    }

    bool Parser::sgetValues(auto &putIn, auto &map, std::string key)
    {
        auto value = map.find(key);
        if (value == map.end())
        {
            this->error = true;
            return false;
        }
        putIn = value->second;
        return true;
    }

    void Parser::evaluate_math_exp()
    {
        S_math();
        if(this->current_token.lexem != L";") {
            throw std::runtime_error("Token esperado: ';'");
        }
    }

    void Parser::S_math() {
        if(current_token.type == TokenType::IDENTIFIER || current_token.type == TokenType::LITERAL) {
            get_next_token();
            while(current_token.lexem != L";") {
                if(current_token.type != TokenType::OPERATOREXP) {
                    throw std::runtime_error("Token esperado: OPERATOREXP");
                }
                get_next_token();
                if(current_token.type != TokenType::IDENTIFIER && current_token.type != TokenType::LITERAL) {
                    throw std::runtime_error("Token esperado: IDENTIFIER o LITERAL");
                }
                get_next_token();
            }
        } else {
            throw std::runtime_error("Token esperado: IDENTIFIER o LITERAL");
        }
    }

} // namespace mtv
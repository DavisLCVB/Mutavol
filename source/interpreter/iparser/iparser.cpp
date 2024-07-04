#include "iparser.hpp"

#include "../../core/parser/parser.hpp"

namespace mtv {

    std::unique_ptr<Iparser> Iparser::instance = nullptr;

    Iparser::Iparser() {
        //Inicializando el token
        this->tok = Token_t();
        //Inicializando la tabla de simbolos
        this->ts_pointer = 0;
        //Inicializando el auxiliar temporal de declaraciones
        this->temp_declaration = std::make_tuple(L"", L"", this->ts_pointer);
        //Inicializando el estado inicial del parser
        this->p_state = "q0";
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
    }

    void Iparser::evaluate_whit_afd(const State &afd)
    {
        std::wstring state = L"q0";
        while (state != L"qf")
        {
            if (this->tok.lexem.empty())
            {
                throw std::runtime_error("Se esperaba un token");
            }

            auto transition = afd.find(state);
            if (transition == afd.end())
            {
                // Simbolo de salida no encontrado
                throw std::runtime_error("Token esperado no identificable");
            }

            auto next_state = transition->second.find(this->tok.lexem);
            if (next_state == transition->second.end())
            {
                next_state = transition->second.find(type_wstr(this->tok.type));
                // Estado no encontrado
                if (next_state == transition->second.end())
                {
                    std::wstring options;
                    for (auto &option : transition->second)
                    {
                        options += option.first + L" ";
                    }
                    throw std::runtime_error("Token esperado: " + wstring_to_string(options));
                }
            }

            state = next_state->second;
            //Se supone si encontro un DTYPE
            if(state == L"q1") {
                std::get<0>(this->temp_declaration) = this->tok.lexem;
            }
            //Se supone si encontro un IDENTIFIER como nombre de variable
            if(state == L"q2") {
                //VERIFICA SI EL IDENTIFICADOR YA EXISTE
                if(getvar(this->tok.lexem).has_value()){
                    std::string lexem_str = wstring_to_string(this->tok.lexem);
                    throw std::runtime_error("La variable " + lexem_str + " ya ha sido declarada.");
                }
                std::get<1>(this->temp_declaration) = this->tok.lexem;
            }
            //Se supone si encontro un IDENTIFIER O LITERAL como valor de la variable
            if(state == L"q4") {
                std::get<2>(this->temp_declaration) = this->ts_pointer;
                this->ts.push_back(this->temp_declaration);
                C();
                //Ultimo de C es una ',' o un ';'
                this->memoria[this->ts_pointer] = evaluate_expr();
                this->ts_pointer++;
                continue;
            }

            //PARA NO ALTERAR EL AUTOMATA
            if(state == L"q3")
                get_next_expr_value();
            else if(state != L"qf")
                get_next_token();

        }

    }

    Iparser &Iparser::get_instance()
    {
        if (instance == nullptr)
        {
            init_parser();
        }
        return *instance;
    }

    void Iparser::init_parser()
    {
        instance.reset(new Iparser());
    }

    void Iparser::get_next_token() {
        this->tok = Scanner::get();
    }

    void Iparser::get_next_expr_value() {
        get_next_token();
        this->expr.push_back(this->tok.lexem);
    }

    void Iparser::get_next_output() {
        get_next_token();
        this->output.push_back(this->tok);
    }

    void Iparser::parse()
    {
        enableColors();
        while(true) {
            Scanner::init_scanner();
            get_next_token();
            //Si el primer token que le llega es $
            //Significa fin del programa
            if(this->tok.lexem == L"$") break;

            try {
                Z();
            } catch (std::runtime_error &e) {
                this->expr.clear();
                this->output.clear();
                this->last_var.clear();
                this->temp_declaration = std::make_tuple(L"", L"", this->ts_pointer);
                std::wcout << L"Error en la posicion: " << this->tok.pos.column << L"\n";
                std::wcout << L"Error en la lexema: " << this->tok.lexem << L" - type:" << type_wstr(this->tok.type) << L"\n";
                std::wcout << e.what() << std::endl;
            }

/*
            while (true) {
                std::wcout << this->tok.lexem << std::endl;
                get_next_token();
                if(this->tok.lexem == L"$") break;

            }
            */
        }
    }

    void Iparser::Z()
    {
        A();

        if(this->tok.lexem == L";")
        {
            get_next_token();
            if(this->tok.lexem != L"$") {
                throw std::runtime_error("Error luego de terminar una linea de codigo");
            }
        }
    }

    void Iparser::A() {
        if(this->tok.lexem == L"int" || this->tok.lexem == L"float" || this->tok.lexem == L"double") {
            evaluate_whit_afd(this->afdVars);
        } else if(this->tok.lexem == L"cout") {
            get_next_token();
            E();
            my_cout();
        } else if(this->tok.type == TokenType::IDENTIFIER ) {
            this->last_var= this->tok.lexem;
            get_next_token();
            B();
            assign_value();
        } else {
            throw std::runtime_error("Token esperado: DTYPE, cout o IDENTIFIER");
        }
    }

    void Iparser::B() {
        if(this->tok.lexem == L"=") {
            get_next_expr_value();
            C();
            if(this->tok.lexem != L";") {
                throw std::runtime_error("Token esperado para terminar la instrucción: ;");
            }
        } else {
            throw std::runtime_error("Token esperado para comenzar la asignacion: =");
        }
    }

    void Iparser::E() {
        if(this->tok.lexem == L"<") {
            get_next_token();
            if(this->tok.lexem == L"<") {
                get_next_output();
                K();
                D();
                /*if(this->tok.type == TokenType::IDENTIFIER || this->tok.type == TokenType::LITERAL) {
                    get_next_token();
                    D();
                }*/
            }else {
                throw std::runtime_error("Token esperado para comenzar la instrucción: <<");
            }
        } else {
            throw std::runtime_error("Token esperado para comenzar la instrucción: <<");
        }
    }

    void Iparser::K() {
        if(this->tok.type == TokenType::IDENTIFIER || this->tok.type == TokenType::LITERAL) {
            this->expr.push_back(this->tok.lexem);
            C();
            const double result = evaluate_expr();
            this->output.back().lexem = std::to_wstring(result);
            this->output.back().type = TokenType::LITERAL;
        } else if(this->tok.lexem == L"\"") {
            get_next_token();
            this->output.back().type = TokenType::LITERAL;
            this->output.back().lexem = this->tok.lexem + L" ";
            while(this->tok.lexem != L"\"") {
                get_next_token();
                this->output.back().lexem += this->tok.lexem +L" ";
            }
            //Eliminamos el " " y la "
            this->output.back().lexem.pop_back();
            this->output.back().lexem.pop_back();
            //Refresh del siguiente token para la funcion D
            get_next_token();
        }else {
            throw std::runtime_error("Token esperado: IDENTIFIER, NUMERO o \"");
        }
    }

    void Iparser::D() {
        if(this->tok.lexem == L"<") {
            E();
        } else if(this->tok.lexem == L";") {
            //lambda
        } else {
            throw std::runtime_error("Token esperado: << o ;");
        }
    }

    void Iparser::C() {
        W();
        U();
    }

    void Iparser::U() {
        if(this->tok.lexem == L"+") {
            get_next_expr_value();
            C();
        } else if(this->tok.lexem == L"-") {
            get_next_expr_value();
            C();
        } else if(this->tok.lexem == L";" || this->tok.lexem == L"," || this->tok.lexem == L"<" || this->tok.lexem == L")") {
            //lambda
        } else {
            throw std::runtime_error("Token esperado: Operador o Fin de expresion");
        }
    }

    void Iparser::W() {
        Y();
        P();
    }

    void Iparser::P() {
        if(this->tok.lexem == L"*") {
            get_next_expr_value();
            W();
        } else if(this->tok.lexem == L"/") {
            get_next_expr_value();
            W();
        } else if(this->tok.lexem == L";" || this->tok.lexem == L"," || this->tok.lexem == L"<" || this->tok.lexem == L")" || this->tok.lexem == L"+" || this->tok.lexem == L"-") {
            //lambda
        } else {
            throw std::runtime_error("Token esperado: Operador o Fin de expresion");
        }
    }

    void Iparser::Y() {
        O();
        F();
    }


    void Iparser::F() {
        if(this->tok.lexem == L"^") {
            get_next_expr_value();
            Y();
        } else if(this->tok.lexem == L"*" || this->tok.lexem == L"/" || this->tok.lexem == L"+" || this->tok.lexem == L"-" || this->tok.lexem == L";" || this->tok.lexem == L"," || this->tok.lexem == L"<" || this->tok.lexem == L")") {
            //lambda
        } else {
            throw std::runtime_error("Token esperado: Operador o Fin de expresion");
        }
    }

    void Iparser::O() {
        if (this->tok.lexem == L"(") {
            get_next_expr_value();
            C();
            if(this->tok.lexem != L")") {
                throw std::runtime_error("Token esperado: )");
            }
            get_next_expr_value();
        } else if(this->tok.type == TokenType::IDENTIFIER) {
            get_next_expr_value();
        } else if(this->tok.type == TokenType::LITERAL) {
            get_next_expr_value();
        } else {
            throw std::runtime_error("Token esperado: ( o IDENTIFIER o NUMERO");
        }
    }

    double Iparser::evaluate_expr() {
        std::stack<std::wstring> stackOperators;
        std::stack<double> stackOperands;

        for(std::wstring &e : this->expr) {
            if(e == L";" || e ==L"," || e == L"<") break;
            if(e == L"^") {
                stackOperators.push(e);
            } else if(e == L"+" || e == L"-") {
                while (!stackOperators.empty() && stackOperators.top() == L"^") {
                    evaluate_operands(stackOperators, stackOperands);
                }

                while (!stackOperators.empty() && (stackOperators.top() == L"%" || stackOperators.top() == L"*" || stackOperators.top() == L"/")) {
                    evaluate_operands(stackOperators, stackOperands);
                }

                stackOperators.push(e);
            } else if (e == L"*" || e == L"/" || e == L"%") {

                while(!stackOperators.empty() && stackOperators.top() == L"^") {
                    evaluate_operands(stackOperators, stackOperands);
                }

                while (!stackOperators.empty() && (stackOperators.top() == L"^" || stackOperators.top() == L"*" || stackOperators.top() == L"/" || stackOperators.top() == L"%")) {
                    evaluate_operands(stackOperators, stackOperands);
                }
                stackOperators.push(e);
            } else if(e == L"(") {
                stackOperators.push(e);
            } else if(e == L")") {
                while (!stackOperators.empty() && stackOperators.top() != L"(") {
                    evaluate_operands(stackOperators, stackOperands);
                }
                stackOperators.pop();
            } else {
                try {
                    stackOperands.push(std::stod(e));
                } catch (const std::exception& err){
                    std::optional<double> r = getvar(e);
                    if(!r.has_value()){
                        std::string lexem_str = wstring_to_string(e);
                        throw std::runtime_error("La variable " + lexem_str + " no ha sido declarada.");
                    }
                    stackOperands.push(r.value());
                }
            }
        }

        while(!stackOperators.empty()) {
            evaluate_operands(stackOperators, stackOperands);
        }
        //DEBUG DE EXPRESIONES
        /*std::wcout << L"Expr : ";
        for(std::wstring &e : this->expr) {
            std::wcout << e << L" ";
        }
        std::wcout << std::endl;
        std::cout << "Result: " << stackOperands.top() << std::endl;*/

        this->expr.clear();
        return stackOperands.top();
    }

    void Iparser::evaluate_operands(std::stack<std::wstring> &operators, std::stack<double> &operands) {
        const std::wstring op = operators.top();
        operators.pop();
        const double op2 = operands.top();
        operands.pop();
        const double op1 = operands.top();
        operands.pop();
        if(op == L"+") {
            operands.push(op1 + op2);
        } else if(op == L"-") {
            operands.push(op1 - op2);
        } else if(op == L"*") {
            operands.push(op1 * op2);
        } else if(op == L"/") {
            operands.push(op1 / op2);
        } else if(op == L"^") {
            operands.push(std::pow(op1, op2));
        } else if(op == L"%") {
            operands.push(std::fmod(op2, op1));
        }
    }

    void Iparser::my_cout() {
        std::wcout << L"\033[1;34m>> ";
        for(Token_t &e: this->output) {
            if(e.type == TokenType::LITERAL)
                std::wcout << e.lexem;
            else if(e.type == TokenType::IDENTIFIER) {
                std::optional<double> r = getvar(e.lexem);
                if(!r.has_value()){
                    std::string lexem_str = wstring_to_string(e.lexem);
                    throw std::runtime_error("La variable " + lexem_str + "no ha sido declarada.");
                }
                std::wcout << r.value();
            }
        }
        this->output.clear();
        std::wcout << L"\033[0m" << std::endl;
    }

    void Iparser::assign_value() {
        for(row &r : this->ts) {
            if(std::get<1>(r) == this->last_var) {
                this->memoria[std::get<2>(r)] = evaluate_expr();
                this->last_var.clear();
                return;
            }
        }
        throw std::runtime_error("La variable " + wstring_to_string(this->last_var) + " no ha sido declarada.");
    }


    std::optional<double> Iparser::getvar(const std::wstring &var) {
        for(row &r : this->ts) {
            if(std::get<1>(r) == var) {
                return this->memoria[std::get<2>(r)];
            }
        }
        return std::nullopt;
    }

    void Iparser::enableColors() {
     HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
     if (hOut == INVALID_HANDLE_VALUE) return;

     DWORD dwMode = 0;
     if (!GetConsoleMode(hOut, &dwMode)) return;

     dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
     if (!SetConsoleMode(hOut, dwMode)) return;
    }

}

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
                throw std::runtime_error("Error en la linea: " + std::to_string(this->tok.pos.row) + " columna: " + std::to_string(this->tok.pos.column));
            }

            auto transition = afd.find(state);
            if (transition == afd.end())
            {
                // Simbolo de salida no encontrado
                throw std::runtime_error("Error en la linea: " + std::to_string(this->tok.pos.row) + " columna: " + std::to_string(this->tok.pos.column));
            }

            auto next_state = transition->second.find(this->tok.lexem);
            if (next_state == transition->second.end())
            {
                next_state = transition->second.find(type_wstr(this->tok.type));
                // Estado no encontrado
                if (next_state == transition->second.end())
                {
                    throw std::runtime_error("Error en la linea: " + std::to_string(this->tok.pos.row) + " columna: " + std::to_string(this->tok.pos.column));
                }
            }

            state = next_state->second;
            //Se supone si encontro un DTYPE
            if(state == L"q1") {
                std::get<0>(this->temp_declaration) = this->tok.lexem;
            }
            //Se supone si encontro un IDENTIFIER como nombre de variable
            if(state == L"q2") {
                std::get<1>(this->temp_declaration) = this->tok.lexem;
            }
            //Se supone si encontro un IDENTIFIER O LITERAL como valor de la variable
            if(state == L"q4") {
                std::get<2>(this->temp_declaration) = this->ts_pointer;
                this->ts.push_back(this->temp_declaration);
                C();
                //Ultimo de C es una ',' o un ';'
                this->memoria[this->ts_pointer] = evaluate_expr();
                /*
                if(this->tok.type == TokenType::LITERAL)
                    this->memoria[this->ts_pointer] = std::stod(this->tok.lexem);
                else if(this->tok.type == TokenType::IDENTIFIER) {
                    bool band = false;
                    //Recuperar valor de memoria
                    for(row &r : this->ts) {
                        if(std::get<1>(r) == this->tok.lexem) {
                            this->memoria[this->ts_pointer] = this->memoria[std::get<2>(r)];
                            band = true;
                            break;
                        }
                    }
                    std::string lexem_str = wstring_to_string(this->tok.lexem);
                    if(!band) throw std::runtime_error("La variable " + lexem_str + "no ha sido declarada.");
                }
                */
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
        while(true) {
            Scanner::init_scanner();
            get_next_token();
            //Si el primer token que le llega es $
            //Significa fin del programa
            if(this->tok.lexem == L"$") break;

            try {
                Z();
            } catch (std::runtime_error &e) {
                std::wcout << L"Error en la linea: " << this->tok.pos.row << L" columna: " << this->tok.pos.column << L"\n";
                std::wcout << L"Error en la lexema: " << this->tok.lexem << L" type:" << type_wstr(this->tok.type) << L"\n";
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
            get_next_token();
            B();
            evaluate_expr();
        }
    }

    void Iparser::B() {
        if(this->tok.lexem == L"=") {
            get_next_expr_value();
            C();
            if(this->tok.lexem != L";") {
                throw std::runtime_error("Error en la linea: " + std::to_string(this->tok.pos.row) + " columna: " + std::to_string(this->tok.pos.column));
            }
        } else {
            throw std::runtime_error("Error en la linea: " + std::to_string(this->tok.pos.row) + " columna: " + std::to_string(this->tok.pos.column));
        }
    }

    void Iparser::E() {
        if(this->tok.lexem == L"<") {
            get_next_token();
            if(this->tok.lexem == L"<") {
                get_next_output();
                if(this->tok.type == TokenType::IDENTIFIER || this->tok.type == TokenType::LITERAL) {
                    get_next_token();
                    D();
                }
            }
        } else {
            throw std::runtime_error("Error en la linea: " + std::to_string(this->tok.pos.row) + " columna: " + std::to_string(this->tok.pos.column));
        }
    }

    void Iparser::D() {
        if(this->tok.lexem == L"<") {
            E();
        } else if(this->tok.lexem == L";") {
            //lambda
        } else {
            throw std::runtime_error("Error en la linea: " + std::to_string(this->tok.pos.row) + " columna: " + std::to_string(this->tok.pos.column));
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
        } else if(this->tok.lexem == L";" || this->tok.lexem == L"," || this->tok.lexem == L")") {
            //lambda
        } else {
            throw std::runtime_error("Error en la linea: " + std::to_string(this->tok.pos.row) + " columna: " + std::to_string(this->tok.pos.column));
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
        } else if(this->tok.lexem == L";" || this->tok.lexem == L"," || this->tok.lexem == L")" || this->tok.lexem == L"+" || this->tok.lexem == L"-") {
            //lambda
        } else {
            throw std::runtime_error("Error en la linea: " + std::to_string(this->tok.pos.row) + " columna: " + std::to_string(this->tok.pos.column));
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
        } else if(this->tok.lexem == L"*" || this->tok.lexem == L"/" || this->tok.lexem == L"+" || this->tok.lexem == L"-" || this->tok.lexem == L";" || this->tok.lexem == L"," || this->tok.lexem == L")") {
            //lambda
        } else {
            throw std::runtime_error("Error en la linea: " + std::to_string(this->tok.pos.row) + " columna: " + std::to_string(this->tok.pos.column));
        }
    }

    void Iparser::O() {
        if (this->tok.lexem == L"(") {
            get_next_expr_value();
            C();
            if(this->tok.lexem != L")") {
                throw std::runtime_error("Error en la linea: " + std::to_string(this->tok.pos.row) + " columna: " + std::to_string(this->tok.pos.column));
            }
            get_next_expr_value();
        } else if(this->tok.type == TokenType::IDENTIFIER) {
            get_next_expr_value();
        } else if(this->tok.type == TokenType::LITERAL) {
            get_next_expr_value();
        } else {
            throw std::runtime_error("Error en la linea: " + std::to_string(this->tok.pos.row) + " columna: " + std::to_string(this->tok.pos.column));
        }
    }

    double Iparser::evaluate_expr() {
        std::stack<std::wstring> stackOperators;
        std::stack<double> stackOperands;

        for(std::wstring &e : this->expr) {
            if(e == L";" || e ==L",") break;
            if(e == L"^")
                stackOperators.push(e);
            else if(e == L"+" || e == L"-") {
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
                    bool band = false;
                    //Recuperar valor de memoria
                    for(row &r : this->ts) {
                        if(std::get<1>(r) == e) {
                            stackOperands.push(this->memoria[std::get<2>(r)]);
                            band = true;
                            break;
                        }
                    }
                    std::string lexem_str = wstring_to_string(e);
                    if(!band) throw std::runtime_error("La variable " + lexem_str + " no ha sido declarada.");
                }
            }
        }

        while(!stackOperators.empty()) {
            evaluate_operands(stackOperators, stackOperands);
        }

        std::wcout << L"Expr : ";
        for(std::wstring &e : this->expr) {
            std::wcout << e << L" ";
        }
        std::wcout << std::endl;
        this->expr.clear();
        std::cout << "Result: " << stackOperands.top() << std::endl;

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
            operands.push(op2 + op1);
        } else if(op == L"-") {
            operands.push(op2 - op1);
        } else if(op == L"*") {
            operands.push(op2 * op1);
        } else if(op == L"/") {
            operands.push(op2 / op1);
        } else if(op == L"^") {
            operands.push(std::pow(op2, op1));
        } else if(op == L"%") {
            operands.push(std::fmod(op2, op1));
        }
    }

    void Iparser::my_cout() {
        for(Token_t &e: this->output) {
            if(e.type == TokenType::LITERAL)
                std::wcout << e.lexem;
            else if(e.type == TokenType::IDENTIFIER) {
                bool band = false;
                //Recuperar valor de memoria
                for(row &r : this->ts) {
                    if(std::get<1>(r) == e.lexem) {
                        std::wcout << this->memoria[std::get<2>(r)];
                        band = true;
                        break;
                    }
                }
                std::string lexem_str = wstring_to_string(this->tok.lexem);
                if(!band) throw std::runtime_error("La variable " + lexem_str + "no ha sido declarada.");
            }
        }
        this->output.clear();
        std::wcout << std::endl;
    }


}

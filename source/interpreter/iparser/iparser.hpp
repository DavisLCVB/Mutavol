#ifndef IPARSER_H
#define IPARSER_H

#include <memory>
#include <stack>
#include <cmath>
#include <tuple>
#include <optional>
#include <windows.h>
#include "../source/utils/format/format.hpp"
#include "../../utils/data_structures/types.hpp"
#include "../../core/scanner/scanner.hpp"

// Mutavol namespace
namespace mtv {
    class Iparser {
        std::string p_state;
        State afdVars;
        std::vector<std::wstring> expr;

        using row = std::tuple<std::wstring, std::wstring, int>;
        std::vector<row> ts;
        row temp_declaration;
        int ts_pointer;
        double memoria[100];

        std::vector<Token_t> output;

        public:
            ~Iparser() = default;

            Iparser(const Iparser &) = delete;

            Iparser &operator=(const Iparser &) = delete;

            static Iparser &get_instance();

            static void init_parser();

            void parse();

        private:
            static std::unique_ptr<Iparser> instance;

            Iparser();

            Token_t tok;

            void get_next_token();
            void get_next_expr_value();
            double evaluate_expr();
            static void evaluate_operands(std::stack<std::wstring> &operators, std::stack<double> &operands);

            void get_next_output();
            void my_cout();

            std::optional<double> getvar(const std::wstring &var);

            void evaluate_whit_afd(const State &afd);
            // Parser LL(1) - Expresiones generales
            void Z();
            void A();
            void B();
            void C();
            //Parser LL(1) para salida de consola
            void D();
            void E();
            void K();
            void V();

            // Parser LL(1) - Expresiones aritmeticas
            void W();
            void U();
            void Y();
            void P();
            void O();
            void F();

            void enableColors();
    };
} // namespace mtv

#endif // IPARSER_H
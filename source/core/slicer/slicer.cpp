#include "slicer.hpp"

namespace mtv {
    const std::string slicer::operators = "\"\'#,;(){}[]=*/-+<>&|!~^%?:.";

    slicer* slicer::instance = nullptr;

    slicer& slicer::get_instance(const std::vector<char>& test_buffer) {
        if (!instance)
            instance = new slicer(test_buffer);
        return *instance;
    }

    void slicer::slice() {
        while ((*(this->charcurrent) == ' ' || *(this->charcurrent) == '\n' || *(this->charcurrent) == '\t') && this->charcurrent != this->charend)
        {
            this->charcurrent++;
        }
        if(charconcatenate())
            return;
        if (is_number())
            return;
        if (is_operator())
            return;
    }
    const std::string slicer::getnext_token() {
        if (this->charcurrent == this->charend)
        {
            return "fin";
        }
        this->slice();
        return this->tokens.back();
    }

    bool slicer::charconcatenate() {
        char c = *(this->charcurrent);
        if (c >= 'a' && c <= 'z' || c == '_' || c >= 'A' && c <= 'Z')
        {
            std::string token;
            while ((this->charcurrent) != charend && ((c >= 'a' && c <= 'z') ||
                                              (c >= '0' && c <= '9' || c >= 'A' && c <= 'Z') || c == '_'))
            {
                token.push_back(c);
                (this->charcurrent)++;
                if ((this->charcurrent) == this->charend)
                    break;
                c = *(this->charcurrent);
            }
            tokens.push_back(token);
            return true;
        }
        return false;
    }
    bool slicer::is_number() {
        char c = *(this->charcurrent);
        if (c >= '0' && c <= '9')
        {
            std::string token;
            while ((this->charcurrent) != charend && (c >= '0' && c <= '9') || c == '.')
            {
                token.push_back(c);
                (this->charcurrent)++;
                if ((this->charcurrent) == this->charend)
                    break;
                c = *(this->charcurrent);
            }
            tokens.push_back(token);
            return true;
        }
        return false;
    }
    bool slicer::is_operator() {
        char c = *(this->charcurrent);
        if (operators.find(c) != std::string::npos)
        {
            std::string token;
            token.push_back(c);
            (this->charcurrent)++;
            if((this->charcurrent) != charend){
                std::string double_symbol_token = token;
                double_symbol_token.push_back(*(this->charcurrent));
                case_double_symbols(double_symbol_token, token);
            }
            tokens.push_back(token);
            return true;
        }
        return false;
    }
    void slicer::case_double_symbols(std::string &double_symbol_token, std::string &token) {
        if ((this->charcurrent) == this->charend)
        {
            return;
        }
        if (double_symbol_token == "&&" || double_symbol_token == "||" || double_symbol_token == "==" || double_symbol_token == "!=" ||
            double_symbol_token == "<=" || double_symbol_token == ">=" || double_symbol_token == "++" || double_symbol_token == "--" ||
            double_symbol_token == "+=" || double_symbol_token == "-=" || double_symbol_token == "*=" || double_symbol_token == "/=" ||
            double_symbol_token == "%=" || double_symbol_token == "&=" || double_symbol_token == "|=" || double_symbol_token == "^=" ||
            double_symbol_token == "<<=" || double_symbol_token == ">>=")
        {
            token = double_symbol_token;
            (this->charcurrent)++;
        }
    }

    const std::vector<std::string> slicer::get_tokens() const {
        return tokens;
    }
} // namespace mtv

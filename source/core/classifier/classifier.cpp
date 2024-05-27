#include "classifier.hpp"

#include "../slicer/slicer.hpp"

namespace mtv {
    std::once_flag Classifier::init_instance_flag;
    std::unique_ptr<Classifier> Classifier::instance = nullptr;
    const std::wstring Classifier::IDENTIFIERS = L"(_|[a-zA-Z])(_|[a-zA-Z0-9])*";
    const std::wstring Classifier::OPERATORS =
            LR"((\+)|(-)|(\*)|(/)|(<)|(>)|(=)|(==)|(!=)|(<=)|(>=)|(&&)|(\|\|))";
    const std::wstring Classifier::KEYWORDS =
            L"(si)|(sino)|(mientras)|(para)|(void)|(caracter)|(cadena)|(entero)|(decimal)|(funcion)$";
    const std::wstring Classifier::LITERALS = L"(\"(.)*\")|([0-9]+)";
    const std::wstring Classifier::DELIMITERS =
            LR"((\()|(\))|(\[)|(\])|(do)|(fin_.*)|(,)|(\\n))";

    Classifier &Classifier::get_instance() {
        std::call_once(init_instance_flag, &Classifier::init_instance);
        return *instance;
    }

    void Classifier::init_slicer() {
        auto &slicer = Slicer::get_instance();
        slicer.slice();
    }

    void Classifier::init_instance() {
        instance.reset(new Classifier());
    }

    void Classifier::classify() {
        // Llamado artificial a slicer
        auto &slicer = Slicer::get_instance();
        this->tok = slicer.get_next_token();
        if(this->tok.lexem.empty()) return;
        if (is_keyword())
            return;
        if (is_delimiter())
            return;
        if (is_identifier())
            return;
        if (is_operator())
            return;
        is_literal();
    }

    Token_t Classifier::next_token() {
        classify();
        return this->tok;
    }

    bool Classifier::is_identifier() {
        if (const std::wregex identifier(IDENTIFIERS); std::regex_match(
            this->tok.lexem, identifier)) {
            this->tok.type = TokenType::IDENTIFIER;
            return true;
        }
        return false;
    }

    bool Classifier::is_operator() {
        if (const std::wregex operator_regex(OPERATORS); std::regex_match(
            this->tok.lexem, operator_regex)) {
            this->tok.type = TokenType::OPERATOR;
            return true;
        }
        return false;
    }

    bool Classifier::is_keyword() {
        if (const std::wregex keyword(KEYWORDS);
            std::regex_match(this->tok.lexem, keyword)) {
            this->tok.type = TokenType::KEYWORD;
            return true;
        }
        return false;
    }

    bool Classifier::is_literal() {
        if (const std::wregex literal(LITERALS);
            std::regex_match(this->tok.lexem, literal)) {
            this->tok.type = TokenType::LITERAL;
            return true;
        }
        return false;
    }

    bool Classifier::is_delimiter() {
        if (const std::wregex delimiter(DELIMITERS); std::regex_match(
            this->tok.lexem, delimiter)) {
            this->tok.type = TokenType::DELIMITER;
            return true;
        }
        return false;
    }
}

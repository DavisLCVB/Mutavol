#include "classifier.hpp"

#include "../slicer/slicer.hpp"

namespace mtv
{
    std::unique_ptr<Scanner::Classifier> Scanner::Classifier::instance = nullptr;
    const std::wstring Scanner::Classifier::IDENTIFIERS = L"(_|[a-zA-Z])(_|[a-zA-Z0-9])*";
    const std::wstring Scanner::Classifier::OPERATORS =
        LR"((\+)|(-)|(\*)|(/)|(<)|(>)|(=)|(==)|(!=)|(<=)|(>=)|(&&)|(\|\|))";
    const std::wstring Scanner::Classifier::KEYWORDS =
        L"(si)|(sino)|(mientras)|(para)$";
    const std::wstring Scanner::Classifier::DTYPES =
        L"(void)|(caracter)|(cadena)|(entero)|(decimal)$";
    const std::wstring Scanner::Classifier::LITERALS = L"(\"(.)*\")|([0-9]+)";
    const std::wstring Scanner::Classifier::DELIMITERS =
        LR"((\()|(\))|(\[)|(\])|(do)|(fin_.*)|(,)|(\n))";

    Scanner::Classifier &Scanner::Classifier::get_instance()
    {
        if (instance == nullptr)
        {
            init_instance();
        }
        return *instance;
    }

    void Scanner::Classifier::init_slicer()
    {
        auto &slicer = Slicer::get_instance();
        slicer.slice();
    }

    void Scanner::Classifier::init_instance()
    {
        instance.reset(new Classifier());
    }

    void Scanner::Classifier::classify()
    {
        // Llamado artificial a slicer
        auto &slicer = Slicer::get_instance();
        this->tok = slicer.get_next_token();
        if (this->tok.lexem.empty())
        {
            this->tok.type = TokenType::DELIMITER;
            this->tok.lexem = L"$";
            return;
        }

        if (is_datatype())
            return;
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

    Token_t Scanner::Classifier::next_token()
    {
        classify();
        return this->tok;
    }

    bool Scanner::Classifier::is_identifier()
    {
        if (const std::wregex identifier(IDENTIFIERS); std::regex_match(
                this->tok.lexem, identifier))
        {
            this->tok.type = TokenType::IDENTIFIER;
            return true;
        }
        return false;
    }

    bool Scanner::Classifier::is_operator()
    {
        if (const std::wregex operator_regex(OPERATORS); std::regex_match(
                this->tok.lexem, operator_regex))
        {
            this->tok.type = TokenType::OPERATOR;
            return true;
        }
        return false;
    }

    bool Scanner::Classifier::is_datatype()
    {
        if (const std::wregex dtypes_regex(DTYPES);
            std::regex_match(this->tok.lexem, dtypes_regex))
        {
            this->tok.type = TokenType::DTYPE;
            return true;
        }
        return false;
    }

    bool Scanner::Classifier::is_keyword()
    {
        if (const std::wregex keyword(KEYWORDS);
            std::regex_match(this->tok.lexem, keyword))
        {
            this->tok.type = TokenType::KEYWORD;
            return true;
        }
        return false;
    }

    bool Scanner::Classifier::is_literal()
    {
        if (const std::wregex literal(LITERALS);
            std::regex_match(this->tok.lexem, literal))
        {
            this->tok.type = TokenType::LITERAL;
            return true;
        }
        return false;
    }

    bool Scanner::Classifier::is_delimiter()
    {
        if (const std::wregex delimiter(DELIMITERS); std::regex_match(
                this->tok.lexem, delimiter))
        {
            this->tok.type = TokenType::DELIMITER;
            return true;
        }
        return false;
    }
} // namespace mtv

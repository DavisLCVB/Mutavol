#include "classifier.hpp"

#include "../slicer/slicer.hpp"

namespace mtv
{
    std::unique_ptr<Scanner::Classifier> Scanner::Classifier::instance = nullptr;
    const std::wstring Scanner::Classifier::IDENTIFIERS = L"(_|[a-zA-Z])(_|[a-zA-Z0-9])*";
    const std::wstring Scanner::Classifier::OPERATORSCOMP =
        LR"((<)|(>)|(==)|(!=)|(<=)|(>=)|(&&)|(\|\|))";
    const std::wstring Scanner::Classifier::OPERATORSEXP =
        LR"((\+)|(-)|(\*)|(/)|(=)|(\^))";
    const std::wstring Scanner::Classifier::OPERATORSDOBLE =
        LR"((\+=)|(\+\+)|(--)|(-=)|(\*=)|(/=)|(=)|(\^=))";
    const std::wstring Scanner::Classifier::KEYWORDS =
        L"(if)|(else)|(while)|(for)$";
    const std::wstring Scanner::Classifier::DTYPES =
        L"(void)|(char)|(string)|(int)|(float)|(bool)$";
    const std::wstring Scanner::Classifier::LITERALS = L"(\"(.)*\")|([0-9]+)";
    const std::wstring Scanner::Classifier::DELIMITERS =
        LR"((\()|(\))|(\[)|(\])|(\{)|(\})|(,)|(;))";

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
        auto &slicer = Slicer::get_instance();
        this->tok = slicer.get_next_token();
        if (this->tok.lexem.empty())
        {
            //FINAL DEL ARCHIVO
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
        if (is_operator_comp())
            return;
        if (is_operator_exp())
            return;
        if (is_operator_dob())
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

    bool Scanner::Classifier::is_operator_comp()
    {
        if (const std::wregex opcomp_regex(OPERATORSCOMP); std::regex_match(
                this->tok.lexem, opcomp_regex))
        {
            this->tok.type = TokenType::OPERATORCOMP;
            return true;
        }
        return false;
    }

    bool Scanner::Classifier::is_operator_exp()
    {
        if (const std::wregex opexp_regex(OPERATORSEXP); std::regex_match(
                this->tok.lexem, opexp_regex))
        {
            this->tok.type = TokenType::OPERATOREXP;
            return true;
        }
        return false;
    }

    bool Scanner::Classifier::is_operator_dob()
    {
        if (const std::wregex opdob_regex(OPERATORSDOBLE); std::regex_match(
                this->tok.lexem, opdob_regex))
        {
            this->tok.type = TokenType::OPERATORDOBLE;
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

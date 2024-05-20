#include "classifier.hpp"

namespace classifier
{
    std::once_flag Classifier::init_instance_flag;
    std::unique_ptr<Classifier> Classifier::instance = nullptr;
    const std::string Classifier::IDENTIFIERS = "(_|[a-zA-Z])(_|[a-zA-Z0-9])*";
    const std::string Classifier::OPERATORS = "(\\+)|(-)|(\\*)|(/)|(<)|(>)|(=)|(==)|(!=)|(<=)|(>=)|(&&)|(\\|\\|)";
    const std::string Classifier::KEYWORDS = "(si)|(sino)|(mientras)|(para)|(void)|(caracter)|(cadena)|(entero)|(decimal)|(funcion)$";
    const std::string Classifier::LITERALS = "(\"(.)*\")|([0-9]+)";
    const std::string Classifier::DELIMITERS = "(\\()|(\\))|(\\[)|(\\])|(do)|(fin_.*)|(,)|(\\\\n)";

    const std::vector<std::string> Classifier::TEST_STRINGS = {
        "entero",
        "a",
        "=",
        "5",
        ";",
        "{",
        "}",
        "&",
        "B",
        "(",
        ")",
        "si",
        "sino",
        "mientras",
        "para",
        "void",
        "main",
        "char",
        "float",
        "double",
        "[",
        "]",
        "\\n",
        "&",
        "<=",
        "fin_mientras",
        "fin_aya"};
    Classifier &Classifier::get_instance()
    {
        std::call_once(init_instance_flag, &Classifier::init_instance);
        return *instance;
    }

    void Classifier::init_instance()
    {
        instance.reset(new Classifier());
    }

    void Classifier::classify()
    {
        // Llamado artificial a slicer
        this->tok = Token_t({this->TEST_STRINGS[this->test_string_index], TokenType::UNIDENTIFIED});
        this->test_string_index++;

        if (is_keyword())
            return;
        if (is_delimiter())
            return;
        if (is_identifier())
            return;
        if (is_operator())
            return;
        if (is_literal())
            return;
    }

    const Token_t Classifier::next_token()
    {
        this->classify();
        return this->tok;
    }

    bool Classifier::is_identifier()
    {
        std::regex identifier(this->IDENTIFIERS);
        if (std::regex_match(this->tok.lexem, identifier))
        {
            this->tok.type = TokenType::IDENTIFIER;
            return true;
        }
        return false;
    }

    bool Classifier::is_operator()
    {
        std::regex operator_regex(this->OPERATORS);
        if (std::regex_match(this->tok.lexem, operator_regex))
        {
            this->tok.type = TokenType::OPERATOR;
            return true;
        }
        return false;
    }

    bool Classifier::is_keyword()
    {
        std::regex keyword(this->KEYWORDS);
        if (std::regex_match(this->tok.lexem, keyword))
        {
            this->tok.type = TokenType::KEYWORD;
            return true;
        }
        return false;
    }

    bool Classifier::is_literal()
    {
        std::regex literal(this->LITERALS);
        if (std::regex_match(this->tok.lexem, literal))
        {
            this->tok.type = TokenType::LITERAL;
            return true;
        }
        return false;
    }

    bool Classifier::is_delimiter()
    {
        std::regex delimiter(this->DELIMITERS);
        if (std::regex_match(this->tok.lexem, delimiter))
        {
            this->tok.type = TokenType::DELIMITER;
            return true;
        }
        return false;
    }
}
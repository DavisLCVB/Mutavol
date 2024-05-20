#include "classifier.hpp"
#include "../slicer/slicer.hpp"

namespace mia_kalinux
{
    std::once_flag Classifier::init_instance_flag;
    std::unique_ptr<Classifier> Classifier::instance = nullptr;
    const std::string Classifier::IDENTIFIERS = "(_|[a-zA-Z])(_|[a-zA-Z0-9])*";
    const std::string Classifier::OPERATORS = "(\\+)|(-)|(\\*)|(/)|(<)|(>)|(=)|(==)|(!=)|(<=)|(>=)|(&&)|(\\|\\|)";
    const std::string Classifier::KEYWORDS = "(si)|(sino)|(mientras)|(para)|(void)|(caracter)|(cadena)|(entero)|(decimal)|(funcion)$";
    const std::string Classifier::LITERALS = "(\"(.)*\")|([0-9]+)";
    const std::string Classifier::DELIMITERS = "(\\()|(\\))|(\\[)|(\\])|(do)|(fin_.*)|(,)|(\\\\n)";


    std::string str = "entero a = 12;\nsi (a = 12){}\nsino{ float b = 0.1; \n far.get();}";

    // false buffer "se obtendrá de clase buffer"

    //std::vector<char> test_buffer(str.begin(), str.end());

    std::vector<char> test_buffer = {'i', 'n', 't', ' ', 'm', 'a', 'i', 'n', '(', 'c', 'o', 'n', 's', 't', ' ',
                                     'i', 'n', 't', ' ', 'a', 'r', 'g', 'c', ',', ' ', 'c', 'h', 'a', 'r', ' ',
                                     '*', 'a', 'r', 'g', 'v', '[', ']', ')', '{', '\n', ' ', ' ', 'm', 't', 'v',
                                     ':', ':', 'A', 'r', 'g', 's', ' ', '&', 'a', 'r', 'g', 's', ' ', '=', ' ',
                                     'm', 't', 'v', ':', ':', 'A', 'r', 'g', 's', ':', ':', 'g', 'e', 't', '_',
                                     'i', 'n', 's', 't', 'a', 'n', 'c', 'e', '(', 'a', 'r', 'g', 'c', ',', ' ',
                                     'a', 'r', 'g', 'v', ')', ';', '\n', ' ', ' ', 'a', 'r', 'g', 's', '.', 'r',
                                     'e', 'c', 'o', 'n', '(', ')', ';', '\n', ' ', ' ', 'i', 'f', ' ', '(', 'a',
                                     'r', 'g', 's', '.', 's', 'h', 'o', 'u', 'l', 'd', '_', 'e', 'x', 'i', 't', '(',
                                     ')', ')', ' ', '{', '\n', ' ', ' ', ' ', ' ', 'r', 'e', 't', 'u', 'r', 'n',
                                     ' ', '0', ';', '\n', ' ', ' ', '}', '\n', ' ', ' ', 'a', 'u', 't', 'o', ' ',
                                     'o', 'u', 't', 'p', 'u', 't', '_', 'f', 'i', 'l', 'e', ' ', '=', ' ', 'a',
                                     'r', 'g', 's', '.', 'g', 'e', 't', '_', 'o', 'u', 't', 'p', 'u','t'};

    mtv::slicer &slicer = mtv::slicer::get_instance(test_buffer);

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
        // Llamado al slicer
        this->tok = Token_t({slicer.getnext_token(), TokenType::UNIDENTIFIED});
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

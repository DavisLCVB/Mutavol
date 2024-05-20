#include "../source/core/classifier/classifier.hpp"
#include "../source/core/slicer/slicer.hpp"
#include <iostream>
std::string enumToString(TokenType type);

int main(const int argc, char *argv[])
{

    mia_kalinux::Classifier &classifier = mia_kalinux::Classifier::get_instance();
    Token_t tok;
    for (int i = 0; i < 27; i++)
    {
        tok = classifier.next_token();
        std::cout << "Token: " << tok.lexem << " - " << enumToString(tok.type) << std::endl;
    }
    return 0;
}

std::string enumToString(TokenType type)
{
    switch (type)
    {
        case TokenType::IDENTIFIER:
            return "IDENTIFIER";
        case TokenType::OPERATOR:
            return "OPERATOR";
        case TokenType::DELIMITER:
            return "DELIMITER";
        case TokenType::KEYWORD:
            return "KEYWORD";
        case TokenType::LITERAL:
            return "LITERAL";
        case TokenType::UNIDENTIFIED:
            return "UNIDENTIFIED";
    }

    return "UNIDENTIFIED";
}
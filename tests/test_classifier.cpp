#include "../source/core/classifier/classifier.hpp"
#include <iostream>
std::string enumToString(TokenType type);

int test_classifier(const int argc, char *argv[])
{
    mtv::Classifier &classifier = mtv::Classifier::get_instance();
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
#include "../source/core/classifier/classifier.hpp"
#include "../source/utilities/data_structures/types.hpp"
#include <iostream>
std::string enumToString(mtv::TokenType type);

int test_classifier(const int argc, char *argv[])
{
    mtv::Classifier &classifier = mtv::Classifier::get_instance();
    mtv::Token_t tok;
    for (int i = 0; i < 27; i++)
    {
        tok = classifier.next_token();
        std::cout << "Token: " << tok.lexem << " - " << enumToString(tok.type) << std::endl;
    }
    return 0;
}

std::string enumToString(mtv::TokenType type)
{
    switch (type)
    {
    case mtv::TokenType::IDENTIFIER:
        return "IDENTIFIER";
    case mtv::TokenType::OPERATOR:
        return "OPERATOR";
    case mtv::TokenType::DELIMITER:
        return "DELIMITER";
    case mtv::TokenType::KEYWORD:
        return "KEYWORD";
    case mtv::TokenType::LITERAL:
        return "LITERAL";
    case mtv::TokenType::UNIDENTIFIED:
        return "UNIDENTIFIED";
    }

    return "UNIDENTIFIED";
}
#include "parser.hpp"

namespace mtv
{
    std::unique_ptr<Parser> Parser::instance = nullptr;

    Parser() : ()
    {
        // Inicializacion del parser de variables
        this->afdVars[L"q0"][L"DTYPE"] = L"q1";
        this->afdVars[L"q1"][L"IDENTIFIER"] = L"q2";
        this->afdVars[L"q2"][L";"] = L"qf";
        this->afdVars[L"q2"][L"="] = L"q3";
        this->afdVars[L"q2"][L","] = L"q1";
        this->afdVars[L"q3"][L"IDENTIFIER"] = L"q4";
        this->afdVars[L"q4"][L";"] = L"qf";
        this->afdVars[L"q4"][L","] = L"q1";
    }

    Parser &Parser::get_instance()
    {
        if (instance == nullptr)
        {
            init_instance();
        }
        return *instance;
    }

    void Parser::init_instance()
    {
        instance.reset(new Parser());
    }

    void Parser::parse()
    {
        /* while (true)
        {
            const auto [lexem, type, pos] = mtv::Scanner::get();
            if (lexem.empty())
                break;
            std::wcout << lexem << " - " << type_str(type) << L'\n'; */

        for (const auto &state : this->afdVars)
        {
            std::wcout << L"State: " << state.first << std::endl;
            for (const auto &transition : state.second)
            {
                std::wcout << L"  Symbol: " << transition.first
                           << L" -> Next State: " << transition.second << std::endl;
            }
        }
        //}
        // Llamado a classifier
    }
} // namespace mtv
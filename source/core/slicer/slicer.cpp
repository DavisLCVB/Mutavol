#include "slicer.hpp"

namespace mtv
{
    const std::string slicer::operators = "\"\'#,;(){}[]=*/-+<>&|!~^%?:.";

    slicer *slicer::instance = nullptr;

    slicer &slicer::get_instance(mtv::Buffer<mtv::LinkedList<std::pair<wchar_t,mtv::Position>>>& bufferInstance)
    {
        if (!instance)
            instance = new slicer(bufferInstance);
        return *instance;
    }
    slicer &slicer::get_instance()
    {
        if (!instance)
            throw std::logic_error("slicer instance is not created yet.");
        return *instance;
    }

    void slicer::slice()
    {
        for (auto &ll: *buffer) {
            size_t index{0};
            auto it = ll.begin();
            auto charend = ll.end();
            while (it != charend) {
                while ((it->first == L' '|| it->first == L'\t') && it != charend) {
                    ++it;
                }
                if (it == charend) {
                    break;
                }
                if (!charconcatenate(it, charend) && !is_number(it, charend) && !is_operator(it, charend)) {
                    ++it;
                }
            }
        }
    }

    bool slicer::charconcatenate(LinkedList<std::pair<wchar_t,mtv::Position>>::Iterator& it,
                                 LinkedList<std::pair<wchar_t,mtv::Position>>::Iterator& charend)
    {
        wchar_t c = it->first;
        if (c >= L'a' && c <= L'z' || c == L'_' || c >= L'A' && c <= L'Z') {
            std::wstring token;
            while (it != charend && ((c >= L'a' && c <= L'z') ||
            (c >= L'0' && c <= L'9' || c >= L'A' && c <= L'Z') || c == L'_')) {
                token.push_back(c);
                ++it;
                if (it == charend)
                    break;
                c = it->first;
            }
            tokens.push_back(token);
            return true;
        }
        return false;
    }

    bool slicer::is_number(LinkedList<std::pair<wchar_t,mtv::Position>>::Iterator& it,
                           LinkedList<std::pair<wchar_t,mtv::Position>>::Iterator& charend)
    {
        wchar_t c = it->first;
        if (c >= L'0' && c <= L'9') {
            std::wstring token;
            while (it != charend && ((c >= L'0' && c <= L'9') || c == L'.')) {
                token.push_back(c);
                ++it;
                if (it == charend)
                    break;
                c = it->first;
            }
            tokens.push_back(token);
            return true;
        }
        return false;
    }

    bool slicer::is_operator(LinkedList<std::pair<wchar_t,mtv::Position>>::Iterator& it,
                             LinkedList<std::pair<wchar_t,mtv::Position>>::Iterator& charend)
    {
        wchar_t c0 = it->first;
        char c1 = static_cast<char>(c0);
        if (operators.find(c1) != std::string::npos) {
            std::wstring token;
            token.push_back(c0);
            ++it;
            if (it != charend) {
                std::wstring double_symbol_token = token;
                double_symbol_token.push_back(it->first);
                case_double_symbols(double_symbol_token, token, it, charend);
            }
            tokens.push_back(token);
            return true;
        }
        return false;
    }

    void slicer::case_double_symbols(std::wstring &double_symbol_token, std::wstring &token,
                                     LinkedList<std::pair<wchar_t,mtv::Position>>::Iterator& it,
                                     LinkedList<std::pair<wchar_t,mtv::Position>>::Iterator& charend)
    {
        if ( it == charend)
        {
            return;
        }
        if (double_symbol_token == L"&&" || double_symbol_token == L"||" || double_symbol_token == L"==" ||
            double_symbol_token == L"!=" || double_symbol_token == L"<=" || double_symbol_token == L">=" ||
            double_symbol_token == L"++" || double_symbol_token == L"--" || double_symbol_token == L"+=" ||
            double_symbol_token == L"-=" || double_symbol_token == L"*=" || double_symbol_token == L"/=" ||
            double_symbol_token == L"%=" || double_symbol_token == L"&=" || double_symbol_token == L"|=" ||
            double_symbol_token == L"^=" || double_symbol_token == L"<<=" || double_symbol_token == L">>=")
        {
            token = double_symbol_token;
            ++it;
        }
    }

    const std::vector<std::wstring> slicer::get_tokens() const
    {
        return tokens;
    }
} // namespace mtv

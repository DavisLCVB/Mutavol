#include "slicer.hpp"

namespace mtv {
    const std::string Slicer::operators = "\"\'#,;(){}[]=*/-+<>&|!~^%?:.";

    std::unique_ptr<Slicer> Slicer::instance = nullptr;

    LinkedList<std::wstring> Slicer::double_symbols = {
        L"&&", L"||", L"==", L"!=",
        L"<=", L">=", L"++", L"--",
        L"+=", L"-=", L"*=", L"/=",
        L"%=", L"&=", L"|=", L"^=",
        L"<<=", L">>="
    };

    void Slicer::init_instance() {
        instance.reset(new Slicer());
    }

    Slicer &Slicer::get_instance() {
        if (instance == nullptr) {
            instance.reset(new Slicer());
        }
        return *instance;
    }


    void Slicer::slice() {
        for (auto &buff = Buffer<LinkedList<std::pair<wchar_t,
                 Position> > >::get_instance(); auto &ll: buff) {
            auto it = ll.begin();
            auto charend = ll.end();
            while (it != charend) {
                while ((it->first == L' ' || it->first == L'\t') && it != charend) {
                    ++it;
                }
                if (it == charend) {
                    break;
                }
                if (!char_concatenate(it, charend) && !is_number(it, charend) && !
                    is_operator(it, charend)) {
                    ++it;
                }
            }
        }
    }

    bool Slicer::char_concatenate(LL_Iterator &it, const LL_Iterator &charend) {
        if (wchar_t c = it->first; c >= L'a' && c <= L'z' || c == L'_' || c >= L'A' && c
                                   <= L'Z') {
            std::wstring token;
            while (it != charend && ((c >= L'a' && c <= L'z') ||
                                     (c >= L'0' && c <= L'9' || c >= L'A' && c <= L'Z') ||
                                     c == L'_')) {
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

    bool Slicer::is_number(LL_Iterator &it, const LL_Iterator &charend) {
        if (wchar_t c = it->first; c >= L'0' && c <= L'9') {
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

    bool Slicer::is_operator(LL_Iterator &it, const LL_Iterator &charend) {
        const wchar_t c0 = it->first;
        if (const char c1 = static_cast<char>(c0);
            operators.find(c1) != std::string::npos) {
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

    void Slicer::case_double_symbols(const std::wstring &double_symbol_token,
                                     std::wstring &token, LL_Iterator &it,
                                     const LL_Iterator &charend) {
        if (it == charend) {
            return;
        }
        if (double_symbols.contains(double_symbol_token)) {
            token = double_symbol_token;
            ++it;
        }
    }

    std::vector<std::wstring> Slicer::get_tokens() const {
        return tokens;
    }

    std::wstring Slicer::get_next_token() {
        if (tokens.empty()) {
            return L"";
        }
        std::wstring token = tokens.front();
        tokens.erase(tokens.begin());
        return token;
    }
} // namespace mtv

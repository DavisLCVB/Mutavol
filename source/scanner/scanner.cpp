#include "scanner.hpp"

namespace mtv {
void Scanner::pre_tokenize() {
    wchar_t prev = L'\0';
    std::wstring token;
    for (auto c : buffer) {
        if (is_blank(c)) {
            if (!token.empty()) {
                pre_tokens.push_back(token);
                token.clear();
            }
            continue;
        }
        if (is_alpha(c) || is_number(c) || c == L'_') {
            token += c;
            continue;
        }
        if (is_delimiter(c)) {
            if (!token.empty()) {
                pre_tokens.push_back(token);
                token.clear();
            }
            pre_tokens.push_back(std::wstring(1, c));
            continue;
        }
        if (is_operator(c)) {
            if (!token.empty()) {
                pre_tokens.push_back(token);
                token.clear();
            }
            if (c == L'=' && prev == L'=') {
                pre_tokens.pop_back();
                pre_tokens.push_back(L"==");
            } else if (c == L'!' && prev == L'=') {
                pre_tokens.pop_back();
                pre_tokens.push_back(L"!=");
            } else if (c == L'<' && prev == L'=') {
                pre_tokens.pop_back();
                pre_tokens.push_back(L"<=");
            } else if (c == L'>' && prev == L'=') {
                pre_tokens.pop_back();
                pre_tokens.push_back(L">=");
            } else {
                pre_tokens.push_back(std::wstring(1, c));
            }
            prev = c;
            continue;
        }
    }
}

bool Scanner::is_blank(wchar_t c) {
    return c == L' ' || c == L'\n' || c == L'\t';
}

bool Scanner::is_alpha(wchar_t c) {
    return (c >= L'a' && c <= L'z') || (c >= L'A' && c <= L'Z');
}

bool Scanner::is_delimiter(wchar_t c) {
    return c == L'(' || c == L')' || c == L'{' || c == L'}' || c == L'[' ||
           c == L']' || c == L'<' || c == L'>';
}

bool Scanner::is_operator(wchar_t c) {
    return c == L'+' || c == L'-' || c == L'*' || c == L'/' || c == L'%' ||
           c == L'=' || c == L'!' || c == L'<' || c == L'>' || c == L'&' ||
           c == L'|' || c == L'^' || c == L'~';
}

bool Scanner::is_number(wchar_t c) { return c >= L'0' && c <= L'9'; }
void Scanner::scan() { pre_tokenize(); }
} // namespace mtv
#pragma once
#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <vector>
#include "../structures/structures.hpp"

namespace mtv {
class Scanner {
  public:
    Scanner()  =default;
    Scanner(const std::vector<wchar_t>& buffer): buffer(buffer) {};
    void scan();
    vec_token_t get_tokens() const { return tokens; }
    vec_wstring_t get_pre_tokens() const { return pre_tokens; }
  private:
    std::vector<wchar_t> buffer;
    vec_token_t tokens;
    vec_wstring_t pre_tokens;
    void pre_tokenize();
    bool is_blank(wchar_t c);
    bool is_alpha(wchar_t c);
    bool is_delimiter(wchar_t c);
    bool is_operator(wchar_t c);
    bool is_number(wchar_t c);
};
} // namespace mtv

#endif // SCANNER_HPP
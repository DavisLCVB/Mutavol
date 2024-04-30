#pragma once
#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <vector>

namespace mtv {
class Scanner {
  public:
    Scanner()  =default;
    Scanner(const std::vector<wchar_t>& buffer): buffer(buffer) {};
    wchar_t scan();
  private:
    std::vector<wchar_t> buffer;
    size_t index{0};
    size_t size{0};
};
} // namespace mtv

#endif // SCANNER_HPP
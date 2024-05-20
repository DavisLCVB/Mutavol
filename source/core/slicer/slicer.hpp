#ifndef Slicer_HPP
#define Slicer_HPP

#include <string>
#include <iostream>
#include <vector>
#include <memory>

namespace mtv {
    class slicer{
    private:
        static slicer* instance;
        std::vector<char> test_buffer;
        std::vector<char>::iterator charcurrent;
        std::vector<char>::iterator charend;
        slicer(const std::vector<char> &test_buffer) : test_buffer(test_buffer) {
            charcurrent = this->test_buffer.begin();
            charend = this->test_buffer.end();
        }
        std::vector<std::string> tokens;

    public:
        slicer(slicer const&) = delete;
        slicer() = delete;
        static slicer &get_instance(const std::vector<char>& test_buffer);

        static const std::string operators;

        void slice();
        const std::string getnext_token();
        bool charconcatenate();
        bool is_number();
        bool is_operator();
        void case_double_symbols(std::string &double_symbol_token, std::string &token);
        const std::vector<std::string> get_tokens() const;

    };

} // namespace mtv
#endif // Slicer_HPP
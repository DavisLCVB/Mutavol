#include "scanner.hpp"

#include "../buffer/buffer.hpp"
#include "classifier/classifier.hpp"
#include "reader/reader.hpp"

namespace mtv {
    void Scanner::init_scanner(const std::string &input_file) noexcept(false) {
        if (const auto &reader = Reader::get_instance(input_file); !reader.read()) {
            throw std::runtime_error("Error reading file");
        }
        auto &classifier = Classifier::get_instance();
        Classifier::init_slicer();
        auto &buff_tok = Buffer<Token_t>::get_instance();
        buff_tok.clear();
        while(true) {
            auto tok = classifier.next_token();
            buff_tok.push(tok);
            if(tok.lexem == L"$") break;
        }
    }

    void Scanner::init_scanner() noexcept(false) {
        std::wstring line;
        std::getline(std::wcin, line);
        if (!Reader::read_cin(line)) {
            throw std::runtime_error("Error reading input");
        }
        auto &classifier = Classifier::get_instance();
        Classifier::init_slicer();
        //PARA EL INTERPRETE
        Slicer::clean_own_buff();

        auto &buff_tok = Buffer<Token_t>::get_instance();
        buff_tok.clear();
        while(true) {
            auto tok = classifier.next_token();
            buff_tok.push(tok);
            if(tok.lexem == L"$") break;
        }
    }

    Token_t Scanner::get() {
        auto &buff = Buffer<Token_t>::get_instance();
        auto tok = buff[0];
        buff.pop(0);
        return tok;
    }
} // namespace mtv

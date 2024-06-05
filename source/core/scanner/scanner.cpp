#include "scanner.hpp"

#include "../buffer/buffer.hpp"
#include "classifier/classifier.hpp"
#include "reader/reader.hpp"

namespace mtv {
    void Scanner::init_scanner(const std::string &input_file) noexcept(false) {
        if (const auto &reader = Reader::get_instance(input_file); !reader.read()) {
            throw std::runtime_error("Error reading file");
        }
        Classifier::init_slicer();
    }

    Token_t Scanner::get() {
        auto &classifier = Classifier::get_instance();
        return classifier.next_token();
    }
} // namespace mtv

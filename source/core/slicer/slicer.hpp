#ifndef Slicer_HPP
#define Slicer_HPP

#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include "../buffer/buffer.hpp"
#include "../../utilities/data_structures/position.hpp"

namespace mtv {
    class slicer{
    private:
        static slicer* instance;
        mtv::Buffer<mtv::LinkedList<std::pair<wchar_t,mtv::Position>>>* buffer;
        std::vector<std::wstring> tokens;

        // Constructor privado
        explicit slicer(mtv::Buffer<mtv::LinkedList<std::pair<wchar_t,mtv::Position>>>& bufferInstance)
            : buffer(&bufferInstance) {}

    public:
        slicer(slicer const&) = delete;
        slicer() = delete;

        static slicer &get_instance(mtv::Buffer<mtv::LinkedList<std::pair<wchar_t,mtv::Position>>>& bufferInstance);

        static slicer &get_instance();

        static const std::string operators;

        void slice();

        const std::wstring getnext_token();

        bool charconcatenate(LinkedList<std::pair<wchar_t,mtv::Position>>::Iterator& it,
                             LinkedList<std::pair<wchar_t,mtv::Position>>::Iterator& charend);

        bool is_number(LinkedList<std::pair<wchar_t,mtv::Position>>::Iterator& it,
                       LinkedList<std::pair<wchar_t,mtv::Position>>::Iterator& charend);

        bool is_operator(LinkedList<std::pair<wchar_t,mtv::Position>>::Iterator& it,
                         LinkedList<std::pair<wchar_t,mtv::Position>>::Iterator& charend);

        void case_double_symbols(std::wstring &double_symbol_token, std::wstring &token,
                                 LinkedList<std::pair<wchar_t,mtv::Position>>::Iterator& it,
                                 LinkedList<std::pair<wchar_t,mtv::Position>>::Iterator& charend);

        const std::vector<std::wstring> get_tokens() const;
    };
} // namespace mtv
#endif // Slicer_HPP
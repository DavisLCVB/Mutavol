#ifndef Slicer_HPP
#define Slicer_HPP

#include <string>
#include <vector>
#include <memory>
#include "../buffer/buffer.hpp"
#include "../../utilities/data_structures/position.hpp"
#include "../../utilities/data_structures/types.hpp"

namespace mtv {
    class Slicer {
    public:
        using LL_Iterator = LinkedList<std::pair<wchar_t, Position> >::Iterator;

        Slicer(Slicer const &) = delete;

        Slicer &operator=(const Slicer &) = delete;

        static Slicer &get_instance();

        void slice();

        Token_t get_next_token();

        [[nodiscard]]
        LinkedList<Token_t>  get_tokens() const;

    private:
        static std::unique_ptr<Slicer> instance;
        LinkedList<Token_t> tokens;
        static const std::string operators;
        static LinkedList<std::wstring> double_symbols;

        Slicer() = default;

        static void init_instance();

        bool char_concatenate(LL_Iterator &it, const LL_Iterator &charend);

        bool is_number(LL_Iterator &it, const LL_Iterator &charend);

        bool is_operator(LL_Iterator &it, const LL_Iterator &charend);

        static void case_double_symbols(const std::wstring &double_symbol_token,
                                        std::wstring &token, LL_Iterator &it,
                                        const LL_Iterator &charend);
    };
} // namespace mtv
#endif // Slicer_HPP

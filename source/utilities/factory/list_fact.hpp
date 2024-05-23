#ifndef LIST_FACT_HPP
#define LIST_FACT_HPP
#include "../data_structures/linked_list.hpp"

namespace mtv {
    class ListFactory {
    public:
        template<typename T>
        static LinkedList<T> *create_linked_list() {
            return new LinkedList<T>();
        }
    };
} // namespace mtv

#endif //LIST_FACT_HPP

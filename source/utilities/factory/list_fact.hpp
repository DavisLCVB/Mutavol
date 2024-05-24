#ifndef LIST_FACT_HPP
#define LIST_FACT_HPP

/**
* @file list_fact.hpp
* @author Davis Cartagena
* @brief Factory class to create linked lists
*/

//Include section
#include "../data_structures/linked_list.hpp"

// Mutavol namespace
namespace mtv {
    /**
     * @class ListFactory
     * @brief Class to create linked lists
     */
    class ListFactory {
    public:
        /**
         * @brief Method to create a simple linked list
         * @tparam T Type of the data to store
         * @return A pointer to the linked list
         */
        template<typename T>
        static LinkedList<T> *create_linked_list() {
            return new LinkedList<T>();
        }
    };
} // namespace mtv

#endif //LIST_FACT_HPP

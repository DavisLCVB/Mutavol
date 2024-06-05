#ifndef NODE_FACT_HPP
#define NODE_FACT_HPP

/**
 * @file node_fact.hpp
 * @author Davis Cartagena
 * @brief Factory class to create nodes
 */

// Include section
#include "../data_structures/nodes.hpp"

// Mutavol namespace
namespace mtv {
    /**
     * @class NodeFactory
     * @brief Factory class to create nodes
     */
    class NodeFactory {
    public:
        /**
         * @brief Method to create a simple node
         * @tparam T Type of the data to store
         * @param data The data to store in the node
         * @return A pointer to the created node
         */
        template<typename T>
        static Node<T> *create_simple_node(const T &data) {
            return new Node<T>(data);
        }
    };
} // namespace mtv

#endif //NODE_FACT_HPP

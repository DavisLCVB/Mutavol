#ifndef NODE_FACT_HPP
#define NODE_FACT_HPP
#include <memory>

#include "../data_structures/nodes.hpp"

namespace mtv {
    class NodeFactory {
    public:
        template<typename T>
        static Node<T> *create_simple_node(const T &data) {
            return new Node<T>(data);
        }

        template<typename T>
        static DoubleNode<T> *create_double_node(const T &data) {
            return new DoubleNode<T>(data);
        }
    };
} // namespace mtv

#endif //NODE_FACT_HPP

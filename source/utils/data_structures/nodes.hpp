#ifndef NODES_HPP
#define NODES_HPP

/**
 * @file nodes.hpp
 * @author Davis Cartagena
 * @brief Collection of nodes for linked lists
 */

// Mutavol namespace
namespace mtv {
    /**
     * @class Node
     * @brief Node class for linked lists
     * @tparam T Type of the data to store
     */
    template<typename T>
    class Node {
    public:
        /**
         * @brief Constructor for the Node class
         * @param _data The data to store in the node
         */
        explicit Node(const T &_data) : data(_data), next(nullptr) {}
        /**
         * @brief Method to set the data of the node
         * @param _data the data to store in the node
         */
        virtual void set_data(T _data) { this->data = _data; }
        /**
         * @brief Method to get a reference to the data of the node
         * @return The data of the node
         */
        virtual T &get_data() { return data; }
        /**
         * @brief Method to get a constant reference to the data of the node
         * @return The data of the node
         */
        virtual const T &get_data() const { return data; }
        /**
         * @brief Method to set the next node
         * @param next The next node
         */
        virtual void set_next(Node *next) { this->next = next; }
        /**
         * @brief Method to get a pointer to the next node
         * @return The next node
         */
        virtual Node *get_next() { return next; }
        /**
         * @brief Method to get a constant pointer to the next node
         * @return The next node
         */
        virtual const Node *get_next() const { return next; }

        /**
         * @brief Destructor for the Node class
         */
        virtual ~Node() = default;

    protected:
        // Data of the node
        T data;
        // Pointer to the next node
        Node *next;
    };
} // namespace mtv

#endif //NODES_HPP

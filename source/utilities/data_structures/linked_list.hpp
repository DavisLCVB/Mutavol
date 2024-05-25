#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

/**
 * @file linked_list.hpp
 * @author Davis Cartagena
 * @brief Class and methods to manage a linked list
 */

//Include section
#include <functional>
#include <stdexcept>

#include "../factory/node_fact.hpp"

// Mutavol namespace
namespace mtv {
    /**
     * @class LinkedList
     * @brief Class to manage a linked list
     * @tparam T Type of the elements to store
     */
    template<typename T>
    class LinkedList {
    public:
        /**
         * @brief Default constructor, initializes the head and tail to nullptr and the size to 0
         */
        LinkedList() : head(nullptr), tail(nullptr), size(0) {}

        /**
         * @brief Copy constructor
         * @param other The linked list to be copied
         */
        LinkedList(const LinkedList &other) noexcept : head{nullptr}, tail{nullptr},
                                                       size(0) {
            for (auto &element: other) {
                push(element);
            }
        }

        /**
         * @brief Adds an element to the end of the list
         * @param data The data to be added
         */
        void push(const T &data) noexcept(false) {
            push(data, size);
        }

        /**
         * @brief Adds an element at the specified position
         * @param data The data to be added
         * @param index The position where the data will be added
         */
        void push(const T &data, const size_t index) noexcept(false) {
            if (!check_index(index, true)) {
                throw std::out_of_range("Index out of range");
            }
            auto node = NodeFactory::create_simple_node(data);
            if (size == 0) {
                head = node;
                tail = node;
            } else if (index == 0) {
                node->set_next(head);
                head = node;
            } else if (index == size) {
                tail->set_next(node);
                tail = node;
            } else {
                auto current = head;
                for (size_t i = 0; i < index - 1; ++i) {
                    current = current->get_next();
                }
                node->set_next(current->get_next());
                current->set_next(node);
            }
            size++;
        }

        /**
         * @brief Method to remove the last element of the list
         */
        void pop() noexcept(false) {
            pop(size - 1);
        }

        /**
         * @brief Method to remove the element at the specified position
         * @param index The position of the element to be removed
         */
        void pop(const size_t index) noexcept(false) {
            if (!check_index(index)) {
                const std::string msg =
                        "Index out of range: " + std::to_string(index) + " " +
                        std::to_string(size) + "\n";
                throw std::out_of_range(msg);
            }
            if (index == 0) {
                auto temp = head;
                head = head->get_next();
                delete temp;
            } else {
                auto current = head;
                for (size_t i = 0; i < index - 1; ++i) {
                    current = current->get_next();
                }
                auto temp = current->get_next();
                current->set_next(temp->get_next());
                delete temp;
            }
            size--;
        }

        /**
         * @brief Method to get the element at the specified position
         * @param index The position of the element to be retrieved
         * @return A reference to the element at the specified position
         */
        T &get(const size_t index) {
            return _get(this, index);
        }

        /**
         * @brief Returns a constant reference to the element at the specified position
         * @param index The position of the element to be retrieved
         * @return A constant reference to the element at the specified position
         */
        const T &get(const size_t index) const {
            return _get(this, index);
        }

        /**
         * @brief Method to get the size of the list
         * @return The size of the list
         */
        [[nodiscard]]
        size_t get_size() const {
            return size;
        }

        /**
         * @brief Method to check if the list is empty
         * @return true if the list is empty, false otherwise
         */
        [[nodiscard]]
        bool is_empty() const {
            return size == 0;
        }

        /**
         * @brief Gets the element at the specified position
         * @param index The position of the element to be retrieved
         * @return A reference to the element at the specified position
         */
        [[nodiscard]]
        T &operator[](const size_t index) {
            auto current = head;
            for (size_t i = 0; i < index; ++i) {
                current = current->get_next();
            }
            return current->get_data();
        }

        /**
         * @brief Gets a constant element at the specified position
         * @param index The position of the element to be retrieved
         * @return A constant reference to the element at the specified position
         */
        const T &operator[](const size_t index) const {
            auto current = head;
            for (size_t i = 0; i < index; ++i) {
                current = current->get_next();
            }
            return current->get_data();
        }

        /**
         * @brief Method to check if the element is in the list
         * @return true if the element is in the list, false otherwise
         */
        [[nodiscard]]
        bool contains(const T &data) const {
            auto current = head;
            while (current != nullptr) {
                if (current->get_data() == data) {
                    return true;
                }
                current = current->get_next();
            }
            return false;
        }

        /**
         * @brief Method to clear the list
         */
        void clear() {
            auto current = head;
            while (current != nullptr) {
                auto temp = current;
                current = current->get_next();
                delete temp;
            }
            head = nullptr;
            tail = nullptr;
            size = 0;
        }

        /**
         * @brief Destructor, clears the list
         */
        ~LinkedList() {
            clear();
        }

        /**
         * @class Iterator
         * @brief Class to manage an iterator for the linked list
         */
        class Iterator;
        /**
         * @class ConstIterator
         * @brief Class to manage a constant iterator for the linked list
         */
        class ConstIterator;

        /**
         * @brief Method to get an Iterator to the beginning of the list
         * @return An iterator to the beginning of the list
         * @see Iterator
         */
        Iterator begin() {
            return Iterator(head);
        }

        /**
         * @brief Method to get an iterator to the end of the list
         * @return An iterator to the end of the list
         * @see Iterator
         */
        Iterator end() {
            return Iterator(nullptr);
        }

        /**
         * @brief Method to get a constant iterator to the beginning of the list
         * @return A constant iterator to the beginning of the list
         * @see ConstIterator
         */
        ConstIterator begin() const {
            return ConstIterator(head);
        }

        /**
         * @brief Method to get a constant iterator to the end of the list
         * @return A constant iterator to the end of the list
         * @see ConstIterator
         */
        ConstIterator end() const {
            return ConstIterator(nullptr);
        }

    private:
        // Head of the list
        Node<T> *head;
        // Tail of the list
        Node<T> *tail;
        // Size of the list
        size_t size;

        /**
         * @brief Method to check if the index is valid
         * @param index The index to be checked
         * @param is_push If the index is for a push operation
         * @return true if the index is valid, false otherwise
         */
        [[nodiscard]]
        bool check_index(const size_t index,
                         const bool is_push = false) const {
            if (is_push) {
                return index <= size;
            }
            return index < size;
        }

        /**
         * @brief Get the element at the specified position and infer the qualifier
         * @param self The pointer to this object
         * @param index The position of the element to be retrieved
         * @return The element at the specified position with the correct qualifier
         */
        static auto &_get(auto &&self, const size_t index) {
            auto current = self->head;
            for (size_t i = 0; i < index; ++i) {
                current = current->get_next();
            }
            return current->get_data();
        }
    };


    template<typename T>
    class LinkedList<T>::Iterator {
    public:
        // Iterator traits
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T *;
        using reference = T &;

        /**
         * @brief Constructor
         * @param node the node to be pointed by the iterator
         */
        explicit Iterator(Node<T> *node) : node(node) {}

        /**
         * @brief Copy constructor
         * @param other the iterator to be copied
         */
        Iterator(const Iterator &other) noexcept : node(other.node) {}

        /**
         * @brief Dereference operator
         * @return the data of the node pointed by the iterator
         */
        reference operator*() const {
            return node->get_data();
        }

        /**
         * @brief Arrow operator
         * @return The pointer to the data of the node pointed by the iterator
         */
        pointer operator->() {
            return &node->get_data();
        }

        /**
         * @brief Prefix increment operator, moves the iterator to the next node
         * @return A reference to this iterator
         */
        Iterator &operator++() {
            node = node->get_next();
            return *this;
        }

        /**
         * @brief Postfix increment operator, moves the iterator to the next node
         * @return A copy of the iterator before the increment
         */
        Iterator operator++(int) {
            Iterator temp = *this;
            node = node->get_next();
            return temp;
        }

        /**
         * @brief Addition operator, generates an iterator that points to the node at the resulting of adding the index
         * @param index The index to be added
         * @return A copy of the iterator with the index added
         */
        Iterator operator+(const size_t index) {
            auto cp{*this};
            for (size_t i = 0; i < index && cp.node != nullptr; ++i) {
                cp.node = cp.node->get_next();
            }
            return cp;
        }

        /**
         * @brief Friend function to compare two iterators
         * @param lhs First iterator
         * @param rhs Second iterator
         * @return true if the iterators are equal, false otherwise
         */
        friend bool operator==(const Iterator &lhs, const Iterator &rhs) {
            return lhs.node == rhs.node;
        }

        /**
         * @brief Friend function to compare two iterators
         * @param lhs First iterator
         * @param rhs Second iterator
         * @return true if the iterators are different, false otherwise
         */
        friend bool operator!=(const Iterator &lhs, const Iterator &rhs) {
            return lhs.node != rhs.node;
        }

    private:
        // Node pointed by the iterator
        Node<T> *node;
    };

    template<typename T>
    class LinkedList<T>::ConstIterator {
    public:
        // Iterator traits
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = const T;
        using pointer = const T *;
        using reference = const T &;

        /**
         * @brief Constructor
         * @param node the node to be pointed by the iterator
         */
        explicit ConstIterator(const Node<T> *
            node) : node(node) {}

        /**
         * @brief Copy constructor
         * @param other The iterator to be copied
         */
        ConstIterator(const ConstIterator &other) noexcept : node(other.node) {}

        /**
         * @brief Dereference operator
         * @return The data of the node pointed by the iterator
         */
        reference operator*() const {
            return node->get_data();
        }

        /**
         * @brief Arrow operator
         * @return A pointer to the data of the node pointed by the iterator
         */
        pointer operator->() const {
            return &node->get_data();
        }

        /**
         * @brief Prefix increment operator, moves the iterator to the next node
         * @return A reference to this iterator
         */
        ConstIterator &operator++() {
            node = node->get_next();
            return *this;
        }

        /**
         * @brief Postfix increment operator, moves the iterator to the next node
         * @return A copy of the iterator before the increment
         */
        ConstIterator operator++(int) {
            ConstIterator temp = *this;
            node = node->get_next();
            return temp;
        }

        /**
         * @brief Addition operator, generates an iterator that points to the node at the resulting of adding the index
         * @param index The index to be added
         * @return A copy of the iterator with the index added
         */
        ConstIterator operator+(const size_t index) {
            auto cp{*this};
            for (size_t i = 0; i < index && cp.node != nullptr; ++i) {
                cp.node = cp.node->get_next();
            }
            return cp;
        }

        /**
         * @brief Friend function to compare two iterators
         * @param lhs First iterator
         * @param rhs Second iterator
         * @return true if the iterators are equal, false otherwise
         */
        friend bool operator==(const ConstIterator &lhs, const ConstIterator &rhs) {
            return lhs.node == rhs.node;
        }

        /**
         * @brief Friend function to compare two iterators
         * @param lhs First iterator
         * @param rhs Second iterator
         * @return true if the iterators are different, false otherwise
         */
        friend bool operator!=(const ConstIterator &lhs, const ConstIterator &rhs) {
            return lhs.node != rhs.node;
        }

    private:
        // Node pointed by the iterator
        const Node<T> *node;
    };
} //namespace mtv

#endif //LINKED_LIST_HPP

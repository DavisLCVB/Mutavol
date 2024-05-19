#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include <functional>

#include "../factory/node_fact.hpp"

namespace mtv {
    template<typename T>
    class LinkedList {
    public:
        LinkedList() : head(nullptr), tail(nullptr), size(0) {}

        void push(const T &data) noexcept(false) {
            push(data, size);
        }

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

        void pop() noexcept(false) {
            pop(size - 1);
        }

        void pop(const size_t index) noexcept(false) {
            if (!check_index(index))
                throw std::out_of_range("Index out of range");
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

        T &get(const size_t index) {
            return _get(this, index);
        }

        const T &get(const size_t index) const {
            return _get(this, index);
        }

        [[nodiscard]]
        size_t get_size() const {
            return size;
        }

        T &operator[](const size_t index) {
            auto current = head;
            for (size_t i = 0; i < index; ++i) {
                current = current->get_next();
            }
            return current->get_data();
        }

        const T &operator[](const size_t index) const {
            auto current = head;
            for (size_t i = 0; i < index; ++i) {
                current = current->get_next();
            }
            return current->get_data();
        }

        void clear() {
            auto current = head;
            while (current != nullptr) {
                auto temp = current;
                current = current->get_next();
                delete temp;
            }
        }

        ~LinkedList() {
            clear();
        }

        class Iterator;
        class ConstIterator;

        Iterator begin() {
            return Iterator(head);
        }

        Iterator end() {
            return Iterator(nullptr);
        }

        ConstIterator begin() const {
            return ConstIterator(head);
        }

        ConstIterator end() const {
            return ConstIterator(nullptr);
        }

    private:
        Node<T> *head;
        Node<T> *tail;
        size_t size;

        [[nodiscard]]
        bool check_index(const size_t index,
                         const bool is_push = false) const {
            if (is_push) {
                return index <= size;
            }
            return index < size;
        }

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
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = T;
        using pointer           = T*;
        using reference         = T&;

        explicit Iterator(Node<T> *node) : node(node) {}

        reference operator*() const {
            return node->get_data();
        }

        pointer operator->() {
            return &node->get_data();
        }

        Iterator &operator++() {
            node = node->get_next();
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            node = node->get_next();
            return temp;
        }

        friend bool operator==(const Iterator &lhs, const Iterator &rhs) {
            return lhs.node == rhs.node;
        }

        friend bool operator!=(const Iterator &lhs, const Iterator &rhs) {
            return lhs.node != rhs.node;
        }

    private:
        Node<T> *node;
    };

    template<typename T>
    class LinkedList<T>::ConstIterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = const T;
        using pointer           = const T*;
        using reference         = const T&;

        explicit ConstIterator(Node<T> *node) : node(node) {}

        reference operator*() const {
            return node->get_data();
        }

        pointer operator->() const {
            return &node->get_data();
        }

        ConstIterator &operator++() {
            node = node->get_next();
            return *this;
        }

        ConstIterator operator++(int) {
            ConstIterator temp = *this;
            node = node->get_next();
            return temp;
        }

        friend bool operator==(const ConstIterator &lhs, const ConstIterator &rhs) {
            return lhs.node == rhs.node;
        }

        friend bool operator!=(const ConstIterator &lhs, const ConstIterator &rhs) {
            return lhs.node != rhs.node;
        }
    private:
        const Node<T> *node;
    };
} //namespace mtv

#endif //LINKED_LIST_HPP

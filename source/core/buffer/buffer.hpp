#ifndef BUFFER_HPP
#define BUFFER_HPP
#include <iostream>
#include <memory>

#include "../../utilities/data_structures/linked_list.hpp"
#include "../../utilities/factory/list_fact.hpp"

namespace mtv {
    template<typename T>
    class Buffer {
    public:
        static Buffer &get_instance() {
            if (instance == nullptr) {
                init_instance();
            }
            return *instance;
        }

        void push(const T &element) {
            this->push(element, buffer->get_size());
        }

        void push(const T &element, const size_t index) {
            try {
                buffer->push(element, index);
            } catch (std::exception &e) {
                std::cerr << e.what() << std::endl;
            }
        }

        void pop() {
            try {
                this->pop();
            } catch (std::exception &e) {
                std::cerr << e.what() << std::endl;
            }
        }

        void pop(const size_t index) {
            try {
                buffer->pop(index);
            } catch (std::exception &e) {
                std::cerr << e.what() << std::endl;
            }
        }

        [[nodiscard]]
        size_t get_size() const {
            return buffer->get_size();
        }

        T &get(size_t index) {
            return buffer->get(index);
        }

        const T &get(size_t index) const {
            return buffer->get(index);
        }

        T &operator[](size_t index) {
            return buffer->get(index);
        }

        Buffer(const Buffer &) = delete;

        Buffer &operator=(const Buffer &) = delete;

        typename LinkedList<T>::Iterator begin() {
            return buffer->begin();
        }

        typename LinkedList<T>::Iterator end() {
            return buffer->end();
        }

        typename LinkedList<T>::ConstIterator begin() const {
            return buffer->begin();
        }

        typename LinkedList<T>::ConstIterator end() const {
            return buffer->end();
        }

    private:
        static std::unique_ptr<Buffer> instance;
        LinkedList<T> *buffer;

        Buffer() {
            buffer = ListFactory::create_linked_list<T>();
        };

        static void init_instance() {
            instance.reset(new Buffer());
        }
    };

    template<typename T>
    std::unique_ptr<Buffer<T> > Buffer<T>::instance = nullptr;

} // namespace mtv

#endif //BUFFER_HPP

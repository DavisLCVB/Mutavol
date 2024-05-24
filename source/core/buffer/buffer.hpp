#ifndef BUFFER_HPP
#define BUFFER_HPP

/**
* @file buffer.hpp
* @author Davis Cartagena
* @brief Class and methods to manage the buffer
*/

//Include section
#include <iostream>
#include <memory>

#include "../../utilities/data_structures/linked_list.hpp"
#include "../../utilities/factory/list_fact.hpp"

// Mutavol namespace
namespace mtv {
    /**
    * @class Buffer
    * @brief Class to store data in a unique instance
    * @tparam T Type of the elements to store
    */
    template<typename T>
    class Buffer {
    public:
        /**
        * @brief Gets the instance of the Buffer class
        * @return The instance of the Buffer class
        */
        static Buffer &get_instance() {
            if (instance == nullptr) {
                init_instance();
            }
            return *instance;
        }

        /**
        * @brief Adds an element to the end of the buffer
        */
        void push(const T &element) {
            this->push(element, buffer->get_size());
        }

        /**
        * @brief Adds an element at specified position
        */
        void push(const T &element, const size_t index) {
            try {
                buffer->push(element, index);
            } catch (std::exception &e) {
                std::cerr << e.what() << std::endl;
            }
        }

        /**
        * @brief Removes an element from the end of the buffer
        */
        void pop() {
            try {
                this->pop();
            } catch (std::exception &e) {
                std::cerr << e.what() << std::endl;
            }
        }

        /**
        * @brief Removes an element from specified position
        */
        void pop(const size_t index) {
            try {
                buffer->pop(index);
            } catch (std::exception &e) {
                std::cerr << e.what() << std::endl;
            }
        }

        /**
        * @brief Gets the size of the buffer
        * @return The size of the buffer
        */
        [[nodiscard]]
        size_t get_size() const {
            return buffer->get_size();
        }

        /**
        * @brief Gets the element at specified position
        */
        T &get(size_t index) {
            return buffer->get(index);
        }

        /**
        * @brief Gets a constant element at specified position
        */
        const T &get(size_t index) const {
            return buffer->get(index);
        }

        /**
        * @brief Gets the element at specified position
        */
        T &operator[](size_t index) {
            return buffer->get(index);
        }

        // Delete copy constructor
        Buffer(const Buffer &) = delete;

        // Delete copy assignment operator
        Buffer &operator=(const Buffer &) = delete;

        /**
        * @brief Gets an iterator to the beginning of the buffer
        */
        typename LinkedList<T>::Iterator begin() {
            return buffer->begin();
        }

       /**
        * @brief Gets an iterator to the end of the buffer
        */
        typename LinkedList<T>::Iterator end() {
            return buffer->end();
        }

        /**
        * @brief Gets a constant iterator to the beginning of the buffer
        */
        typename LinkedList<T>::ConstIterator begin() const {
            return buffer->begin();
        }

        /**
        * @brief Gets a constant iterator to the end of the buffer
        */
        typename LinkedList<T>::ConstIterator end() const {
            return buffer->end();
        }

    private:
        // Pointer to the unique instance of the class
        static std::unique_ptr<Buffer> instance;
        // Pointer to the buffer (LinkedList)
        LinkedList<T> *buffer;

        /**
        * Constructor
        */
        Buffer() {
            buffer = ListFactory::create_linked_list<T>();
        };

        /**
        * Method to initialize the instance of the class
        */
        static void init_instance() {
            instance.reset(new Buffer());
        }
    };

    template<typename T>
    std::unique_ptr<Buffer<T> > Buffer<T>::instance = nullptr;

} // namespace mtv

#endif //BUFFER_HPP

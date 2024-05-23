#ifndef NODES_HPP
#define NODES_HPP

namespace mtv {
    template<typename T>
    class Node {
    public:
        explicit Node(const T &_data) : data(_data) , next(nullptr) {}
        virtual void set_data(T _data) { this->data = _data; }
        virtual T &get_data() { return data; }
        virtual const T &get_data() const { return data; }
        virtual void set_next(Node *next) { this->next = next; }
        virtual Node *get_next() { return next; }
        virtual const Node *get_next() const { return next; }

        virtual ~Node() = default;

    protected:
        T data;
        Node *next;
    };

    template<typename T>
    class DoubleNode final : public Node<T> {
    public:
        explicit DoubleNode(const T &data) : Node<T>(data), previous(nullptr) {}
        void set_previous(DoubleNode *previous) { this->previous = previous; }
        const DoubleNode *get_previous() const { return previous; }
        DoubleNode *get_previous() { return previous; }

    private:
        DoubleNode *previous;
    };
} // namespace mtv

#endif //NODES_HPP

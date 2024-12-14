#ifndef BTREE_CPP
#define BTREE_CPP

#include <iostream>

template <typename T, int N>
struct Node {
    T data[N];
    Node* parent;
    Node* children[N + 1] = {nullptr};
    int size {0};

    explicit Node(Node* parent) : parent{parent} {}

    int insert(const T& value) {
        int index {0};
        while (index < size && data[index] < value) {
            ++index;
        }

        for (int i {size}; i > index; --i) {
            data[i] = data[i - 1];
            children[i + 1] = children[i];
        }

        data[index] = value;
        ++size;
        return index;
    }
};

template <typename T, int order>
class BTree {

    Node<T, order> * root {nullptr};
    Node<T, order>* find_node(Node<T, order>* node, const T& value) const {
        if (!node || !node->children[0]) {
            return node;
        }

        for (int i {0}; i < node->size; ++i) {
            if (value <= node->data[i]) {
                return find_node(node->children[i], value);
            }
        }

        return find_node(node->children[node->size], value);
    }

    void split_node(Node<T, order>* node) {
        if (!node || node->size < order) {
            return;
        }

        Node<T, order>* parent;

        if (!node->parent) {
            parent = new Node<T, order>{nullptr};
            root = parent;
            node->parent = parent;
        } else {
            parent = node->parent;
        }

        Node<T, order>* new_right_node = new Node{parent};
        int mid_index {(order - 1) / 2};

        for (int i {mid_index + 1}; i < order; ++i) {
            new_right_node->data[i - mid_index - 1] = node->data[i];
            new_right_node->children[i - mid_index - 1] = node->children[i];

            if (new_right_node->children[i - mid_index - 1]) {
                new_right_node->children[i - mid_index - 1]->parent = new_right_node;
            }
        }

        new_right_node->children[order - mid_index - 1] = node->children[order];
        new_right_node->size = order - mid_index - 1;

        int parentIndex = parent->insert(node->data[mid_index]);
        parent->children[parentIndex] = node;
        parent->children[parentIndex + 1] = new_right_node;

        node->size = mid_index;

        split_node(parent);
    }

    void print(Node<T, order>* node, const int indentation) const {
        if (!node) {
            return;
        }

        std::cout << std::string(indentation, ' ');
        std::cout << node->data[0];

        for (int i {1}; i < node->size; ++i) {
            std::cout << ',' << node->data[i];
        }

        std::cout << std::endl;

        for (int i {0}; i <= node->size; ++i) {
            print(node->children[i], indentation + 2);
        }
    }

public:
    BTree() {
        if (order < 3) {
            throw std::invalid_argument("The order should at least be 3.");
        }
    }

    void Insert(const T& value) {
        if (!root) {
            root = new Node<T, order>{nullptr};
            root->insert(value);
            return;
        }

        Node<T, order>* node = find_node(root, value);
        node->insert(value);
        split_node(node);
    }

    void Print() const {
        print(root, 0);
    }
};

#endif

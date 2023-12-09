#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>


const int t = 3; // минимальная степень дерева

struct Node {
    std::vector<int> keys;
    std::vector<Node*> child;
    Node* parent;

    Node() : parent(nullptr) {} // создание пустой вершины

    Node(Node* parent, const int& key) { // создание нового корня
        this->parent = parent;
        keys.push_back(key);
    }

    Node(Node* parent, const std::vector<int>& keys) : parent(parent), keys(keys) {}
};

class BTree {
private:
    Node* root;

private:
    void deleteNode(Node* nd);
    void insert_to_node(Node* nd, const int& key);
    void split_node(Node* nd, const int& middle);
    void split_childs(Node* l_node, Node* r_node);

    void merge_nodes(Node* ptr, Node* parent, std::vector<Node*>::iterator&, int& nd_pos);
    void rebalance(Node* ptr);


public:
    BTree() : root(nullptr) {}
    ~BTree();
    
    void insert(const int& key);
    void remove(const int& key, Node* from = nullptr);
    std::pair<Node*, int> search(const int& key);

    void print(const Node* ptr = nullptr, int level = 0);
};
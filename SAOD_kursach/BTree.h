#pragma once

#include <vector>
#include <string>
#include <memory>
//#include "Node.h"


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
    const int T; // минимальная степень дерева

    Node* root;
    size_t keyCount;
    size_t treeDepth;

private:
    void deleteNode(Node* nd);
    void insertToNode(Node* nd, const int& key);
    void splitNode(Node* nd, const int& middle);
    void splitChilds(Node* l_node, Node* r_node);

    void mergeNodes(Node* ptr, Node* parent, std::vector<Node*>::iterator&, int& nd_pos);
    void rebalance(Node* ptr);


public:
    BTree(const int T) : T(T), root(nullptr), keyCount(NULL), treeDepth(NULL) {}
    ~BTree();

    void remove(const int& key, Node* from = nullptr);
    std::pair<Node*, int> search(const int& key);
    void insert(const int& key);
    void insertList(const std::vector<int>& keys) {
        for (int i = 0; i < keys.size(); i++) {
            insert(keys[i]);
        }
    }

    size_t getKeyCount() { return keyCount; }
    size_t getTreeDepth() { return treeDepth; }

    void print(const Node* ptr = nullptr, int level = 0, int pos = 0, std::string output = "");

    void clear();
};
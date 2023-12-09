#include "BTree.h"

#include <algorithm>
#include <iostream>
#include <string>



void BTree::clear() {
    deleteNode(root);
    root = nullptr;
    keyCount = 0;
    treeDepth = 0;
}

BTree::~BTree() {
    deleteNode(root);
    //root == nullptr;
}

void BTree::deleteNode(Node* nd) {
    if (nd != nullptr) {
        for (int i = 0; i < nd->child.size(); i++) {
            deleteNode(nd->child[i]);
        }
        nd->keys.clear();
        nd->child.clear();
        delete(nd);
    }
}



void BTree::splitChilds(Node* l_node, Node* r_node) {

    std::vector<Node*> childs; // ����� ������ ��������
    childs.insert(childs.end(), l_node->child.begin(), l_node->child.end());
    childs.insert(childs.end(), r_node->child.begin(), r_node->child.end());

    int mid = childs.size() / 2;

    std::vector<Node*> l_child(childs.begin(), childs.begin() + mid);
    std::vector<Node*> r_child(childs.begin() + mid, childs.end());

    childs.clear();

    l_node->child = l_child;
    r_node->child = r_child;

    // ��������� ���������� � ��������� � ��������
    for (int i = 0; i < r_child.size(); i++) {
        r_child[i]->parent = r_node;
    }
    for (int i = 0; i < l_child.size(); i++) {
        l_child[i]->parent = l_node;
    }
}

void BTree::splitNode(Node* ptr, const int& middle) {

    Node* parent = ptr->parent; // ���������� ��������

    if (parent == nullptr) { // ���� ��� ��������
        parent = new Node();
        ptr->parent = parent;
        root = parent;
        parent->child.push_back(ptr);

        treeDepth++; // ������� ������ �����������
    }

    std::vector<int> l_keys(ptr->keys.begin(), ptr->keys.begin() + middle);
    std::vector<int> r_keys(ptr->keys.begin() + middle + 1, ptr->keys.end());

    Node* r_node = new Node(parent, r_keys); // ������� ����� �������
    ptr->keys = l_keys;

    l_keys.clear();
    r_keys.clear();

    // ��������� ����� ������� � ������ ������ �������� ����� ����� ������� �������.
    auto it = std::find_if(parent->child.begin(), parent->child.end(), [&ptr](const auto& lhs) {
        return lhs == ptr;
        });
    parent->child.insert(it + 1, r_node);

    // ��������� �������� ����� ����� ���������, ���� ��� �� �������� ��������
    if (ptr->child.size() != 0) {
        splitChilds(ptr, r_node);
    }
}

void BTree::insertToNode(Node* ptr, const int& key) {
    // ��������� ���� � ������� � ��������� ��������
    ptr->keys.push_back(key);
    sort(ptr->keys.begin(), ptr->keys.end());

    // ���� ������� ���������
    if (ptr->keys.size() >= 2 * T - 1) {
        int middle = ptr->keys.size() / 2;
        int new_separator = ptr->keys[middle];

        // ��������� �������
        splitNode(ptr, middle);

        // ��������� ����������� ���� � ��������
        insertToNode(ptr->parent, new_separator);
    }
    else {
        keyCount++;
    }
}

void BTree::insert(const int& key) {

    if (root == nullptr) {                // ������� ������, ���� ��� ���
        root = new Node(nullptr, key);
        keyCount++;
    }
    else {
        Node* ptr = root;                 // ��������� �� �������

        while (ptr->child.size() != 0) {  // ������ �� ������ �� ��� ���, ���� �� ������ � ����
            bool is_lower = false;

            // ��� ������� ����� ��������� - ���� key ������, �� ��������� � �������� ����� �� ��������
            for (int i = 0; i < ptr->keys.size(); i++) {
                // ���� ����� ���� ��� ����
                if (key == ptr->keys[i]) {
                    std::cout << "\nThe key already exists" << std::endl;
                    return;
                }

                if (key < ptr->keys[i]) { // ���� ����� ���� ������ ��������, ��
                    ptr = ptr->child[i];  // ��������� � ��������������� �������
                    is_lower = true;
                    break;
                }
            }

            // ���� ���� ������ ���� ������ � �������, ��������� � ������� ������ �������
            if (is_lower == false) {
                ptr = ptr->child.back();
            }
        }
        // ��������� ���� � �������
        insertToNode(ptr, key);
    }
}

void BTree::mergeNodes(Node* ptr, Node* parent, std::vector<Node*>::iterator& nd_it, int& nd_pos) {
    Node* l_neighb = ptr;
    Node* r_neighb = ptr;

    if (nd_it != parent->child.begin()) { // ���� ���� ����� �����
        l_neighb = parent->child[nd_pos - 1];
        nd_pos--;
    } 
    else { // ���� ���� ����� ������
        nd_it++;
        r_neighb = parent->child[nd_pos + 1];
    }

    l_neighb->keys.push_back(parent->keys[nd_pos]); // �������� ����-����������� �������� � ����� ������� �������� �������
    l_neighb->keys.insert(l_neighb->keys.end(), r_neighb->keys.begin(), r_neighb->keys.end()); // �������� ��� �������� ������ ������� � �����

    // ���� �������� ������ ������� ����������� ����� �������
    l_neighb->child.insert(l_neighb->child.end(), r_neighb->child.begin(), r_neighb->child.end());
    for (int i = 0; i < r_neighb->child.size(); i++) {
        r_neighb->child[i]->parent = l_neighb;
    }
    r_neighb->child.clear();

    parent->keys.erase(parent->keys.begin() + nd_pos); // ������� ����-����������� �� ��������
    deleteNode(parent->child[nd_it - parent->child.begin()]);
    parent->child.erase(nd_it); // ������� ������ �������        

    if (parent == root && parent->keys.size() == 0) { // ���� �������� - ������ ������
        root = l_neighb;
        parent->child.clear();
        l_neighb->parent = nullptr;
        deleteNode(parent);
        //std::cout << "worked this rebalancing";
    }
    else if (parent->keys.size() < parent->child.size() - 1) { // ���� �������� ����� ����� ������������� ���������� ������
        rebalance(parent); // ����������� ������
        //std::cout << "rebalancing done";
    }
}

void BTree::rebalance(Node* ptr) {

    // ������� ������� ������� � ������ ������ ��������
    Node* parent = ptr->parent;

    if (parent == nullptr) {
        std::cout << "\nParent cant be NULL" << std::endl;
        exit(EXIT_FAILURE);
    }

    auto nd_it = std::find_if(parent->child.begin(), parent->child.end(), [&ptr](const auto& lhs) {
        return lhs == ptr;
        });
    int nd_pos = nd_it - parent->child.begin();

    // ���� ���� ����� ����� � �� ����� ����� ������������ ���������� ������
    if (nd_it != parent->child.begin()) {
        if (parent->child[nd_pos - 1]->keys.size() > T - 1) {
            Node* l_neighb = parent->child[nd_pos - 1];

            // rotate right
            ptr->keys.insert(ptr->keys.begin(), parent->keys[nd_pos - 1]);
            parent->keys[nd_pos - 1] = l_neighb->keys.back();

            splitChilds(l_neighb, ptr);
            l_neighb->keys.pop_back();

            return;
        }
    }

    // ���� ���� ������ ����� � �� ����� ����� ������������ ���������� ������
    if (nd_it != parent->child.end() - 1) {
        if (parent->child[nd_pos + 1]->keys.size() > T - 1) {
            Node* r_neighb = parent->child[nd_pos + 1];

            // rotate left
            ptr->keys.push_back(parent->keys[nd_pos]);
            parent->keys[nd_pos] = r_neighb->keys.front();

            splitChilds(ptr, r_neighb);
            r_neighb->keys.erase(r_neighb->keys.begin());

            return;
        }
    }

    // ����� ���������� �������
    mergeNodes(ptr, parent, nd_it, nd_pos);
}

void BTree::remove(const int& key, Node* ptr) {

    std::pair<Node*, int> vertex_of_key;
    int key_pos;
    std::vector<int>::iterator key_it;

    // ���� ����, ������� �������� ���� � ������� ����� � �������
    if (ptr == nullptr) {
        vertex_of_key = search(key);

        ptr = vertex_of_key.first;
        key_pos = vertex_of_key.second;
        key_it = ptr->keys.begin() + key_pos;
    }
    else {
        key_it = std::find_if(ptr->keys.begin(), ptr->keys.end(), [&key](const auto& lhs) {
            return lhs == key;
            });
        key_pos = key_it - ptr->keys.begin();
    }


    // ���� ������� �������� ������
    if (ptr->child.size() == 0) {
        ptr->keys.erase(key_it); // ������� ����
        keyCount--;

        if (ptr->keys.size() < T - 1) { // ����������� ������, ���� �������
            rebalance(ptr); // ����� ����� ������ ���������� ���������� ������
        }
        return;
    }
    else { // ���� ���� �������� ����������
        Node* subtree = ptr->child[key_pos]; // ����� ���������

        while (subtree->child.size() != 0) {
            subtree = subtree->child.back();
        }

        int new_key = subtree->keys.back(); // ������������ ���� � ����� ���������
        ptr->keys[key_pos] = new_key;

        remove(new_key, subtree);
    }
}



std::pair<Node*, int> BTree::search(const int& key) {
    Node* ptr = root;

    for (;;) {
        bool is_lower = false;
        for (int i = 0; i < ptr->keys.size(); i++) {
            if (key == ptr->keys[i]) {
                return { ptr, i };
            }
            if (key < ptr->keys[i]) {
                ptr = ptr->child[i];
                is_lower = true;    
                break;
            }
        }
        if (is_lower == false) {
            if (ptr->child.size() == 0) {
                std::cout << "\nThe element could not be found" << std::endl;
                exit(EXIT_FAILURE);
            }
            ptr = ptr->child[ptr->child.size() - 1];
        }
    }
    return { nullptr, NULL };
}



void BTree::print(const Node* ptr, int level, int pos, std::string output) {
    // output - "�������", ������������� ��� ������ �������� ��� ��� ��������
    std::cout << output; // ����� "��������"

    if (ptr == nullptr) { // ���� �� ������� ��������
        ptr = root;
        output += "+";
        for (int i = 0; i < 2 * T - 1; i++)
            output += "-";
    }
    else {
        int len = 2 * T;
        // ���� ������ �� ��������� � ������ �������� ��������
        if (pos != ptr->parent->keys.size()) {
            output.insert(output.size() - len, "|");
            for (int i = 0; i < len - 1; i++)
                output.insert(output.size() - len, " ");
        }
        // ���� ������ ��������� � ������ �������� ��������
        else {
            for (int i = 0; i < len; i++)
                output.insert(output.size() - len, " ");
        }
    }
    
    int str_len = 0; // ������� ���������� �������� � ������ �����
    for (int i = 0; i < ptr->keys.size(); i++) {
        std::cout << ptr->keys[i];

        // ������������ ���������� �������� � ������
        int key = ptr->keys[i];
        while (key) {
            key /= 10;
            str_len++;
        }

        if (i != ptr->keys.size() - 1)
            std::cout << ", ";
    }

    // ������� ������ ����� ��������
    for (int i = 0; i < str_len - 1; i++)
        output.insert(output.size() - 2 * T, " ");
    
    // ���������� ������ ������ ������ �������
    std::cout << "\n";
    if (ptr->parent != nullptr) {
        if (ptr->child.size() == 0 && pos == ptr->parent->keys.size()) {
            std::cout << output.substr(0, output.size() - 2 * T);
            std::cout << "\n";
        }
    }

    // ������������ ����� ��������
    for (int pos = 0; pos < ptr->child.size(); pos++) {
        print(ptr->child[pos], level + 1, pos, output);
    }
}
#pragma once
#include <vector>

template<typename K, typename V>
class BTreeNode {
public:
    BTreeNode() = default;

    ~BTreeNode() = default;

private:
    bool is_leaf_;
};

template<typename K, typename V>
class BTree {
public:
    BTree() = default;

    ~BTree() = default;

    void insert(const K &key, const V &value) {
    };

    V get(const K &key) {
    };

private:
    std::vector<BTreeNode<K, V> *> root_;
};

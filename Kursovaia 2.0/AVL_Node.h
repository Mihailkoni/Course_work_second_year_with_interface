#pragma once
#include "Hash_Table.h"
#include "Key.h"
#include <string>

struct AVL_Node {
    int bal;
    AVL_Node* left;
    AVL_Node* right;
    Hash_Table* HT;
    Key* key;
    AVL_Node(int initial_max_table_size) :key(new Key()), HT(new Hash_Table(initial_max_table_size)), right(nullptr), left(nullptr), bal(0) {};

};

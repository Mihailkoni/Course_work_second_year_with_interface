#pragma once

#include "Key.h"
#include <string>

enum Node_status { // статусы ключа
    ZERO, ONE, TWO
};

struct Hash_Table_Node {
    Node_status status;
    Key* key;
    Hash_Table_Node() : status(ZERO) {}
};

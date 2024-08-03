#ifndef LINKED_H
#define LINKED_H

#include "dlinked_list.hpp"

// For now the linked list can only hold a pointer to a dlinked node
struct linked {
    linked *child_node;
    dlinked* node_data;
};

#endif
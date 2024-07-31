#ifndef DLINKED_H
#define DLINKED_H


// For now the double linked list can only hold vertex data from polygons
struct dlinked {
    dlinked *parent_node;
    dlinked *child_node;
    float x;
    float y;
    float z;
};

#endif
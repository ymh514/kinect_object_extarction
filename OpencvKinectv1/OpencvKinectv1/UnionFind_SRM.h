#include <stdlib.h>


#ifndef SRM_UNIONFIND_H
#define SRM_UNIONFIND_H



struct UnionFind {
    unsigned int size;
    unsigned int count;
    unsigned int* weights;
    unsigned int* parents;
};

UnionFind* unionfind_new(unsigned int size);
void unionfind_init(UnionFind* uf);
unsigned int unionfind_find(UnionFind* uf, unsigned int id);
unsigned int unionfind_union(UnionFind* uf, unsigned int i1, unsigned int i2);
unsigned int unionfind_count(UnionFind* uf);
void unionfind_delete(UnionFind* uf);

#endif
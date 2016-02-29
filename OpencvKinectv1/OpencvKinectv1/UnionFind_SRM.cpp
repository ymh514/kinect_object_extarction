#include "UnionFind_SRM.h"


UnionFind* unionfind_new(unsigned int size) {
    UnionFind* uf;
    uf = (UnionFind*)malloc(sizeof(UnionFind));

    uf->count = size;
    uf->size = size;
    uf->weights = (unsigned int*)malloc(size * sizeof(unsigned int));
    uf->parents = (unsigned int*)malloc(size * sizeof(unsigned int));

    unionfind_init(uf);

    return uf;
}

void unionfind_init(UnionFind* uf) {
    for (unsigned int i = 0; i < uf->size; i++) {
        uf->weights[i] = 1;
        uf->parents[i] = i;
    }

}

unsigned int unionfind_find(UnionFind* uf, unsigned int id) {
    /* Finding the root */
    unsigned int root = uf->parents[id];
    unsigned int previous;

    do {
        previous = root;
        root = uf->parents[root];
    } while (root != previous);

    /* Path compression */
    unsigned int next = id;
    unsigned int tmp;
    do {
        tmp = next;
        next = uf->parents[tmp];
        uf->parents[tmp] = root;
    } while (root != next);

    return root;
}

unsigned int unionfind_union(UnionFind* uf, unsigned int i1, unsigned int i2) {
    i1 = unionfind_find(uf, i1);
    i2 = unionfind_find(uf, i2);

    unsigned int w1 = uf->weights[i1];
    unsigned int w2 = uf->weights[i2];

    /* i1 will be the heaviest set */
    if (w2 > w1) {
        unsigned int tmp;

        tmp = i1;
        i1 = i2;
        i2 = tmp;

        tmp = w1;
        w1 = w2;
        w2 = tmp;
    }

    uf->weights[i1] += w2;
    uf->parents[i2] = i1;

    uf->count--;

    return i1;
}

unsigned int unionfind_count(UnionFind* uf) {
    return uf->count;
}

void unionfind_delete(UnionFind* uf) {
    free(uf->weights);
    free(uf->parents);
    free(uf);
}


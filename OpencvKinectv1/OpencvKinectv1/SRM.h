
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "UnionFind_SRM.h"


#ifndef SRM_H
#define SRM_H

struct Pair {
    unsigned int r1;
    unsigned int r2;
    unsigned int diff;
};

struct SRM {
    unsigned int width;
    unsigned int height;
    unsigned int size;
    unsigned int channels;
    uint8_t *in;
    uint8_t *out;
    unsigned int *sizes;
    double logdelta;
    unsigned int smallregion;
    double g;
    double Q;
    UnionFind* uf;
    unsigned int borders;
    Pair *pairs;
    unsigned int n_pairs;
    Pair *ordered_pairs;
    unsigned int widthStep_in;
    unsigned int widthStep_out;
};

SRM* srm_new(double Q, unsigned int width, unsigned int height, unsigned int channels, unsigned int borders);
void srm_run(SRM *srm, unsigned int widthStep_in, uint8_t *in, unsigned int widthStep_out, uint8_t *out);
unsigned int srm_regions_count(SRM *srm);
unsigned int* srm_regions(SRM *srm);
unsigned int* srm_regions_sizes(SRM *srm);
void srm_delete(SRM *srm);

void DoSRM(double Q, unsigned int width, unsigned int height, unsigned int channels, uint8_t *in, uint8_t *out, unsigned int borders);

#endif
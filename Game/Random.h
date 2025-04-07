#ifndef RANDOM_H
#define RANDOM_H

#include "Random.c"

struct Random;
void intialise(struct Random* random);
void setSeed(struct Random* random, char seed);
unsigned char getRandom(struct Random* random);

#endif
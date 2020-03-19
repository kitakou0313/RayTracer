#ifndef RANDOMH
#define RANDOMH

#include <cstdlib>
#include <random>

std::random_device seed_gen;
std::default_random_engine engine(seed_gen());
std::uniform_real_distribution<> dist(0.0, 1.0);

double drand()
{
    return rand() / (RAND_MAX + 1.0);
}

#endif
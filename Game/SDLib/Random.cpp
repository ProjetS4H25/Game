#pragma once

#include <iostream>
#include <random>

class Random {
private:
    static std::random_device randomDevice;                   // Source de randomisation
    static std::mt19937 generator;                            // Générateur Mersenne Twister
    static std::uniform_int_distribution<int> distribution;   // Distribution uniforme pour les entiers

public:
    static void setRange(int min, int max) {
        distribution = std::uniform_int_distribution<int>(min, max);
    }

    static int getRandomNumber() {
        return distribution(generator);
    }
};
std::random_device Random::randomDevice;
std::mt19937 Random::generator(randomDevice());
std::uniform_int_distribution<int> Random::distribution(0, 100);

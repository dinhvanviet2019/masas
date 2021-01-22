#include "Random.h"

std::random_device Random::rd;
std::mt19937 Random::genarator(Random::rd());
std::uniform_real_distribution<double> Random::uniDistribution = std::uniform_real_distribution<double>(0.0, 1.0);

double Random::getUniform() {
    return uniDistribution(genarator);
}

int Random::getRandInt(int bound) {
    return (int) (getUniform() * bound);
}
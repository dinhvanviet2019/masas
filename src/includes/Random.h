#ifndef RANDOM_H
#define RANDOM_H
#include <random>

class Random {
public:
    static double getUniform();
    static int getRandInt(int bound);
private:
    static std::random_device rd;
    static std::mt19937 genarator;
    static std::uniform_real_distribution<double> uniDistribution;
};
#endif
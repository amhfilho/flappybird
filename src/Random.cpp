#include "Random.h"

#include <random>

namespace {
    // Single engine shared by all Random methods
    std::mt19937& Generator() {
        static std::mt19937 generator{std::random_device{}()};
        return generator;
    }
}

float Random::Float(float min, float max) {
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(Generator());
}

double Random::Double(double min, double max) {
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(Generator());
}

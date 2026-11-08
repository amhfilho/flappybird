#pragma once

class Random {
public:
    Random() = delete;

    // Returns a uniformly distributed float in [min, max)
    static float Float(float min, float max);

    // Returns a uniformly distributed double in [min, max)
    static double Double(double min, double max);
};

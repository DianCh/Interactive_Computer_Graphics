#ifndef NOISE_H
#define NOISE_H
#include <la.h>
#include <vector>

class FBM
{
private:
    float H;
    float frequency;
    float lacunarity;
    int octaves;
    std::vector<float> exponents;

    float basis(glm::vec2 point);

public:
    FBM();
    FBM(float H, float frequency, float lacunarity, int octaves);

    float fbm(float x, float z);
};


#endif // NOISE_H

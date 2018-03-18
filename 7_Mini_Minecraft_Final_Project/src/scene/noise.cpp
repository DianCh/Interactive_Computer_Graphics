#include "noise.h"

FBM::FBM()
    : H(1.0), frequency(1.0), lacunarity(2.0), octaves(3), exponents()
{}

FBM::FBM(float H, float frequency, float lacunarity, int octaves)
    : H(H), frequency(frequency), lacunarity(lacunarity), octaves(octaves), exponents()
{}

float FBM::fbm(float x, float z)
{
    x = (int)x / 8 + 8;
    z = (int)z / 8 + 8;

    if(exponents.size() == 0){
        for(unsigned int i = 0; i < octaves; i++){
            exponents.push_back(pow(frequency, -H));
            frequency *= lacunarity;
        }
    }

    float result = -1;
    for(unsigned int i = 0; i < octaves; i++){
        result += basis(glm::vec2(x, z)) * exponents[i];
        x *= lacunarity;
        z *= lacunarity;
    }

    return result;
}

float FBM::basis(glm::vec2 point)
{
    return glm::fract(sin(glm::dot(point, glm::vec2(12.9898, 4.1414))) * 43758.5453);
}

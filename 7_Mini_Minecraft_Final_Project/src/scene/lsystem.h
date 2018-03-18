#ifndef LSYSTEM_H
#define LSYSTEM_H
#include "la.h"
#include <stack>
#include <unordered_map>
#include <vector>
//#include <random>



class Turtle
{
public:
    glm::ivec2 pos;
    glm::vec3 orien;
    float dis;
    float degree;

    Turtle(glm::ivec2 pos, glm::vec3 orien, float dis, float degree);
    Turtle(const Turtle& turtle);

    void moveForward();
    void rotateRight();
    void rotateLeft();
};

class LSystem
{

public:
    typedef void (LSystem::*Rule)(void);
    static std::vector<std::pair<glm::ivec2, glm::ivec2>> generateLinearRiver();
    static std::vector<std::pair<glm::ivec2, glm::ivec2>> generateDeltaRiver();

    LSystem(std::string& axiom, std::unordered_map<char, std::string>& grammars, std::unordered_map<char, Rule> operations, int totalDepth, Turtle turtle);
    std::vector<std::pair<glm::ivec2, glm::ivec2>> generate();
private:
    std::string axiom;
    std::unordered_map<char, std::string> grammars;
    std::unordered_map<char, Rule> operations;
    int totalDepth;
    Turtle turtle;
    std::stack<Turtle> turtleStack;
    std::vector<std::pair<glm::ivec2, glm::ivec2>> lines;
    std::string grammarExpansion();

    void moveForward();
    void rotateRight();
    void rotateLeft();
    void pushState();
    void popState();
    void pass();
};

#endif // LSYSTEM_H

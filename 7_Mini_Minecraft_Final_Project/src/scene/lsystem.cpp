#include "lsystem.h"
#include <iostream>

Turtle::Turtle(glm::ivec2 pos, glm::vec3 orien, float dis, float degree)
    : pos(pos), orien(orien), dis(dis), degree(degree)
{}

Turtle::Turtle(const Turtle &turtle)
    : pos(turtle.pos), orien(turtle.orien), dis(turtle.dis), degree(turtle.degree)
{}

void Turtle::moveForward()
{
    glm::vec3 translation = orien * (dis + (int)(pos[0] + pos[1]) % 5);
    pos += glm::ivec2(translation.x, translation.z);
}

void Turtle::rotateRight()
{
    glm::vec3 up = glm::vec3(0, 1, 0);
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(-degree + (int)(pos[0] + pos[1]) % 4), up);
    orien = glm::vec3(rotation * glm::vec4(orien, 1));
}

void Turtle::rotateLeft()
{
    glm::vec3 up = glm::vec3(0, 1, 0);
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(degree - (int)(pos[0] + pos[1]) % 4), up);
    orien = glm::vec3(rotation * glm::vec4(orien, 1));
}

LSystem::LSystem(std::string &axiom, std::unordered_map<char, std::string> &grammars, std::unordered_map<char, Rule> operations, int totalDepth, Turtle turtle)
    : axiom(axiom), grammars(grammars), operations(operations),totalDepth(totalDepth), turtle(turtle), turtleStack(), lines()
{}

std::string LSystem::grammarExpansion()
{
    std::string current = axiom;
    std::string result;
    for(int i = 0; i < totalDepth; i++){
        result = "";
        for(char& c : current){
            if(grammars.count(c) == 0) result += c;
            else result += grammars[c];
        }
        current = result;
    }
//    std::cout<<result<<std::endl;
    return result;
}

std::vector<std::pair<glm::ivec2, glm::ivec2>> LSystem::generate()
{
    std::string path = grammarExpansion();
    turtleStack = std::stack<Turtle>();
    lines.clear();
    for(char& activity : path){
        (this->*operations[activity])();
    }

    return lines;
}

std::vector<std::pair<glm::ivec2, glm::ivec2>> LSystem::generateLinearRiver()
{
    Turtle turtle = Turtle(glm::ivec2(0, -8), glm::vec3(0, 0, 1), 16, 30);
    std::string axiom = "+F[-F++F]+F--FX";
    std::unordered_map<char, std::string> grammars;
    grammars['X'] = std::string("+F[-F++F]+F--FX");
    std::unordered_map<char, Rule> operations;
    operations['F'] = &LSystem::moveForward;
    operations['+'] = &LSystem::rotateRight;
    operations['-'] = &LSystem::rotateLeft;
    operations['['] = &LSystem::pushState;
    operations[']'] = &LSystem::popState;
    operations['X'] = &LSystem::pass;
    int totalDepth = 10;
    LSystem river = LSystem(axiom, grammars, operations, totalDepth, turtle);
    return river.generate();
}

std::vector<std::pair<glm::ivec2, glm::ivec2>> LSystem::generateDeltaRiver()
{
    Turtle turtle = Turtle(glm::ivec2(-32, 0), glm::vec3(1, 0, 0), 16, 10);
    std::string axiom = "FX";
    std::unordered_map<char, std::string> grammars;
    grammars['X'] = std::string("[-FX]+FX");
    std::unordered_map<char, Rule> operations;
    operations['F'] = &LSystem::moveForward;
    operations['+'] = &LSystem::rotateRight;
    operations['-'] = &LSystem::rotateLeft;
    operations['['] = &LSystem::pushState;
    operations[']'] = &LSystem::popState;
    operations['X'] = &LSystem::pass;
    int totalDepth = 10;
    LSystem river = LSystem(axiom, grammars, operations, totalDepth, turtle);
    return river.generate();
}

void LSystem::moveForward()
{
    glm::vec2 lastPos = turtle.pos;
    turtle.moveForward();
    lines.push_back(std::make_pair(lastPos, turtle.pos));
//    std::cout<<"forward"<<std::endl;
}

void LSystem::rotateRight()
{
    turtle.rotateRight();
//    std::cout<<"right"<<std::endl;
}

void LSystem::rotateLeft()
{
    turtle.rotateLeft();
//    std::cout<<"left"<<std::endl;
}

void LSystem::pushState()
{
    turtleStack.push(Turtle(turtle));
//    std::cout<<"push"<<std::endl;
}

void LSystem::popState()
{
    turtle = turtleStack.top();
    turtleStack.pop();
//    std::cout<<"pop"<<std::endl;
}

void LSystem::pass()
{}


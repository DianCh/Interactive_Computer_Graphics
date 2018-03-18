// written by Dian Chen for CIS 560 homework 02

#include <iostream>
#include "node.h"

///----------------------------------------------------------------------
/// vvvvv Implementing Node class
///----------------------------------------------------------------------

/// Default constructor and parametrized constructor
Node::Node() : ptrPlg(nullptr), color(1.0, 0.0, 0.0), name("") {}

Node::Node(Polygon2D *ptrPlg, glm::vec3 color, QString name) :
    ptrPlg(ptrPlg), color(color), name(name) { setText(0, name); }

/// Destructor
Node::~Node() {}

/// Add a pointer to a new child node to the container
void Node::addAChild(Node *newChild)
{
    ptrChld.push_back(newChild);
    QTreeWidgetItem::addChild(newChild);
}

/// Setters for Node
void Node::setColor(glm::vec3 newColor)
{
    color = newColor;
}

void Node::setGeometry(Polygon2D *newPlg)
{
    ptrPlg = newPlg;
}

void Node::setName(QString newName)
{
    name = newName;
}

/// Getters for Node
Node* Node::getAChild(unsigned int index)
{
    /// Examine if the inquiry index is legal. If the index goes out of
    /// bound then retrieve the last pointer in ptrChld.
    if (index < ptrChld.size())
    {
        return ptrChld[index];
    }
    else
    {
        std::cout << "Inquiry out of bound! Retrieving the last child." << std::endl;
        return ptrChld[ptrChld.size() - 1];
    }
}

std::vector<Node*> Node::getChildren()
{
    return ptrChld;
}

glm::vec3 Node::getColor()
{
    return color;
}

Polygon2D* Node::getGeometry()
{
    return ptrPlg;
}

QString Node::getName()
{
    return name;
}

///----------------------------------------------------------------------
/// ^^^^^ Implementing Node class
///----------------------------------------------------------------------

///----------------------------------------------------------------------
/// vvvvv Implementing TranslateNode class
///----------------------------------------------------------------------

/// Default constructor and parametrized constructor
TranslateNode::TranslateNode() : Node(), trans_x(0), trans_y(0) {}

TranslateNode::TranslateNode(Polygon2D *ptrPlg, glm::vec3 color, QString name, float x, float y) :
    Node(ptrPlg, color, name), trans_x(x), trans_y(y) {}

/// Destructor
TranslateNode::~TranslateNode() {}

/// Overriden to get the matrix corresponding to trans_x and trans_y
glm::mat3 TranslateNode::Transformation()
{
    return glm::mat3(1.0, 0.0, 0.0,
                     0.0, 1.0, 0.0,
                     trans_x, trans_y, 1.0);
}

/// Setter for TranslateNode
void TranslateNode::setTranslateX(float x)
{
    trans_x = x;
}

void TranslateNode::setTranslateY(float y)
{
    trans_y = y;
}

/// Getters for TranslateNode
float TranslateNode::getTranslateX()
{
    return trans_x;
}

float TranslateNode::getTranslateY()
{
    return trans_y;
}

/// Slots
//void TranslateNode::slot_disableRSBox()
//{
//    emit disableRSBox();
//}

//void slot_addNewTransNode()
//{

//}

///----------------------------------------------------------------------
/// ^^^^^ Implementing TranslateNode class
///----------------------------------------------------------------------

///----------------------------------------------------------------------
/// vvvvv Implementing RotateNode class
///----------------------------------------------------------------------

/// Default constructor and parametrized constructor
RotateNode::RotateNode() : Node(), angle(0) {}

RotateNode::RotateNode(Polygon2D *ptrPlg, glm::vec3 color, QString name, float theta) :
    Node(ptrPlg, color, name), angle(theta) {}

/// Destructor
RotateNode::~RotateNode() {}

/// Overriden to get the matrix corresponding to angle
glm::mat3 RotateNode::Transformation()
{
    float degree = angle / 180.0 * 3.1415;
    return glm::mat3(glm::cos(degree), glm::sin(degree), 0.0,
                   - glm::sin(degree), glm::cos(degree), 0.0,
                     0.0, 0.0, 1.0);
}

/// Setter for RotateNode
void RotateNode::setRotate(float theta)
{
    angle = theta;
}

/// Getter for RotateNode
float RotateNode::getAngle()
{
    return angle;
}

///----------------------------------------------------------------------
/// ^^^^^ Implementing RotateNode class
///----------------------------------------------------------------------

///----------------------------------------------------------------------
/// vvvvv Implementing ScaleNode class
///----------------------------------------------------------------------

/// Default constructor and parametrized constructor
ScaleNode::ScaleNode() : Node(), scale_x(1.0), scale_y(1.0) {}

ScaleNode::ScaleNode(Polygon2D *ptrPlg, glm::vec3 color, QString name, float x, float y) :
    Node(ptrPlg, color, name), scale_x(x), scale_y(y) {}

/// Destructor
ScaleNode::~ScaleNode() {}

/// Overriden to get the matrix corresponding to scale_x and scale_y
glm::mat3 ScaleNode::Transformation()
{
    return glm::mat3(scale_x, 0.0, 0.0,
                     0.0, scale_y, 0.0,
                     0.0, 0.0, 1.0);
}

/// Setter for ScaleNode
void ScaleNode::setScaleX(float x)
{
    scale_x = x;
}

void ScaleNode::setScaleY(float y)
{
    scale_y = y;
}

/// Getters for ScaleNode
float ScaleNode::getScaleX()
{
    return scale_x;
}

float ScaleNode::getScaleY()
{
    return scale_y;
}

///----------------------------------------------------------------------
/// ^^^^^ Implementing ScaleNode class
///----------------------------------------------------------------------

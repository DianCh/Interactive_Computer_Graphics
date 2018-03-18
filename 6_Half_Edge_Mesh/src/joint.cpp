#include "joint.h"

int Joint::lastAssigned = -1;

Joint::Joint() : id(Joint::lastAssigned + 1), parent(nullptr), children(),
    position(glm::vec4(0, 0, 0, 1)), rotation(glm::quat(1, 0, 0, 0)), bindMatrix(glm::mat4(1.0f)),
    color(glm::vec4(0, 1, 1, 1))
{
    lastAssigned += 1;
}

Joint::Joint(Joint *parent) : id(Joint::lastAssigned + 1), parent(parent), children(),
    position(glm::vec4(0, 0, 0, 1)), rotation(glm::quat(1, 0, 0, 0)), bindMatrix(glm::mat4(1.0f)),
    color(glm::vec4(0, 1, 1, 1))
{
    lastAssigned += 1;
}

Joint::Joint(Joint *parent, const glm::vec4 &pos, const glm::quat &rot) :
    id(Joint::lastAssigned + 1), parent(parent), children(), position(pos), rotation(rot), bindMatrix(glm::mat4(1.0f)),
    color(glm::vec4(0, 1, 1, 1))
{
    lastAssigned += 1;
}

Joint::~Joint()
{
    lastAssigned -= 1;
}

void Joint::setName(QString name)
{
    this->name = name;
    setText(0, this->name);
}

void Joint::setParent(Joint *parent)
{
    this->parent = parent;
}

void Joint::addAChild(Joint *child)
{
    children.push_back(child);
    QTreeWidgetItem::addChild(child);
}

void Joint::setPosition(glm::vec4 &pos)
{
    position = pos;
}

void Joint::setRotation(glm::quat &rot)
{
    rotation = rot;
}

void Joint::setBindMatrix(glm::mat4 &bind)
{
    bindMatrix = bind;
}

void Joint::setColor(glm::vec4 &col)
{
    color = col;
}

int Joint::getID() const
{
    return id;
}

QString Joint::getName() const
{
    return name;
}

Joint* Joint::getParent() const
{
    return parent;
}

std::vector<Joint*> Joint::getChildren() const
{
    return children;
}

Joint* Joint::getChildAt(int index) const
{
    if (index < children.size())
    {
        return children[index];
    }
    else
    {
        return children[children.size() - 1];
    }
}

glm::vec4 Joint::getPosition() const
{
    return position;
}

glm::quat Joint::getRotation() const
{
    return rotation;
}

glm::mat4 Joint::getBindMatrix() const
{
    return bindMatrix;
}

glm::vec4 Joint::getColor() const
{
    return color;
}

glm::mat4 Joint::getLocalTransformation() const
{
    glm::mat4 T(glm::vec4(1, 0, 0, 0),
                glm::vec4(0, 1, 0, 0),
                glm::vec4(0, 0, 1, 0),
                position);

    glm::mat4 R = glm::toMat4(rotation);

    return T * R;
}

glm::mat4 Joint::getOverallTransformation() const
{
    if (parent == nullptr)
    {
        return getLocalTransformation();
    }
    else
    {
        return parent->getOverallTransformation() * getLocalTransformation();
    }
}

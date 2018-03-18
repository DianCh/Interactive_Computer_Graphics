#pragma once

#include <QTreeWidgetItem>
#include "la.h"

class Joint : public QTreeWidgetItem
{
private:
    int id;
    QString name;
    Joint *parent;
    std::vector<Joint*> children;
    glm::vec4 position;
    glm::quat rotation;
    glm::mat4 bindMatrix;

    glm::vec4 color;

    static int lastAssigned;

public:
    Joint();
    Joint(Joint *parent);
    Joint(Joint *parent, const glm::vec4 &pos, const glm::quat &rot);
    ~Joint();

    void setName(QString name);
    void setParent(Joint *parent);
    void addAChild(Joint *child);
    void setPosition(glm::vec4 &pos);
    void setRotation(glm::quat &rot);
    void setBindMatrix(glm::mat4 &bind);
    void setColor(glm::vec4 &col);

    int getID() const;
    QString getName() const;
    Joint* getParent() const;
    std::vector<Joint*> getChildren() const;
    Joint* getChildAt(int index) const;
    glm::vec4 getPosition() const;
    glm::quat getRotation() const;
    glm::mat4 getBindMatrix() const;
    glm::vec4 getColor() const;

    glm::mat4 getLocalTransformation() const;
    glm::mat4 getOverallTransformation() const;

};

#ifndef NODE_H
#define NODE_H

#include "scene/polygon.h"
#include "la.h"
#include <vector>

#include <QTreeWidgetItem>


class Node : public QTreeWidgetItem
{

private:
    Polygon2D *ptrPlg;          // A pointer to the geometry
    glm::vec3 color;            // A vec3 with each element ranging [0, 1]
    QString name;               // Name for the node
    std::vector<Node*> ptrChld; // A vector container for pointers of children

public:
    /// Default constructor and parametrized constructor
    Node();
    Node(Polygon2D *ptrPlg, glm::vec3 color, QString name);

    /// Virtual destructor
    virtual ~Node();

    /// Expected to be overriden to get the specified transformation matrix
    virtual glm::mat3 Transformation() = 0;

    /// Add a pointer to a new child node to the container
    void addAChild(Node *newChild);

    /// Setters
    void setColor(glm::vec3 newColor);
    void setGeometry(Polygon2D *newPlg);
    void setName(QString newName);

    /// Getters
    Node* getAChild(unsigned int index);
    std::vector<Node*> getChildren();
    glm::vec3 getColor();
    Polygon2D* getGeometry();
    QString getName();
};

class TranslateNode : public Node
{

private:
    float trans_x, trans_y;     // Specify the translation along x and y axes

public:
    /// Default constructor and parametrized constructor
    TranslateNode();
    TranslateNode(Polygon2D *ptrPlg, glm::vec3 color, QString name, float x, float y);

    /// Virtual destructor
    ~TranslateNode() override;

    /// Overriden to get the matrix corresponding to trans_x and trans_y
    glm::mat3 Transformation() override;

    /// Setter
    void setTranslateX(float x);
    void setTranslateY(float y);

    /// Getters
    float getTranslateX();
    float getTranslateY();

//public slots:
//    void slot_disableRSBox();
//    void slot_addNewTransNode();

//signals:
//    void disableRSBox();
};

class RotateNode : public Node
{

private:
    float angle;                // The rotated angle in degrees (positive for counterclockwise)
public:
    /// Default constructor and parametrized constructor
    RotateNode();
    RotateNode(Polygon2D *ptrPlg, glm::vec3 color, QString name, float theta);

    /// Virtual destructor
    ~RotateNode() override;

    /// Overriden to get the matrix corresponding to angle
    glm::mat3 Transformation() override;

    /// Setter
    void setRotate(float theta);

    /// Getter
    float getAngle();
};

class ScaleNode : public Node
{

private:
    float scale_x, scale_y;     // Specify the scaling factor along x and y axes
public:
    /// Default constructor and parametrized constructor
    ScaleNode();
    ScaleNode(Polygon2D *ptrPlg, glm::vec3 color, QString name, float x, float y);

    /// Virtual destructor
    ~ScaleNode() override;

    /// Overriden to get the matrix corresponding to scale_x and scale_y
    glm::mat3 Transformation() override;

    /// Setter
    void setScaleX(float x);
    void setScaleY(float y);

    /// Getters
    float getScaleX();
    float getScaleY();
};

#endif // NODE_H


#pragma once

#include "joint.h"
#include "drawable.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

class Skeleton : public Drawable
{
private:
    Joint* rootJoint;
    std::vector<Joint*> jointSpheres;

    static std::vector<glm::vec4> wireframes;   // 60 vertex positions for three wireframes

public:
    Skeleton(GLWidget277 *context);

    Joint* getRootJoint() const;
    std::vector<Joint*> getJointSpheres() const;

    void create();                           // Set up the vbo

    GLenum drawMode();                       // Overriden to return GL_LINES

    void clearSkeleton();                    // Clear all existing skeleton joints on heap
    void setupJSON(QString filename);        // Read in JSON files and create joints on the heap
    Joint* setupOneJoint(QJsonObject &joint, Joint *parent);
                                             // Utility function used in setupJSON
    void setupBindMatrices();                // Set up all the bind matrices for joints

    static void initWireframes();            // Set up the initial positions for wireframes
};

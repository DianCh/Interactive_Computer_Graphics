#include "skeleton.h"
#include <iostream>

std::vector<glm::vec4> Skeleton::wireframes;

Skeleton::Skeleton(GLWidget277 *context) : Drawable(context), rootJoint(nullptr), jointSpheres() {}

void Skeleton::initWireframes()
{
    wireframes.clear();

    float theta;

    // Wireframe around X axis
    for (int i = 0; i < 20; ++i)
    {
        theta = M_PI / 10 * i;

        wireframes.push_back(glm::vec4(0, 0.5 * cosf(theta), 0.5 * sinf(theta), 1));
    }

    // Wireframe around Y axis
    for (int i = 0; i < 20; ++i)
    {
        theta = M_PI / 10 * i;

        wireframes.push_back(glm::vec4(0.5 * sinf(theta), 0, 0.5 * cosf(theta), 1));
    }

    // Wireframe around Z axis
    for (int i = 0; i < 20; ++i)
    {
        theta = M_PI / 10 * i;

        wireframes.push_back(glm::vec4(0.5 * cosf(theta), 0.5 * sinf(theta), 0, 1));
    }
}

Joint* Skeleton::getRootJoint() const
{
    return rootJoint;
}

std::vector<Joint*> Skeleton::getJointSpheres() const
{
    return jointSpheres;
}

void Skeleton::create()
{
    // Construct the containers for vbo data
    std::vector<GLuint> idx;
    std::vector<glm::vec4> pos;
    std::vector<glm::vec4> col;

    // Total number of line segments to draw & vertices used
    int N = jointSpheres.size();
    count = N * 60 * 2 + (N - 1) * 2;

    /// Step 1: Populate data for wireframes
    int sphereCount = 0;

    // Iterate over all spheres to set up the wireframes
    for (Joint *sphere : jointSpheres)
    {
        // Get the overall transformation & color of this sphere
        glm::mat4 transform = sphere->getOverallTransformation();
        glm::vec4 color = sphere->getColor();

        // Populate position data and color data
        for (int i = 0; i < wireframes.size(); ++i)
        {
            pos.push_back(transform * wireframes[i]);
            col.push_back(color);
        }

        // Populate index data
        for (int i = 0; i < 19; ++i)
        {
            idx.push_back(GLuint(i + sphereCount * 60));
            idx.push_back(GLuint(i + 1 + sphereCount * 60));
        }
        idx.push_back(GLuint(19 + sphereCount * 60));
        idx.push_back(GLuint(sphereCount * 60));

        for (int i = 20; i < 39; ++i)
        {
            idx.push_back(GLuint(i + sphereCount * 60));
            idx.push_back(GLuint(i + 1 + sphereCount * 60));
        }
        idx.push_back(GLuint(39 + sphereCount * 60));
        idx.push_back(GLuint(20 + sphereCount * 60));

        for (int i = 40; i < 59; ++i)
        {
            idx.push_back(GLuint(i + sphereCount * 60));
            idx.push_back(GLuint(i + 1 + sphereCount * 60));
        }
        idx.push_back(GLuint(59 + sphereCount * 60));
        idx.push_back(GLuint(40 + sphereCount * 60));

        // Count of sphere up by 1
        sphereCount += 1;
    }

    /// Step 2: Populate data for line segments connecting parents and children joints
    // Set predetermined colors
    glm::vec4 parentColor(0.92, 0.89, 0.41, 1);
    glm::vec4 childColor(0.5, 0, 1, 1);

    // Iterate over all joints that have a parent. In my way of setting up the container,
    // the first joint must be the root so we can explicitly skip it by starting at index 1
    for (int i = 1; i < jointSpheres.size(); ++i)
    {
        Joint *parentJoint = jointSpheres[i]->getParent();
        Joint *thisJoint = jointSpheres[i];

        glm::mat4 parentTransform = parentJoint->getOverallTransformation();
        glm::mat4 thisTransform = thisJoint->getOverallTransformation();

        glm::vec4 parentPosition = parentTransform[3];
        glm::vec4 thisPosition = thisTransform[3];

        pos.push_back(parentPosition);
        pos.push_back(thisPosition);

        col.push_back(parentColor);
        col.push_back(childColor);

        idx.push_back(GLuint(60 * N + 2 * (i - 1)));
        idx.push_back(GLuint(60 * N + 2 * (i - 1) + 1));
    }

    /// Step 3: Bind and send data to buffer
    generateIdx();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx.size() * sizeof(GLuint), idx.data(), GL_STATIC_DRAW);

    generatePos();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    mp_context->glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(glm::vec4), pos.data(), GL_STATIC_DRAW);

    generateCol();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    mp_context->glBufferData(GL_ARRAY_BUFFER, col.size() * sizeof(glm::vec4), col.data(), GL_STATIC_DRAW);

}

GLenum Skeleton::drawMode()
{
    return GL_LINES;
}

void Skeleton::clearSkeleton()
{
    for (Joint *joint : jointSpheres)
    {
        delete joint;
        // We only need to delete the root joint (which is the first one given the way
        // I set up the container), and each joint all the way down the hierachy will
        // be deleted because children are store in std::vector, which automatically
        // calls delete to each pointer stored in it. This is automatically a recursive
        // deletion, quite unlike the deletions in Mesh class.
        break;
    }

    jointSpheres.clear();
}

void Skeleton::setupJSON(QString filename)
{
    // Clear all existing joint elements
    clearSkeleton();

    // Open and read a file
    QFile file(filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString plainText = file.readAll();
    file.close();
    QJsonDocument content = QJsonDocument::fromJson(plainText.toUtf8());
    QJsonObject root = content.object();
    QJsonValue rootValue = root.value(QString("root"));
    root = rootValue.toObject();

    // Start with root joint
    rootJoint = setupOneJoint(root, nullptr);
}

Joint* Skeleton::setupOneJoint(QJsonObject &joint, Joint *parent)
{
    // Create a new joint on the heap
    Joint *newJoint = new Joint(parent);
    jointSpheres.push_back(newJoint);

    // Set up the member variables for this joint
    QJsonValue name = joint.value("name");
    newJoint->setName(QString::number(newJoint->getID()) + QString(": ") + name.toString());

    QJsonArray pos = (joint.value("pos")).toArray();
    glm::vec4 position((float)pos[0].toDouble(), (float)pos[1].toDouble(), (float)pos[2].toDouble(), 1);
    newJoint->setPosition(position);

    QJsonArray rot = (joint.value("rot")).toArray();
    float angle = (float)rot[0].toDouble();
    glm::vec3 axis((float)rot[1].toDouble(), (float)rot[2].toDouble(), (float)rot[3].toDouble());
    glm::quat rotation = glm::angleAxis(angle, axis);
    newJoint->setRotation(rotation);

    QJsonArray children = (joint.value("children")).toArray();
    for (int i = 0; i < children.size(); ++i)
    {
        QJsonObject childObject = children[i].toObject();
        newJoint->addAChild(setupOneJoint(childObject, newJoint));
    }

    return newJoint;
}

void Skeleton::setupBindMatrices()
{
    for (Joint *joint : jointSpheres)
    {
        glm::mat4 bind = glm::inverse(joint->getOverallTransformation());
        joint->setBindMatrix(bind);
    }
}

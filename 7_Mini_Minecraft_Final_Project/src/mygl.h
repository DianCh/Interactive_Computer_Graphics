#ifndef MYGL_H
#define MYGL_H

#include <openglcontext.h>
#include <utils.h>
#include <shaderprogram.h>
#include <scene/worldaxes.h>
#include "camera.h"
#include <scene/terrain.h>
#include "player.h"
#include <scene/overlay.h>
#include "athread.h"
#include <scene/gui/itemframe.h>
#include <scene/gui/inventory.h>
#include <scene/gui/itemcube.h>
#include <scene/gui/listholder.h>

#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QDateTime>
#include <QMutex>
#include <QThreadPool>


class MyGL : public OpenGLContext
{
    Q_OBJECT
private:
    WorldAxes* mp_worldAxes; // A wireframe representation of the world axes. It is hard-coded to sit centered at (32, 128, 32).
    ShaderProgram* mp_progLambert;// A shader program that uses lambertian reflection
    ShaderProgram* mp_progFlat;// A shader program that uses "flat" reflection (no shadowing at all)
    ShaderProgram* mp_progOverlay; // A shader program that renders overlay quadrangle
    ShaderProgram* mp_progFrame;    // A shader program that renders inventory item frame
    ShaderProgram* mp_progCube;  // A shader program that renders inventory item

    QSound* mp_waterPlayer;
    QSound* mp_walkPlayer;
    QSound* mp_windPlayer;
    bool m_windTempStop;

    ShaderProgram* mp_progFullScreen;
    HPBar* mp_HPBar;
    Overlay* mp_gameoverSign;
    bool mp_survivalMode;
    bool mp_playerDead;

    GLuint m_frameBuffer;
    GLuint m_renderedTexture;
    GLuint m_depthRenderBuffer;
    Overlay* mp_postprocessedImage;

    GLuint vao; // A handle for our vertex array object. This will store the VBOs created in our geometry classes.
                // Don't worry too much about this. Just know it is necessary in order to render geometry.


    Terrain* mp_terrain;
    Player* mp_player;
    Overlay* mp_overlay;
    ItemFrame* mp_itemFrame;
    ItemFrame* mp_highlightFrame;
    ListHolder* mp_listHolder;
    ItemCube* mp_itemCube;
    ItemCube* mp_highlightCube;
    Inventory* mp_inventory;
    Camera* mp_itemCamera;
    ItemFrame* m_renderFrameList[9];
    bool m_showInventory;

    std::vector<Chunk*> expandChunkList;
    std::vector<glm::ivec2> processingChunkCoordList;
    QMutex* mp_mutex;

    /// Timer linked to timerUpdate(). Fires approx. 60 times per second
    QTimer timer;
    qint64 m_lastTime; // store the time of last frame

    void MoveMouseToCenter(); // Forces the mouse position to the screen's center. You should call this
                              // from within a mouse move event after reading the mouse movement so that
                              // your mouse stays within the screen bounds and is always read.

    int m_time;

public:
    explicit MyGL(QWidget *parent = 0);
    ~MyGL();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void GLDrawChunks();

    void terrainExtand();
    void addBlock(BlockType blockType);
    BlockType removeBlock();
    void updateRenderFrameList();

    void updateHP(int HP);
    void resetSurvival();
    void resetGame();

    void createRenderBuffers();
    void performPostprocessRenderPass();

protected:
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

private slots:
    /// Slot that gets called ~60 times per second
    void timerUpdate();
};


#endif // MYGL_H

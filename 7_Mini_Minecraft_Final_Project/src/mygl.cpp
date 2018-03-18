#include "mygl.h"
#include <la.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>


MyGL::MyGL(QWidget *parent)
    : OpenGLContext(parent),
      m_frameBuffer(-1), m_renderedTexture(-1), m_depthRenderBuffer(-1), mp_postprocessedImage(new Overlay(this)),
      mp_worldAxes(new WorldAxes(this)), mp_progOverlay(new ShaderProgram(this)), mp_progFrame(new ShaderProgram(this)),
      mp_progLambert(new ShaderProgram(this)), mp_progFlat(new ShaderProgram(this)), mp_progCube(new ShaderProgram(this)),
      mp_terrain(new Terrain(this)), mp_player(new Player()), mp_overlay(new Overlay(this)), mp_mutex(new QMutex()), m_time(0),
      mp_itemFrame(new ItemFrame(this)), mp_inventory(new Inventory(this)), mp_itemCube(new ItemCube(this)), mp_highlightCube(new ItemCube(this)),
      mp_itemCamera(new Camera()), m_showInventory(false), mp_listHolder(new ListHolder(this)), mp_progFullScreen(new ShaderProgram(this)), mp_HPBar(new HPBar(this)), mp_gameoverSign(new Overlay(this)),
      mp_survivalMode(false), mp_playerDead(false)
{
    // Connect the timer to a function so that when the timer ticks the function is executed
    connect(&timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    // Tell the timer to redraw 60 times per second
    timer.start(16);
    setFocusPolicy(Qt::ClickFocus);

    setMouseTracking(true); // MyGL will track the mouse's movements even if a mouse button is not pressed
    setCursor(Qt::CrossCursor); // Make the cursor invisible

    mp_player->m_middleCursor = glm::ivec2(309, 216);
    m_lastTime = 0;

    expandChunkList.clear();
    processingChunkCoordList.clear();

    mp_highlightFrame = new ItemFrame(this);
    mp_highlightFrame->m_selected = 1;

    mp_itemCamera->eye = glm::vec3(20, 15, 20);
    mp_itemCamera->ref = glm::vec3(0, 0, 0);
    mp_itemCamera->RecomputeAttributes();

    mp_waterPlayer = new QSound(":/sound/water.wav");
    mp_waterPlayer->setLoops(QSound::Infinite );
    mp_walkPlayer = new QSound(":/sound/footstep.wav");
    mp_walkPlayer->setLoops(QSound::Infinite );
    mp_windPlayer = new QSound(":/sound/wind.wav");
    mp_windPlayer->setLoops(QSound::Infinite );

}

MyGL::~MyGL()
{
    makeCurrent();
    glDeleteVertexArrays(1, &vao);

    delete mp_worldAxes;
    delete mp_progLambert;
    delete mp_progFlat;
    delete mp_progOverlay;
    delete mp_progFrame;
    delete mp_progCube;
    delete mp_terrain;
    delete mp_player;
    delete mp_overlay;
    delete mp_itemFrame;
    delete mp_highlightFrame;
    delete mp_itemCube;
    delete mp_highlightCube;
    delete mp_highlightFrame;
    delete mp_listHolder;
    delete mp_inventory;
    delete mp_itemCamera;

    delete mp_HPBar;
    delete mp_gameoverSign;
    delete mp_progFullScreen;

    delete mp_waterPlayer;
    delete mp_walkPlayer;
    delete mp_windPlayer;
}


void MyGL::MoveMouseToCenter()
{
    QCursor::setPos(this->mapToGlobal(QPoint(width() / 2, height() / 2)));
}

void MyGL::initializeGL()
{
    // Create an OpenGL context using Qt's QOpenGLFunctions_3_2_Core class
    // If you were programming in a non-Qt context you might use GLEW (GL Extension Wrangler)instead
    initializeOpenGLFunctions();
    // Print out some information about the current OpenGL context
    debugContextVersion();

    // Set a few settings/modes in OpenGL rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // Set the size with which points should be rendered
    glPointSize(5);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(0.37f, 0.74f, 1.0f, 1);

    // Added in milestone 2. Enable alpha channel
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    glGenVertexArrays(1, &vao);

    createRenderBuffers();

    //Create the instance of Axes only
    mp_worldAxes->create();

    // Set up the texture UVs for blocks for later drawing
    Chunk::initializeUVData();

    mp_overlay->create();
    mp_itemFrame->create();
    mp_highlightFrame->create();
    mp_itemCube->create();
    mp_highlightCube->create();
    mp_listHolder->create();

    for(int i = 0; i < 9; ++i) {
        m_renderFrameList[i] = new ItemFrame(this);
        m_renderFrameList[i]->create();
    }

    // Create and load textures
    mp_terrain->initializeTexture();
    mp_terrain->loadTexture();

    // Create one test chunk
    mp_terrain->initializeTerrian();
//    mp_terrain->initializeTestTerrain();
    mp_terrain->createChunks();

    // Create and set up the diffuse shader
    mp_progLambert->create(":/glsl/lambert.vert.glsl", ":/glsl/lambert.frag.glsl");
    // Create and set up the flat lighting shader
    mp_progFlat->create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");

    mp_progOverlay->create(":/glsl/overlay.vert.glsl", ":/glsl/overlay.frag.glsl");

    mp_progFrame->create(":/glsl/itemFrame.vert.glsl", ":/glsl/itemFrame.frag.glsl");

    mp_progCube->create(":/glsl/itemCube.vert.glsl", ":/glsl/itemCube.frag.glsl");

    mp_progLambert->setModelMatrix(glm::mat4());

    // Set a color with which to draw geometry since you won't have one
    // defined until you implement the Node classes.
    // This makes your geometry render green.
    mp_progLambert->setGeometryColor(glm::vec4(0,1,0,1));

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
//    vao.bind();

    glm::mat4 itemViewProj = mp_itemCamera->getViewProj();
    mp_progCube->setViewProjMatrix(itemViewProj);

    mp_HPBar->create();
    mp_gameoverSign->setDepth(0.008);
    mp_gameoverSign->createWithDeathTexture();

    mp_postprocessedImage->setDepth(0.004);
    mp_postprocessedImage->createWithFullScreenTexture();

    mp_progFullScreen->create(":/glsl/fullscreen.vert.glsl", ":/glsl/fullscreen.frag.glsl");
    mp_progFullScreen->setDeformWater(0);
    mp_progFullScreen->setDeformLava(0);

    mp_progOverlay->setTickingFlag(0);

    glBindVertexArray(vao);

}

void MyGL::resizeGL(int w, int h)
{
    //This code sets the concatenated view and perspective projection matrices used for
    //our scene's camera view.
    glm::mat4 viewproj = mp_player->mp_camera->getViewProj();   

    // Upload the view-projection matrix to our shaders (i.e. onto the graphics card)

    mp_progLambert->setViewProjMatrix(viewproj);
    mp_progFlat->setViewProjMatrix(viewproj);
    mp_progFrame->setDimensions(glm::ivec2(w, h));


    printGLErrorLog();
}


// MyGL's constructor links timerUpdate() to a timer that fires 60 times per second.
// We're treating MyGL as our game engine class, so we're going to use timerUpdate
void MyGL::timerUpdate()
{

    if (m_time % 100 == 0 && mp_survivalMode && !mp_playerDead)
    {
        updateHP(mp_player->m_HP - 1);

        mp_HPBar->destroy();
        mp_HPBar->create();

        if (mp_player->m_HP < 1)
        {
            mp_playerDead = true;
        }

    }


    // calculate delta t
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    if(m_lastTime == 0) {
        m_lastTime = currentTime;
        return;
    }
    qint64 delta_millisec = currentTime - m_lastTime;
    float delta_seconds = delta_millisec * 0.001;
    m_lastTime = currentTime;


    // flags for playing sound
    bool lastSwimState = mp_player->m_swimming;
    bool lastWalkSate = mp_player->m_walking;
    bool lastFlyState = mp_player->m_fly;
    bool lastLiquidState = false;
    if(mp_player->m_inLava || mp_player->m_inLava) {
        lastLiquidState = true;
    } else {
        lastLiquidState = false;
    }

    // update player's position
    mp_player->updatePlayerPos(delta_seconds, mp_terrain);

    // =================play sound==================================
    if(lastSwimState != mp_player->m_swimming) {
        if(lastSwimState == true && mp_player->m_swimming == false) {
            mp_waterPlayer->stop();
        }
        if(lastSwimState == false && mp_player->m_swimming == true) {
            mp_waterPlayer->play();
        }
    }

    if(lastWalkSate != mp_player->m_walking) {
        if(lastWalkSate == true && mp_player->m_walking == false) {
            mp_walkPlayer->stop();
        }
        if(lastWalkSate == false && mp_player->m_walking == true) {
            mp_walkPlayer->play();
        }
    }

    if(lastFlyState != mp_player->m_fly) {
        if(lastFlyState == true && mp_player->m_fly == false) {
            mp_windPlayer->stop();
        }
        if(lastFlyState == false && mp_player->m_fly == true) {
            mp_windPlayer->play();
        }
    }

      bool lastStopFlag = m_windTempStop;
      if(mp_player->m_inLava || mp_player->m_inWater) {
          m_windTempStop = true;
          mp_windPlayer->stop();
      } else {
          m_windTempStop = false;
      }

      if(lastStopFlag != m_windTempStop) {
          if(lastStopFlag == true &&m_windTempStop == false) {
              mp_windPlayer->play();
          }
          if(lastStopFlag == false &&m_windTempStop == true) {
              mp_windPlayer->stop();
          }
      }

      //================================================================

    // Determine if is in "destroy" mode
    if (mp_player->m_destroy)
    {
        std::vector<Chunk*> modifyChunks = mp_player->destroySurroundings(mp_terrain);

        for (Chunk* aChunk : modifyChunks)
        {
            aChunk->destroy();
            aChunk->create();
        }
    }

    terrainExtand();
    if(expandChunkList.size() > 0) {
        for(Chunk* newChunk: expandChunkList) {
            glm::ivec2 globalXZ = newChunk->getGlobalCords();
            mp_terrain->chunks[ivecXZ(globalXZ)] = newChunk;
            newChunk->create();
            if (newChunk->getEast() != nullptr)
            {
                newChunk->getEast()->destroy();
                newChunk->getEast()->create();
            }
            if (newChunk->getSouth() != nullptr)
            {
                newChunk->getSouth()->destroy();
                newChunk->getSouth()->create();
            }
            if (newChunk->getWest() != nullptr)
            {
                newChunk->getWest()->destroy();
                newChunk->getWest()->create();
            }
            if (newChunk->getNorth() != nullptr)
            {
                newChunk->getNorth()->destroy();
                newChunk->getNorth()->create();
            }
        }
        expandChunkList.clear();
        processingChunkCoordList.clear();
    }

    // Clear flags for in-water or in-lava deformation
    mp_progFullScreen->setDeformWater(0);
    mp_progFullScreen->setDeformLava(0);

    if(mp_player->m_inWater) {
        mp_overlay->m_color = glm::vec3(0, 0, 1);
        mp_overlay->destroy();
        mp_overlay->create();
        mp_progFullScreen->setDeformWater(1);
    }
    if(mp_player->m_inLava) {
        mp_overlay->m_color = glm::vec3(1, 0, 0);
        mp_overlay->destroy();
        mp_overlay->create();
        mp_progFullScreen->setDeformLava(1);
    }

    for(int i = 0; i < mp_inventory->getListSize(); ++i) {
        if(i == mp_inventory->m_selectedListIdx - 1) {
            m_renderFrameList[i]->m_selected = 1;
        } else {
            m_renderFrameList[i]->m_selected = 0;
        }
        m_renderFrameList[i]->destroy();
        m_renderFrameList[i]->create();
    }

    update();
}

// This function is called whenever update() is called.
// MyGL's constructor links update() to a timer that fires 60 times per second,
// so paintGL() called at a rate of 60 frames per second.
void MyGL::paintGL()
{
    // Render to our framebuffer rather than the viewport
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
    // Render on the whole framebuffer, complete from the lower left corner to the upper right
    glViewport(0,0,this->devicePixelRatio() * this->width(),this->devicePixelRatio() * this->height());
    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mp_progFlat->setViewProjMatrix(mp_player->mp_camera->getViewProj());
    mp_progLambert->setViewProjMatrix(mp_player->mp_camera->getViewProj());
    mp_progLambert->setModelMatrix(glm::mat4(1.0f));

    mp_progLambert->setEyePos(mp_player->getCameraPos());

    mp_terrain->bindTexture();

    GLDrawChunks();
    performPostprocessRenderPass();

    mp_terrain->bindTexture();

    mp_progLambert->setTime(m_time);
    mp_progFlat->setTime(m_time);
    mp_progOverlay->setTime(m_time);
    mp_progFullScreen->setTime(m_time);
    m_time++;

    glDisable(GL_DEPTH_TEST);
    mp_progFlat->setModelMatrix(glm::mat4());
    mp_progFlat->draw(*mp_worldAxes);

    if(mp_player->m_inWater || mp_player->m_inLava) {
        mp_progOverlay->draw(*mp_overlay);
    }

    if(m_showInventory) {
        //====================draw list holder=============================
        mp_progOverlay->draw(*mp_listHolder);

        //====================draw item frames=============================
        glm::mat4 frameModelMatrix = glm::mat4();

        glm::mat4 translationMatrix = glm::translate(glm::mat4(), glm::vec3(-5.2, -3.5, 0));
        frameModelMatrix = translationMatrix *frameModelMatrix;

        glm::mat4 biggerFrameMat = glm::mat4();
        for(int i = 0; i < 9; ++i) {
            glm::mat4 translateM = glm::translate(glm::mat4(), glm::vec3(1.1, 0, 0));
            frameModelMatrix = translateM * frameModelMatrix;
            mp_progFrame->setModelMatrix(frameModelMatrix);

            ItemFrame* currentFrame = m_renderFrameList[i];
            if(i == mp_inventory->m_selectedListIdx - 1) {
                biggerFrameMat = frameModelMatrix;
                continue;
            }

            // draw normal frame
            mp_progFrame->draw(*currentFrame);
        }
        //darw highlited frame
        if(mp_inventory->m_selectedListIdx != -1) {
            mp_progFrame->setModelMatrix(biggerFrameMat);
            mp_progFrame->draw(*(m_renderFrameList[mp_inventory->m_selectedListIdx - 1]));

         }
    }

    if (mp_survivalMode)
    {
        if (mp_player->m_HP < 4)
        {
            mp_progOverlay->setTickingFlag(1);
        }
        mp_progOverlay->draw(*mp_HPBar);
        mp_progOverlay->setTickingFlag(0);
    }

    if (mp_survivalMode && mp_playerDead)
    {
        mp_progFullScreen->draw(*mp_gameoverSign);
    }

    glEnable(GL_DEPTH_TEST);

}


void MyGL::GLDrawChunks()
{
    QHash<ivecXZ, Chunk*>::iterator iter;

    for (iter = mp_terrain->chunks.begin(); iter != mp_terrain->chunks.end(); ++iter)
    {
        Chunk *thisChunk = iter.value();
        mp_progLambert->draw(*thisChunk);
    }
    for (iter = mp_terrain->chunks.begin(); iter != mp_terrain->chunks.end(); ++iter)
    {
        Chunk *thisChunk = iter.value();
        mp_progLambert->draw(*(thisChunk->getLiquids()));
    }

}

void MyGL::keyPressEvent(QKeyEvent *e)
{
    float amount = 2.0f;
    if(e->modifiers() & Qt::ShiftModifier){
        amount = 10.0f;
    }
    // http://doc.qt.io/qt-5/qt.html#Key-enum
    // This could all be much more efficient if a switch
    // statement were used, but I really dislike their
    // syntax so I chose to be lazy and use a long
    // chain of if statements instead

    if (e->key() == Qt::Key_Escape) {
        QApplication::quit();
    } else if (!mp_playerDead)
    {
        if (e->key() == Qt::Key_W) {
            mp_player->handleKeyPressEvent(e);
        } else if (e->key() == Qt::Key_S) {
            mp_player->handleKeyPressEvent(e);
        } else if (e->key() == Qt::Key_D) {
            mp_player->handleKeyPressEvent(e);
        } else if (e->key() == Qt::Key_A) {
            mp_player->handleKeyPressEvent(e);
        } else if (e->key() == Qt::Key_Space) {
            mp_player->handleKeyPressEvent(e);
        } else if (e->key() == Qt::Key_Q) {
            mp_player->handleKeyPressEvent(e);
        } else if (e->key() == Qt::Key_E) {
            mp_player->handleKeyPressEvent(e);
        } else if (e->key() == Qt::Key_F) {
            mp_player->handleKeyPressEvent(e);
        } else if(e->key() == Qt::Key_1) {
            mp_inventory->m_selectedListIdx = 1;
        } else if(e->key() == Qt::Key_2) {
            mp_inventory->m_selectedListIdx = 2;
        } else if(e->key() == Qt::Key_3) {
            mp_inventory->m_selectedListIdx = 3;
        } else if(e->key() == Qt::Key_4) {
            mp_inventory->m_selectedListIdx = 4;
        } else if(e->key() == Qt::Key_5) {
            mp_inventory->m_selectedListIdx = 5;
        } else if(e->key() == Qt::Key_6) {
            mp_inventory->m_selectedListIdx = 6;
        } else if(e->key() == Qt::Key_7) {
            mp_inventory->m_selectedListIdx = 7;
        } else if(e->key() == Qt::Key_8) {
            mp_inventory->m_selectedListIdx = 8;
        } else if(e->key() == Qt::Key_9) {
            mp_inventory->m_selectedListIdx = 9;
        } else if(e->key() == Qt::Key_I) {
            m_showInventory = !m_showInventory;
        } else if (e->key() == Qt::Key_Z) {
            mp_player->handleKeyPressEvent(e);
        } else if (e->key() == Qt::Key_C && mp_survivalMode && mp_player->m_HP < 10 && mp_player->m_dirtInventory > 9) {
            mp_player->handleKeyPressEvent(e);
            updateHP(mp_player->m_HP + 1);
            mp_HPBar->destroy();
            mp_HPBar->create();
        } else if (e->key() == Qt::Key_V) {
            mp_survivalMode = !mp_survivalMode;
        }
    }
    if (mp_playerDead && e->key() == Qt::Key_X)
    {
        resetGame();
    }

}

void MyGL::keyReleaseEvent(QKeyEvent *e)
{
    if(e->isAutoRepeat()) {
        return;
    }

    if (e->key() == Qt::Key_W) {
        mp_player->handleKeyReleaseEvent(e);
    } else if (e->key() == Qt::Key_S) {
        mp_player->handleKeyReleaseEvent(e);
    } else if (e->key() == Qt::Key_D) {
        mp_player->handleKeyReleaseEvent(e);
    } else if (e->key() == Qt::Key_A) {
        mp_player->handleKeyReleaseEvent(e);
    } else if(e->key() == Qt::Key_Space) {
        mp_player->handleKeyReleaseEvent(e);
    } else if(e->key() == Qt::Key_F) {
        mp_player->handleKeyReleaseEvent(e);
    } else if(e->key() == Qt::Key_Q) {
        mp_player->handleKeyReleaseEvent(e);
    } else if(e->key() == Qt::Key_E) {
        mp_player->handleKeyReleaseEvent(e);
    }

}

void MyGL::mousePressEvent(QMouseEvent *e) {
    if (e->button() == Qt::LeftButton) {
        BlockType removedType = removeBlock();

        // add removed block to inventory
        if(removedType != EMPTY) {
            bool itemExist = false;
            for(Item* item : mp_inventory->m_inventory) {
                if(item->m_blockType == removedType) {
                    item->m_num++;
                    itemExist = true;
                    break;
                }
            }
            if(!itemExist) {
                Item* newItem = new Item(this, removedType);
                newItem->m_num = 1;
                mp_inventory->m_inventory.push_back(newItem);
            }

            updateRenderFrameList();

        }


    } else if (e->button() == Qt::RightButton) {
        if(mp_inventory->m_selectedListIdx < 0 ||
           m_renderFrameList[mp_inventory->m_selectedListIdx - 1]->m_blockType == EMPTY) {
            return;
        }
        addBlock(m_renderFrameList[mp_inventory->m_selectedListIdx - 1]->m_blockType);
        mp_inventory->m_inventory[mp_inventory->m_selectedListIdx - 1]->m_num--;
        if(mp_inventory->m_inventory[mp_inventory->m_selectedListIdx - 1]->m_num < 1) {
            mp_inventory->m_inventory.erase(mp_inventory->m_inventory.begin() + mp_inventory->m_selectedListIdx - 2);
        }

        updateRenderFrameList();
    }

}

void MyGL::updateRenderFrameList() {
    // refresh the frameRenderList
    for(int i = 0; i < mp_inventory->m_inventory.size(); ++i) {
        if(i >= mp_inventory->getListSize()) {
            break;
        }
        m_renderFrameList[i]->m_blockType = mp_inventory->m_inventory[i]->m_blockType;
    }
    if(mp_inventory->m_inventory.size() < mp_inventory->getListSize()) {
        for(int i = mp_inventory->m_inventory.size(); i < mp_inventory->getListSize(); ++i) {
            m_renderFrameList[i]->m_blockType = EMPTY;
        }
    }

    // recreate renderFrameList
    for(int i = 0; i < 9; ++i){
        m_renderFrameList[i]->create();
    }

}

void MyGL::mouseReleaseEvent(QMouseEvent *e) {
    mp_player->handleMouseReleaseEvent(e);
}

void MyGL::mouseMoveEvent(QMouseEvent *e) {
    mp_player->handleMouseMoveEvent(e);
    MoveMouseToCenter();
}

void MyGL::terrainExtand()
{
    float detectDistance = 24.f;
    glm::vec3 forwardDetectPoint = mp_player->m_position + detectDistance * mp_player->mp_camera->look;
    glm::vec3 backwardDetectPoint = mp_player->m_position - detectDistance * mp_player->mp_camera->look;
    glm::vec3 rightDetectPoint = mp_player->m_position + detectDistance * mp_player->mp_camera->right;
    glm::vec3 leftDetectPoint = mp_player->m_position - detectDistance * mp_player->mp_camera->right;

    glm::ivec2 chunkCoord4Detect = mp_terrain->convertBlockCoord2ChunkCoord(floor(forwardDetectPoint[0]), floor(forwardDetectPoint[2]));
    if(!mp_terrain->chunkExists(chunkCoord4Detect)){
        bool existFlag = false;
        for(glm::ivec2 existingCoord : processingChunkCoordList) {
            if(chunkCoord4Detect == existingCoord) {
                existFlag = true;
                break;
            }
        }
        if(!existFlag) {
            processingChunkCoordList.push_back(chunkCoord4Detect);
            AThread* newThread = new AThread(mp_mutex, expandChunkList, mp_terrain, chunkCoord4Detect);
            QThreadPool::globalInstance()->start(newThread);
        }
    }

    chunkCoord4Detect = mp_terrain->convertBlockCoord2ChunkCoord(floor(backwardDetectPoint[0]), floor(backwardDetectPoint[2]));
    if(!mp_terrain->chunkExists(chunkCoord4Detect)){
        bool existFlag = false;
        for(glm::ivec2 existingCoord : processingChunkCoordList) {
            if(chunkCoord4Detect == existingCoord) {
                existFlag = true;
                break;
            }
        }
        if(!existFlag) {
            processingChunkCoordList.push_back(chunkCoord4Detect);
            AThread* newThread = new AThread(mp_mutex, expandChunkList, mp_terrain, chunkCoord4Detect);
            QThreadPool::globalInstance()->start(newThread);
        }
    }

    chunkCoord4Detect = mp_terrain->convertBlockCoord2ChunkCoord(floor(rightDetectPoint[0]), floor(rightDetectPoint[2]));
    if(!mp_terrain->chunkExists(chunkCoord4Detect)){
        bool existFlag = false;
        for(glm::ivec2 existingCoord : processingChunkCoordList) {
            if(chunkCoord4Detect == existingCoord) {
                existFlag = true;
                break;
            }
        }
        if(!existFlag) {
            processingChunkCoordList.push_back(chunkCoord4Detect);
            AThread* newThread = new AThread(mp_mutex, expandChunkList, mp_terrain, chunkCoord4Detect);
            QThreadPool::globalInstance()->start(newThread);
        }
    }

    chunkCoord4Detect = mp_terrain->convertBlockCoord2ChunkCoord(floor(leftDetectPoint[0]), floor(leftDetectPoint[2]));
    if(!mp_terrain->chunkExists(chunkCoord4Detect)){
        bool existFlag = false;
        for(glm::ivec2 existingCoord : processingChunkCoordList) {
            if(chunkCoord4Detect == existingCoord) {
                existFlag = true;
                break;
            }
        }
        if(!existFlag) {
            processingChunkCoordList.push_back(chunkCoord4Detect);
            AThread* newThread = new AThread(mp_mutex, expandChunkList, mp_terrain, chunkCoord4Detect);
            QThreadPool::globalInstance()->start(newThread);
        }
    }
}

void MyGL::addBlock(BlockType blockType)
{
    BlockType targetType = blockType;
    float armLength = 2.f;
    glm::ivec3 targetPos = glm::ivec3(mp_player->m_position + armLength * mp_player->mp_camera->look) + glm::ivec3(0, 1, 0);
    if(mp_terrain->getBlockTypeAt(targetPos[0], targetPos[1], targetPos[2]) == BlockType::EMPTY){
        mp_terrain->setBlockTypeAt(targetPos[0], targetPos[1], targetPos[2], targetType);
        Chunk* chunk = mp_terrain->getChunk(mp_terrain->convertBlockCoord2ChunkCoord(targetPos[0], targetPos[2]));
        chunk->destroy();
        chunk->create();

        update();

        return;
    }
    armLength = 1.f;
    targetPos = glm::ivec3(mp_player->m_position + armLength * mp_player->mp_camera->look) + glm::ivec3(0, 1, 0);
    if(mp_terrain->getBlockTypeAt(targetPos[0], targetPos[1], targetPos[2]) == BlockType::EMPTY){
        mp_terrain->setBlockTypeAt(targetPos[0], targetPos[1], targetPos[2], targetType);
        Chunk* chunk = mp_terrain->getChunk(mp_terrain->convertBlockCoord2ChunkCoord(targetPos[0], targetPos[2]));
        chunk->destroy();
        chunk->create();

        update();

        return;
    }
}

BlockType MyGL::removeBlock()
{
    BlockType targetType = BlockType::EMPTY;
    float armLength = 1.f;
    glm::ivec3 targetPos = glm::ivec3(mp_player->m_position + armLength * mp_player->mp_camera->look) + glm::ivec3(0, 1, 0);

    if(mp_terrain->getBlockTypeAt(targetPos[0], targetPos[1], targetPos[2]) != BlockType::EMPTY){
        //============================add by yuru=======================================================
        BlockType removeBlockType = mp_terrain->getBlockTypeAt(targetPos[0], targetPos[1], targetPos[2]);

        mp_terrain->setBlockTypeAt(targetPos[0], targetPos[1], targetPos[2], targetType);
        Chunk* chunk = mp_terrain->getChunk(mp_terrain->convertBlockCoord2ChunkCoord(targetPos[0], targetPos[2]));
        chunk->destroy();
        chunk->create();

        update();

        mp_player->m_dirtInventory += 1;
        return removeBlockType;
    }
    armLength = 2.f;
    targetPos = glm::ivec3(mp_player->m_position + armLength * mp_player->mp_camera->look) + glm::ivec3(0, 1, 0);
    if(mp_terrain->getBlockTypeAt(targetPos[0], targetPos[1], targetPos[2]) != BlockType::EMPTY){
        //============================add by yuru=======================================================
        BlockType removeBlockType = mp_terrain->getBlockTypeAt(targetPos[0], targetPos[1], targetPos[2]);

        mp_terrain->setBlockTypeAt(targetPos[0], targetPos[1], targetPos[2], targetType);
        Chunk* chunk = mp_terrain->getChunk(mp_terrain->convertBlockCoord2ChunkCoord(targetPos[0], targetPos[2]));
        chunk->destroy();
        chunk->create();

        update();

        return removeBlockType;
    }

    return EMPTY;
}

void MyGL::updateHP(int HP)
{
    mp_player->m_HP = HP;
    mp_HPBar->HPValue = HP;
}

void MyGL::resetSurvival()
{

}

void MyGL::resetGame()
{
    // Reset survival mode flags
    mp_survivalMode = false;
    mp_playerDead = false;
    updateHP(10);

    // Reset destroy mode flags
    mp_player->m_ZHold = false;
    mp_player->m_destroy = false;

    // Reset player's position and inventory
    mp_player->m_position = glm::vec3(5, 245, 5);
    glm::vec3 eyePos = glm::vec3(mp_player->m_position.x + 0.5, mp_player->m_position.y + 1, mp_player->m_position.z + 0.5);
    mp_player->mp_camera->eye = eyePos;

}

void MyGL::createRenderBuffers()
{
    // Initialize the frame buffers and render textures
    glGenFramebuffers(1, &m_frameBuffer);
    glGenTextures(1, &m_renderedTexture);
    glGenRenderbuffers(1, &m_depthRenderBuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
    // Bind our texture so that all functions that deal with textures will interact with this one
    glBindTexture(GL_TEXTURE_2D, m_renderedTexture);
    // Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->devicePixelRatio() * this->width(), this->devicePixelRatio() * this->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)0);

    // Set the render settings for the texture we've just created.
    // Essentially zero filtering on the "texture" so it appears exactly as rendered
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // Clamp the colors at the edge of our texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Initialize our depth buffer
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, this->devicePixelRatio() * this->width(), this->devicePixelRatio() * this->height());
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthRenderBuffer);

    // Set m_renderedTexture as the color output of our frame buffer
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_renderedTexture, 0);

    // Sets the color output of the fragment shader to be stored in GL_COLOR_ATTACHMENT0, which we previously set to m_renderedTextures[i]
    GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBuffers); // "1" is the size of drawBuffers

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "Frame buffer did not initialize correctly..." << std::endl;
        printGLErrorLog();
    }
}

void MyGL::performPostprocessRenderPass()
{
    // Render the frame buffer as a texture on a screen-size quad

    // Tell OpenGL to render to the viewport's frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, this->defaultFramebufferObject());
    // Render on the whole framebuffer, complete from the lower left corner to the upper right
    glViewport(0,0,this->devicePixelRatio() * this->width(),this->devicePixelRatio() * this->height());
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_renderedTexture);

//    mp_progPostprocessCurrent->draw(m_geomQuad, 0);
    mp_progFullScreen->draw(*mp_postprocessedImage);
}

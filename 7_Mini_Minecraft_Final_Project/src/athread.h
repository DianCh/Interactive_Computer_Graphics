#ifndef ATHREAD_H
#define ATHREAD_H

#include <QRunnable>
#include <QMutex>
#include <scene/terrain.h>

class AThread : public QRunnable
{
public:
    AThread(QMutex* mutex, std::vector<Chunk*>& chunkList, Terrain* mp_terrain, glm::ivec2 newChunkCoord);
    void run() override;

    QMutex* mp_mutex;
    std::vector<Chunk*>& chunkList;
    Terrain* mp_terrain;
    glm::ivec2 newChunkCoord;
};

#endif // ATHREAD_H

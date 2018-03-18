#include "athread.h"

AThread::AThread(QMutex* mutex, std::vector<Chunk*>& chunkList, Terrain* mp_terrain, glm::ivec2 newChunkCoord): mp_mutex(mutex),
                                                                                                                chunkList(chunkList),
                                                                                                                mp_terrain(mp_terrain),
                                                                                                                newChunkCoord(newChunkCoord)
{

}

void AThread::run() {
    mp_mutex->lock();

    Chunk* newChunk = mp_terrain->generateNewChunk(newChunkCoord);
    chunkList.push_back(newChunk);

    mp_mutex->unlock();
}


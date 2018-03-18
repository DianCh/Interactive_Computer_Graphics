#pragma once
#include <QList>
#include <QSET>
#include <la.h>
#include "drawable.h"
#include <array>
#include "noise.h"
#include "lsystem.h"
#include <iostream>
#include "texture.h"

// C++ 11 allows us to define the size of an enum. This lets us use only one byte
// of memory to store our different block types. By default, the size of a C++ enum
// is that of an int (so, usually four bytes). This *does* limit us to only 256 different
// block types, but in the scope of this project we'll never get anywhere near that many.
enum BlockType : unsigned char
{
    EMPTY, GRASS, DIRT, STONE, WATER, LAVA, CLOUD, COAL, IRON, SAND, SNOW
};


class BlockWrapper
{
public:
    BlockType bt;

    BlockWrapper();
    BlockWrapper(BlockType b);
};

inline uint64_t qHash(const BlockWrapper &bw);
bool operator ==(const BlockWrapper &bw1, const BlockWrapper &bw2);

class Chunk;

class ivecXZ
{
public:
    glm::ivec2 xz;

    ivecXZ();
    ivecXZ(int x, int z);
    ivecXZ(glm::ivec2 v);
};

inline uint64_t qHash(const ivecXZ &v);
bool operator ==(const ivecXZ &v1, const ivecXZ v2);

class Terrain
{
private:

    Texture *texture;       // New features in milestone 2

public:
    /// vvvvvvvvv Modified or Newly Added vvvvvvvvvvvvvv
    OpenGLContext* context;
    glm::ivec3 dimensions;
    QHash<ivecXZ, Chunk*> chunks;        // modified to use QHash
    FBM terrainGenerator;
    QHash<ivecXZ, int> terrianTops;//map recording the top height of certain xz points
    QSet<ivecXZ> river;//set recording the xz points constructing river

    Terrain(OpenGLContext* context);

    glm::ivec2 convertBlockCoord2ChunkCoord(int blockGlobalX, int blockGlobalZ) const;
    bool chunkExists(glm::ivec2 chunkGlobalXZ) const;
    Chunk* getChunk(glm::ivec2 chunkGlobalXZ) const;

    BlockType getBlockTypeAt(int globalX, int globalY, int globalZ) const;
    void setBlockTypeAt(int& globalX, int& globalY, int& globalZ, BlockType &bt);

    void generateRiver(std::vector<std::pair<glm::ivec2, glm::ivec2>> riverSegments);
    void initializeTerrian();
    Chunk* generateNewChunk(glm::ivec2 globalXZ); // Given block types and global coordinates of the lower-left-back corner, generate a chunk

    void initializeTestTerrain();
    void generateNewTestChunk(glm::ivec2 globalXZ);

    void createChunks();                // Iterate over all chunks and set up their vbo data
    /// ^^^^^^^^^ Modified or Newly Added ^^^^^^^^^^^^^^

    void initializeTexture();       // New features in milestone 2
    void bindTexture() const;       // New features in milestone 2
    void loadTexture() const;       // New features in milestone 2
};

class ChunkLiquids : public Drawable
{
private:
    std::vector<GLuint> idx;
    std::vector<glm::vec4> pnt;
    std::vector<glm::ivec2> flags;

public:
    ChunkLiquids(OpenGLContext* context);
    ~ChunkLiquids();

    void setIdx(std::vector<GLuint> &idxLiquids);
    void setPNT(std::vector<glm::vec4> &pntLiquids);
    void setFlags(std::vector<glm::ivec2> &flagsLiquids);

    void create() override;
    GLenum drawMode() override;
};

class Chunk : public Drawable
{
private:
    std::array<BlockType,65536> blkTypes;
    glm::ivec2 globalCords;
    Chunk *east;            // Pointer to the adjacent chunk having coordinates (x+16, 1, z)
    Chunk *west;            // Pointer to the adjacent chunk having coordinates (x-16, 1, z)
    Chunk *south;           // Pointer to the adjacent chunk having coordiantes (x, 1, z+16)
    Chunk *north;           // Pointer to the adjacent chunk having coordinates (x, 1, z-16)

    // This is added in milestone 2 to achieve render the liquid surfaces last
    ChunkLiquids *liquids;

    static glm::vec4 colorDIRT;
    static glm::vec4 colorSTONE;
    static glm::vec4 colorGRASS;

    static QHash<BlockWrapper, std::vector<glm::vec4>> uve;

public:
    friend class ItemFrame;  //===========add by Yuru============================
    friend class ItemCube;   //===========add by Yuru===============================
    Chunk(OpenGLContext* context);
    ~Chunk();

    // A bunch of getters
    BlockType getBlockTypeAt(int localX, int localY, int localZ) const;
    BlockType& getBlockTypeAt(int localX, int localY, int localZ);
    glm::ivec2 getGlobalCords() const;
    Chunk* getEast() const;
    Chunk* getWest() const;
    Chunk* getSouth() const;
    Chunk* getNorth() const;

    // This is added in milestone 2 to achieve render the liquid surfaces last
    ChunkLiquids* getLiquids() const;

    // A bunch of setters
    void setBlockTypeAt(int localX, int localY, int localZ, BlockType &bt);
    void setBlocks(std::array<BlockType,65536> newTypes);
    void setGlobalCords(glm::ivec2 &xz);
    void setEast(Chunk *e);
    void setWest(Chunk *w);
    void setSouth(Chunk *s);
    void setNorth(Chunk *n);

    void create() override;     // Set up the vbo data for the entire chunk
    void createOneCube(glm::vec4 globalPos, std::vector<bool> &neighborsEmptyOrLiquid, BlockType thisType, int &countVtx,
                       std::vector<GLuint> &idx, std::vector<glm::vec4> &pnt, std::vector<glm::ivec2> &flags);
                                // Set up the vbo data for one brick; utility function called iteratively by create()
    void createOneLiquidCube(glm::vec4 globalPos, std::vector<bool> neighborsEmpty, BlockType thisType, int &countVtx,
                             std::vector<GLuint> &idx, std::vector<glm::vec4> &pnt, std::vector<glm::ivec2> &flags);

    GLenum drawMode() override;

    static void initializeUVData(); // New features in milestone 2
};

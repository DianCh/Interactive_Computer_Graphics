#include <scene/terrain.h>

/// ===== Needed for customized Hash ===================
inline uint64_t qHash(const ivecXZ &v)
{
    uint64_t x = (uint64_t)v.xz[0];
    uint64_t z = (uint64_t)v.xz[1];

    return (x << 32) + z;
}

bool operator ==(const ivecXZ &v1, const ivecXZ v2)
{
    return (v1.xz[0] == v2.xz[0]) && (v1.xz[1] == v2.xz[1]);
}

ivecXZ::ivecXZ() : xz(glm::ivec2(0, 0)) {}

ivecXZ::ivecXZ(int x, int z) : xz(glm::ivec2(x, z)) {}

ivecXZ::ivecXZ(glm::ivec2 v) : xz(v) {}

inline uint64_t qHash(const BlockType &blk)
{
    return (unsigned char)blk;
}

bool operator ==(const BlockType &blk1, BlockType &blk2)
{
    return ((unsigned char)blk1) == ((unsigned char)blk2);
}

inline uint64_t qHash(const BlockWrapper &bw)
{
    return (unsigned char)bw.bt;
}

bool operator ==(const BlockWrapper &bw1, const BlockWrapper &bw2)
{
    return ((unsigned char)bw1.bt) == ((unsigned char)bw2.bt);
}

BlockWrapper::BlockWrapper() : bt(BlockType::EMPTY) {}

BlockWrapper::BlockWrapper(BlockType b) : bt(b) {}

/// ===== Needed for customized Hash ===================


/// =============== Terrain Class ======================
Terrain::Terrain(OpenGLContext* context) : context(context), dimensions(64, 256, 64), chunks(), terrainGenerator(), terrianTops(), river()
{}

glm::ivec2 Terrain::convertBlockCoord2ChunkCoord(int blockGlobalX, int blockGlobalZ) const
{
    int chunkGlobalX = blockGlobalX >= 0 ? blockGlobalX / 16 * 16 : ((blockGlobalX + 1) / 16 - 1) * 16;
    int chunkGlobalZ = blockGlobalZ >= 0 ? blockGlobalZ / 16 * 16 : ((blockGlobalZ + 1) / 16 - 1) * 16;

    return glm::ivec2(chunkGlobalX, chunkGlobalZ);
}

bool Terrain::chunkExists(glm::ivec2 chunkGlobalXZ) const
{
    return chunks.contains(ivecXZ(chunkGlobalXZ));
}

Chunk* Terrain::getChunk(glm::ivec2 chunkGlobalXZ) const
{
    if(!chunkExists(chunkGlobalXZ)) return nullptr;
    return chunks[ivecXZ(chunkGlobalXZ)];
}

BlockType Terrain::getBlockTypeAt(int globalX, int globalY, int globalZ) const
{
    glm::ivec2 chunkGlobalXZ = convertBlockCoord2ChunkCoord(globalX, globalZ);
    Chunk* chunk = chunks[ivecXZ(chunkGlobalXZ)];

    int localX = globalX - chunkGlobalXZ[0];
    int localY = globalY;
    int localZ = globalZ - chunkGlobalXZ[1];

    return chunk->getBlockTypeAt(localX, localY, localZ);
}

void Terrain::setBlockTypeAt(int& globalX, int& globalY, int& globalZ, BlockType &bt)
{
    glm::ivec2 chunkGlobalXZ = convertBlockCoord2ChunkCoord(globalX, globalZ);
    Chunk* chunk = chunks[ivecXZ(chunkGlobalXZ)];

    int localX = globalX - chunkGlobalXZ[0];
    int localY = globalY;
    int localZ = globalZ - chunkGlobalXZ[1];

    chunk->setBlockTypeAt(localX, localY, localZ, bt);
}

void Terrain::generateRiver(std::vector<std::pair<glm::ivec2, glm::ivec2>> riverSegments)
{
    for(auto segment : riverSegments){
        glm::vec2 start, end;
        if(segment.first[0] > segment.second[0]){
            start = segment.second;
            end = segment.first;
        }else{
            start = segment.first;
            end = segment.second;
        }
        if(start[1] < end[1]){
            int i = start[0], j = start[1];
            river.insert(ivecXZ(i, j));
            while(i < end[0] || j < end[1]){
                if(i < end[0]) i++;
                if(j < end[1]) j++;
                river.insert(ivecXZ(i, j));
            }
        }else{
            int i = start[0], j = start[1];
            river.insert(ivecXZ(i, j));
            while(i < end[0] || j > end[1]){
                if(i < end[0]) i++;
                if(j > end[1]) j--;
                river.insert(ivecXZ(i, j));
            }
        }
    }
}

void Terrain::initializeTerrian()
{
    generateRiver(LSystem::generateLinearRiver());
    generateRiver(LSystem::generateDeltaRiver());
    chunks[ivecXZ(glm::ivec2(0, 0))] = generateNewChunk(glm::ivec2(0, 0));
    chunks[ivecXZ(glm::ivec2(-16, 0))] = generateNewChunk(glm::ivec2(-16, 0));
    chunks[ivecXZ(glm::ivec2(0, -16))] = generateNewChunk(glm::ivec2(0, -16));
    chunks[ivecXZ(glm::ivec2(-16, -16))] = generateNewChunk(glm::ivec2(-16, -16));
}

void Terrain::initializeTestTerrain()
{
    generateNewTestChunk(glm::ivec2(0, 0));
}

Chunk* Terrain::generateNewChunk(glm::ivec2 globalXZ)
{
    // Create a new chunk on the heap
    Chunk *chunk = new Chunk(context);

    //using noise to generate top heights
    int top[16][16] = {0};
    for(int x = 0; x < 16; ++x){
        for(int z = 0; z < 16; ++z){
                int noise = 1 + terrainGenerator.fbm(x + globalXZ[0], z + globalXZ[1]);
                if(noise > 0.8 && noise <= 1.0){
                    int r = std::floor((noise - 0.5) * 32);
                    for(int i = x - r; i <= x + r; ++i){
                        for(int j = z - r; j <= z + r; ++j){
                            ivecXZ globalCoord = ivecXZ(i + globalXZ[0], j + globalXZ[1]);

                            int d = std::max(abs(i - x), abs(j - z));
                            int height = 128 + 32 + r + 1 - d;
                            if(terrianTops.count(globalCoord) == 0
                                    || (terrianTops.count(globalCoord) == 1 && terrianTops[globalCoord] < height)) terrianTops[globalCoord] = height;
                            if(i >= 0 && j >= 0 && i <= 15 && j <= 15) top[i][j] = terrianTops[globalCoord];
                        }
                    }
                }else{
                    ivecXZ globalCoord = ivecXZ(x + globalXZ[0], z + globalXZ[1]);
                    if(terrianTops.count(globalCoord) == 0) top[x][z] = 128 + 32;
                    else top[x][z] = terrianTops[globalXZ];
                }
        }
    }

    //fill block types using tops
    std::array<BlockType,65536> blkTypes;
    for(int x = 0; x < 16; ++x){
        for(int z = 0; z < 16; ++z){
            glm::ivec2 globalCoord = glm::ivec2(x + globalXZ[0], z + globalXZ[1]);
            float noise = terrainGenerator.fbm(x + globalXZ[0], z + globalXZ[1]);
            for(int y = 0; y < 256; ++y){
                int i = 16 * z + 256 * y + x;
                if(y > 245) blkTypes[i] = BlockType::EMPTY;
                else if(y == 245){
                    if(noise > 0 && noise <0.3) blkTypes[i] = BlockType::EMPTY;
                    else blkTypes[i] = BlockType::EMPTY;
                }else if(y > top[x][z] && y < 245) blkTypes[i] = BlockType::EMPTY;
                else if(y > top[x][z] - 8 && y <= top[x][z]) {// depth of river is always 8
                    if(river.contains(ivecXZ(globalCoord))
                            || river.contains(ivecXZ(globalCoord + glm::ivec2(1, 1)))//expand river's width from 1 to 3
                            || river.contains(ivecXZ(globalCoord + glm::ivec2(1, 0)))
                            || river.contains(ivecXZ(globalCoord + glm::ivec2(1, -1)))
                            || river.contains(ivecXZ(globalCoord + glm::ivec2(0, 1)))
                            || river.contains(ivecXZ(globalCoord + glm::ivec2(0, -1)))
                            || river.contains(ivecXZ(globalCoord + glm::ivec2(-1, 1)))
                            || river.contains(ivecXZ(globalCoord + glm::ivec2(-1, 0)))
                            || river.contains(ivecXZ(globalCoord + glm::ivec2(-1, -1)))
                            ) blkTypes[i] = BlockType::WATER;
                    else{
                        if(y == top[x][z]) blkTypes[i] = BlockType::GRASS;
                        else blkTypes[i] = BlockType::DIRT;
                    }
                }
                else if(y >= 128 && y <= top[x][z] - 8) blkTypes[i] = BlockType::DIRT;
                else if(y < 128 && y > 44) blkTypes[i] = BlockType::STONE;
                else if(y <= 44 && y > 40){//generate channel in xz plane
                    if((x == 0 || x == 1 || x == 7 || x == 8 || x == 14 || x == 15
                        || z == 0 || z == 1 || z == 7 || z == 8 || z == 14 || z == 15) && (noise < 1 && noise > -0.5)){
                        if(noise > 0.5 && noise < 0.6) blkTypes[i] = BlockType::EMPTY;
                        else if(noise > 0.6 && noise < 0.7) blkTypes[i] = BlockType::EMPTY;
                        else blkTypes[i] = BlockType::EMPTY;
                    }
                    else blkTypes[i] = BlockType::STONE;
                }else if(y <= 40 && y > 30) blkTypes[i] = BlockType::STONE;
                else if(y <= 30 && y > 20) blkTypes[i] = BlockType::EMPTY;
                else if(y <= 20 && y > 15) blkTypes[i] = BlockType::LAVA;
                else if(y <= 15) blkTypes[i] = BlockType::STONE;
            }
        }
    }

    //generate cloud
    for(int x = 0; x < 15; x++){
        for(int z = 3; z < 16; z++){
            float noise = ((double) rand() / (RAND_MAX));//terrainGenerator.fbm(x + globalXZ[0], z + globalXZ[1]);
            if((noise > 0.5 && noise < 0.6)){
                blkTypes[16 * z + 256 * 245 + x] = BlockType::CLOUD;
                blkTypes[16 * (z - 1) + 256 * 245 + x] = BlockType::CLOUD;
//                blkTypes[16 * (z - 2) + 256 * 245 + x] = BlockType::CLOUD;
                blkTypes[16 * (z - 1) + 256 * 245 + (x + 1)] = BlockType::CLOUD;
                blkTypes[16 * (z - 2) + 256 * 245 + (x + 1)] = BlockType::CLOUD;
//                blkTypes[16 * (z - 3) + 256 * 245 + (x + 1)] = BlockType::CLOUD;
            }
        }
    }


    //generate tree
    for(int x = 3; x <= 14; x++){
        for(int z = 3; z <= 14; z++){
            float noise = ((double) rand() / (RAND_MAX));
            if((noise > 0.5 && noise < 0.505)){
                blkTypes[16 * z + 256 * (top[x][z] + 1) + x] = BlockType::DIRT;
                blkTypes[16 * z + 256 * (top[x][z] + 2) + x] = BlockType::DIRT;
                blkTypes[16 * z + 256 * (top[x][z] + 3) + x] = BlockType::DIRT;
                for(int x0 = x - 2; x0 <= x + 2; x0++){
                    for(int z0 = z - 2; z0 <= z + 2; z0++){
                        blkTypes[16 * z0 + 256 * (top[x][z] + 4) + x0] = BlockType::GRASS;
                    }
                }
                for(int x0 = x - 1; x0 <= x + 1; x0++){
                    for(int z0 = z - 1; z0 <= z + 1; z0++){
                        blkTypes[16 * z0 + 256 * (top[x][z] + 5) + x0] = BlockType::GRASS;
                    }
                }
                blkTypes[16 * z + 256 * (top[x][z] + 6) + x] = BlockType::GRASS;
            }
        }
    }

    //generate hole in y-axis
    for(int x = 2; x <= 6; ++x){
        for(int z = 2; z <= 6; ++z){
            float noise = terrainGenerator.fbm(x + globalXZ[0], z + globalXZ[1]);
            if(noise > 0.3 && noise < 0.5){
                int y = top[x][z];
                if(blkTypes[16 * z + 256 * y + x] == BlockType::WATER) y -= 8;
                while(y > 30){
                    blkTypes[16 * z + 256 * y + x] = BlockType::EMPTY;
                    blkTypes[16 * (z + 1) + 256 * y + x] = BlockType::EMPTY;
                    blkTypes[16 * z + 256 * y + (x + 1)] = BlockType::EMPTY;
                    blkTypes[16 * (z + 1) + 256 * y + (x + 1)] = BlockType::EMPTY;
                    y--;
                }
            }
        }
    }

    //generate cave
    for(int x = 0; x < 10; x++){
        for(int z = 0; z < 10; z++){
            float noise = terrainGenerator.fbm(x + globalXZ[0], z + globalXZ[1]);//float noise = ((double) rand() / (RAND_MAX));
            if(noise < 0.1 && noise > 0){
                for(int x0 = x; x0 < x + 4; x0++){
                    for(int z0 = z; z0 < z + 4; z0++){
                        blkTypes[16 * z0 + 256 * 40 + x0] = BlockType::EMPTY;
                        blkTypes[16 * z0 + 256 * 39 + x0] = BlockType::EMPTY;
                    }
                }
                for(int x0 = x; x0 < x + 3; x0++){
                    for(int z0 = z; z0 < z + 3; z0++){
                        blkTypes[16 * z0 + 256 * 38 + x0] = BlockType::LAVA;
                        blkTypes[16 * z0 + 256 * 37 + x0] = BlockType::LAVA;
                    }
                }
            }
        }
    }

//    std::cout<<blkTypes.size()<<std::endl;

    // Set the block types and global coordinates of the lower-left-back corner
    chunk->setBlocks(blkTypes);
    chunk->setGlobalCords(globalXZ);

    // Set up the neighborhood relationships between chunks
    ivecXZ eastNeighbor = ivecXZ(globalXZ + glm::ivec2(16, 0));
    ivecXZ southNeighbor = ivecXZ(globalXZ + glm::ivec2(0, 16));
    ivecXZ westNeighbor = ivecXZ(globalXZ + glm::ivec2(-16, 0));
    ivecXZ northNeighbor = ivecXZ(globalXZ + glm::ivec2(0, -16));

    Chunk *neighbor = nullptr;

    if (chunks.contains(eastNeighbor))
    {
        neighbor = chunks[eastNeighbor];
        chunk->setEast(neighbor);
        neighbor->setWest(chunk);
    }

    if (chunks.contains(southNeighbor))
    {
        neighbor = chunks[southNeighbor];
        chunk->setSouth(neighbor);
        neighbor->setNorth(chunk);
    }

    if (chunks.contains(westNeighbor))
    {
        neighbor = chunks[westNeighbor];
        chunk->setWest(neighbor);
        neighbor->setEast(chunk);
    }

    if (chunks.contains(northNeighbor))
    {
        neighbor = chunks[northNeighbor];
        chunk->setNorth(neighbor);
        neighbor->setSouth(chunk);
    }

    return chunk;
}

void Terrain::generateNewTestChunk(glm::ivec2 globalXZ)
{
    // Create a new chunk on the heap
    Chunk *chunk = new Chunk(context);

    // Store this chunk in the hash map
    chunks[ivecXZ(globalXZ)] = chunk;

    std::array<BlockType,65536> blkTypes;
    for(int x = 0; x < 16; ++x)
    {
        for(int z = 0; z < 16; ++z)
        {
            for(int y = 0; y < 256; ++y)
            {
                int i = 16 * z + 256 * y + x;

                if (y < 3)
                {
                    blkTypes[i] = BlockType::STONE;
                }
                else if (y < 5)
                {
                    if (z < 12 && z > 4 && x < 8 && x > 4)
                    {
                        blkTypes[i] = BlockType::LAVA;
                    }
                    else if (z < 12 && z > 4 && x < 12 && x > 9)
                    {
                        blkTypes[i] = BlockType::WATER;
                    }
                    else
                    {
                        blkTypes[i] = BlockType::CLOUD;
                    }
                }
                else if (y < 7)
                {
                    if (z < 14 && z > 2 && x < 8 && x > 2)
                    {
                        blkTypes[i] = BlockType::LAVA;
                    }
                    else if (z < 14 && z > 2 && x < 14 && x > 9)
                    {
                        blkTypes[i] = BlockType::WATER;
                    }
                    else
                    {
                        blkTypes[i] = BlockType::CLOUD;
                    }
                }
                else
                {
                    blkTypes[i] = BlockType::EMPTY;
                }
            }
        }
    }

    // Set the block types and global coordinates of the lower-left-back corner
    chunk->setBlocks(blkTypes);
    chunk->setGlobalCords(globalXZ);

}

void Terrain::createChunks()
{
    QHash<ivecXZ, Chunk*>::iterator iter;

    for (iter = chunks.begin(); iter != chunks.end(); ++iter)
    {
        Chunk *thisChunk = iter.value();
        thisChunk->create();
    }
}

void Terrain::initializeTexture()
{
    texture = new Texture(context);
    texture->create(":/textures/minecraft_textures_all_dead.png");
}

void Terrain::loadTexture() const
{
    texture->load(0);
}

void Terrain::bindTexture() const
{
    texture->bind(0);
}

/// =============== Terrain Class ======================



/// ================= Chunk Class ======================

glm::vec4 Chunk::colorDIRT = glm::vec4(121.f, 85.f, 58.f, 255.f) / 255.f;
glm::vec4 Chunk::colorGRASS = glm::vec4(95.f, 159.f, 53.f, 255.f) / 255.f;
glm::vec4 Chunk::colorSTONE = glm::vec4(0.5f);

QHash<BlockWrapper, std::vector<glm::vec4>> Chunk::uve;

void Chunk::initializeUVData()
{
    // Set up UVs for GRASS block
    float eGrass = 7;
    std::vector<glm::vec4> uveGrass {glm::vec4(4.0 / 16, 1, eGrass, 0), glm::vec4(3.0 / 16, 1, eGrass, 0), glm::vec4(3.0 / 16, 15.0 / 16, eGrass, 0), glm::vec4(4.0 / 16, 15.0 / 16, eGrass, 0),
                                     glm::vec4(4.0 / 16, 1, eGrass, 0), glm::vec4(3.0 / 16, 1, eGrass, 0), glm::vec4(3.0 / 16, 15.0 / 16, eGrass, 0), glm::vec4(4.0 / 16, 15.0 / 16, eGrass, 0),
                                     glm::vec4(4.0 / 16, 1, eGrass, 0), glm::vec4(3.0 / 16, 1, eGrass, 0), glm::vec4(3.0 / 16, 15.0 / 16, eGrass, 0), glm::vec4(4.0 / 16, 15.0 / 16, eGrass, 0),
                                     glm::vec4(4.0 / 16, 1, eGrass, 0), glm::vec4(3.0 / 16, 1, eGrass, 0), glm::vec4(3.0 / 16, 15.0 / 16, eGrass, 0), glm::vec4(4.0 / 16, 15.0 / 16, eGrass, 0),
                                     glm::vec4(9.0 / 16, 14.0 / 16, eGrass, 0), glm::vec4(8.0 / 16, 14.0 / 16, eGrass, 0), glm::vec4(8.0 / 16, 13.0 / 16, eGrass, 0), glm::vec4(9.0 / 16, 13.0 / 16, eGrass, 0),
                                     glm::vec4(3.0 / 16, 1, eGrass, 0), glm::vec4(2.0 / 16, 1, eGrass, 0), glm::vec4(2.0 / 16, 15.0 / 16, eGrass, 0), glm::vec4(3.0 / 16, 15.0 / 16, eGrass, 0)};
    uve[BlockWrapper(BlockType::GRASS)] = uveGrass;

    // Set up UVs for STONE block
    float eStone = 3;
    std::vector<glm::vec4> uveStone {glm::vec4(2.0 / 16, 1, eStone, 0), glm::vec4(1.0 / 16, 1, eStone, 0), glm::vec4(1.0 / 16, 15.0 / 16, eStone, 0), glm::vec4(2.0 / 16, 15.0 / 16, eStone, 0),
                                     glm::vec4(2.0 / 16, 1, eStone, 0), glm::vec4(1.0 / 16, 1, eStone, 0), glm::vec4(1.0 / 16, 15.0 / 16, eStone, 0), glm::vec4(2.0 / 16, 15.0 / 16, eStone, 0),
                                     glm::vec4(2.0 / 16, 1, eStone, 0), glm::vec4(1.0 / 16, 1, eStone, 0), glm::vec4(1.0 / 16, 15.0 / 16, eStone, 0), glm::vec4(2.0 / 16, 15.0 / 16, eStone, 0),
                                     glm::vec4(2.0 / 16, 1, eStone, 0), glm::vec4(1.0 / 16, 1, eStone, 0), glm::vec4(1.0 / 16, 15.0 / 16, eStone, 0), glm::vec4(2.0 / 16, 15.0 / 16, eStone, 0),
                                     glm::vec4(2.0 / 16, 1, eStone, 0), glm::vec4(1.0 / 16, 1, eStone, 0), glm::vec4(1.0 / 16, 15.0 / 16, eStone, 0), glm::vec4(2.0 / 16, 15.0 / 16, eStone, 0),
                                     glm::vec4(2.0 / 16, 1, eStone, 0), glm::vec4(1.0 / 16, 1, eStone, 0), glm::vec4(1.0 / 16, 15.0 / 16, eStone, 0), glm::vec4(2.0 / 16, 15.0 / 16, eStone, 0)};
    uve[BlockWrapper(BlockType::STONE)] = uveStone;

    // Set up UVs for DIRT block
    float eDirt = 9;
    std::vector<glm::vec4> uveDirt {glm::vec4(3.0 / 16, 1, eDirt, 0), glm::vec4(2.0 / 16, 1, eDirt, 0), glm::vec4(2.0 / 16, 15.0 / 16, eDirt, 0), glm::vec4(3.0 / 16, 15.0 / 16, eDirt, 0),
                                    glm::vec4(3.0 / 16, 1, eDirt, 0), glm::vec4(2.0 / 16, 1, eDirt, 0), glm::vec4(2.0 / 16, 15.0 / 16, eDirt, 0), glm::vec4(3.0 / 16, 15.0 / 16, eDirt, 0),
                                    glm::vec4(3.0 / 16, 1, eDirt, 0), glm::vec4(2.0 / 16, 1, eDirt, 0), glm::vec4(2.0 / 16, 15.0 / 16, eDirt, 0), glm::vec4(3.0 / 16, 15.0 / 16, eDirt, 0),
                                    glm::vec4(3.0 / 16, 1, eDirt, 0), glm::vec4(2.0 / 16, 1, eDirt, 0), glm::vec4(2.0 / 16, 15.0 / 16, eDirt, 0), glm::vec4(3.0 / 16, 15.0 / 16, eDirt, 0),
                                    glm::vec4(3.0 / 16, 1, eDirt, 0), glm::vec4(2.0 / 16, 1, eDirt, 0), glm::vec4(2.0 / 16, 15.0 / 16, eDirt, 0), glm::vec4(3.0 / 16, 15.0 / 16, eDirt, 0),
                                    glm::vec4(3.0 / 16, 1, eDirt, 0), glm::vec4(2.0 / 16, 1, eDirt, 0), glm::vec4(2.0 / 16, 15.0 / 16, eDirt, 0), glm::vec4(3.0 / 16, 15.0 / 16, eDirt, 0)};
    uve[BlockWrapper(BlockType::DIRT)] = uveDirt;

    // Set up UVs for WATER block
    float eWater = 0.6;
    std::vector<glm::vec4> uveWater {glm::vec4(13.0 / 16, 4.0 / 16, eWater, 0), glm::vec4(13.0 / 16, 3.0 / 16, eWater, 0), glm::vec4(14.0 / 16, 3.0 / 16, eWater, 0), glm::vec4(14.0 / 16, 4.0 / 16, eWater, 0),
                                     glm::vec4(14.0 / 16, 4.0 / 16, eWater, 0), glm::vec4(13.0 / 16, 4.0 / 16, eWater, 0), glm::vec4(13.0 / 16, 3.0 / 16, eWater, 0), glm::vec4(14.0 / 16, 3.0 / 16, eWater, 0),
                                     glm::vec4(14.0 / 16, 3.0 / 16, eWater, 0), glm::vec4(14.0 / 16, 4.0 / 16, eWater, 0), glm::vec4(13.0 / 16, 4.0 / 16, eWater, 0), glm::vec4(13.0 / 16, 3.0 / 16, eWater, 0),
                                     glm::vec4(13.0 / 16, 3.0 / 16, eWater, 0), glm::vec4(14.0 / 16, 3.0 / 16, eWater, 0), glm::vec4(14.0 / 16, 4.0 / 16, eWater, 0), glm::vec4(13.0 / 16, 4.0 / 16, eWater, 0),
                                     glm::vec4(14.0 / 16, 4.0 / 16, eWater, 0), glm::vec4(13.0 / 16, 4.0 / 16, eWater, 0), glm::vec4(13.0 / 16, 3.0 / 16, eWater, 0), glm::vec4(14.0 / 16, 3.0 / 16, eWater, 0)};
    uve[BlockWrapper(BlockType::WATER)] = uveWater;

    float eLava = 0.6;
    std::vector<glm::vec4> uveLava {glm::vec4(15.0 / 16, 1.0 / 16, eLava, 0), glm::vec4(15.0 / 16, 0, eLava, 0), glm::vec4(1, 0, eLava, 0), glm::vec4(1, 1.0 / 16, eLava, 0),
                                    glm::vec4(1, 1.0 / 16, eLava, 0), glm::vec4(15.0 / 16, 1.0 / 16, eLava, 0), glm::vec4(15.0 / 16, 0, eLava, 0), glm::vec4(1, 0, eLava, 0),
                                    glm::vec4(1, 0, eLava, 0), glm::vec4(1, 1.0 / 16, eLava, 0), glm::vec4(15.0 / 16, 1.0 / 16, eLava, 0), glm::vec4(15.0 / 16, 0, eLava, 0),
                                    glm::vec4(15.0 / 16, 0, eLava, 0), glm::vec4(1, 0, eLava, 0), glm::vec4(1, 1.0 / 16, eLava, 0), glm::vec4(15.0 / 16, 1.0 / 16, eLava, 0),
                                    glm::vec4(1, 1.0 / 16, eLava, 0), glm::vec4(15.0 / 16, 1.0 / 16, eLava, 0), glm::vec4(15.0 / 16, 0, eLava, 0), glm::vec4(1, 0, eLava, 0)};
    uve[BlockWrapper(BlockType::LAVA)] = uveLava;

    float eCloud = 5;
    std::vector<glm::vec4> uveCloud = {glm::vec4(13.0 / 16, 5.0 / 16, eCloud, 0), glm::vec4(12.0 / 16, 5.0 / 16, eCloud, 0), glm::vec4(12.0 / 16, 4.0 / 16, eCloud, 0), glm::vec4(12.0 / 16, 5.0 / 16, eCloud, 0),
                                       glm::vec4(13.0 / 16, 5.0 / 16, eCloud, 0), glm::vec4(12.0 / 16, 5.0 / 16, eCloud, 0), glm::vec4(12.0 / 16, 4.0 / 16, eCloud, 0), glm::vec4(12.0 / 16, 5.0 / 16, eCloud, 0),
                                       glm::vec4(13.0 / 16, 5.0 / 16, eCloud, 0), glm::vec4(12.0 / 16, 5.0 / 16, eCloud, 0), glm::vec4(12.0 / 16, 4.0 / 16, eCloud, 0), glm::vec4(12.0 / 16, 5.0 / 16, eCloud, 0),
                                       glm::vec4(13.0 / 16, 5.0 / 16, eCloud, 0), glm::vec4(12.0 / 16, 5.0 / 16, eCloud, 0), glm::vec4(12.0 / 16, 4.0 / 16, eCloud, 0), glm::vec4(12.0 / 16, 5.0 / 16, eCloud, 0),
                                       glm::vec4(13.0 / 16, 5.0 / 16, eCloud, 0), glm::vec4(12.0 / 16, 5.0 / 16, eCloud, 0), glm::vec4(12.0 / 16, 4.0 / 16, eCloud, 0), glm::vec4(12.0 / 16, 5.0 / 16, eCloud, 0),
                                       glm::vec4(13.0 / 16, 5.0 / 16, eCloud, 0), glm::vec4(12.0 / 16, 5.0 / 16, eCloud, 0), glm::vec4(12.0 / 16, 4.0 / 16, eCloud, 0), glm::vec4(12.0 / 16, 5.0 / 16, eCloud, 0)};
    uve[BlockWrapper(BlockType::CLOUD)] = uveCloud;
}

Chunk::Chunk(OpenGLContext* context) : Drawable(context), globalCords(glm::ivec2(0, 0)),
    east(nullptr), west(nullptr), south(nullptr), north(nullptr), liquids(nullptr)
{
    ChunkLiquids *thisLiquids = new ChunkLiquids(context);
    liquids = thisLiquids;
}

Chunk::~Chunk()
{}

BlockType Chunk::getBlockTypeAt(int localX, int localY, int localZ) const
{
    return blkTypes[16 * localZ + 256 * localY + localX];
}

BlockType& Chunk::getBlockTypeAt(int localX, int localY, int localZ)
{
    return blkTypes[16 * localZ + 256 * localY + localX];
}

glm::ivec2 Chunk::getGlobalCords() const
{
    return globalCords;
}

Chunk* Chunk::getEast() const
{
    return east;
}

Chunk* Chunk::getWest() const
{
    return west;
}

Chunk* Chunk::getSouth() const
{
    return south;
}

Chunk* Chunk::getNorth() const
{
    return north;
}

ChunkLiquids* Chunk::getLiquids() const
{
    return liquids;
}

void Chunk::setBlockTypeAt(int localX, int localY, int localZ, BlockType &bt)
{
    blkTypes[16 * localZ + 256 * localY + localX] = bt;
}

void Chunk::setBlocks(std::array<BlockType, 65536> newTypes)
{
    blkTypes = newTypes;
}

void Chunk::setGlobalCords(glm::ivec2 &xz)
{
    globalCords = xz;
}

void Chunk::setEast(Chunk *e)
{
    east = e;
}

void Chunk::setWest(Chunk *w)
{
    west = w;
}

void Chunk::setSouth(Chunk *s)
{
    south = s;
}

void Chunk::setNorth(Chunk *n)
{
    north = n;
}

void Chunk::create()
{
    // Construct the containers for vbo data
    std::vector<GLuint> idx;
    std::vector<GLuint> idxLiquid;
    std::vector<glm::vec4> pnt;
    std::vector<glm::vec4> pntLiquid;
    std::vector<glm::ivec2> flags;
    std::vector<glm::ivec2> flagsLiquid;

    glm::vec4 offset = glm::vec4(globalCords[0], 0, globalCords[1], 0);

    int countVtx = 0;
    int countVtxWater = 0;

    // Iterate over all block in the chunk
    for (int x = 0; x < 16; ++x)
    {
        for (int y = 0; y < 256; ++y)
        {
            for (int z = 0; z < 16; ++z)
            {
                int i = 16 * z + 256 * y + x;

                // Don't bother to draw this block if it's empty
                if (blkTypes[i] == BlockType::EMPTY)
                {
                    continue;
                }

                // The following only happens if this is a non-empty block
                glm::vec4 globalPos = glm::vec4(x, y, z, 1) + offset;
                glm::vec4 color = glm::vec4(1, 0, 0, 1);

                if (blkTypes[i] == BlockType::DIRT)
                {
                    color = colorDIRT;
                }
                if (blkTypes[i] == BlockType::GRASS)
                {
                    color = colorGRASS;
                }
                if (blkTypes[i] == BlockType::STONE)
                {
                    color = colorSTONE;
                }

                // Set up flags for neighbors of this block
                std::vector<bool> neighborsEmptyOrLiquid {false, false, false, false, false, false};

                // Retrieve neighbors
                BlockType eastNeighbor = (x == 15) ? ((east == nullptr) ? BlockType::EMPTY : east->getBlockTypeAt(0, y, z))
                                                   : blkTypes[16 * z + 256 * y + x + 1];
                BlockType southNeighbor = (z == 15) ? ((south == nullptr) ? BlockType::EMPTY : south->getBlockTypeAt(x, y, 0))
                                                    : blkTypes[16 * z + 256 * y + x + 16];
                BlockType westNeighbor = (x == 0) ? ((west == nullptr) ? BlockType::EMPTY : west->getBlockTypeAt(15, y, z))
                                                  : blkTypes[16 * z + 256 * y + x - 1];
                BlockType northNeighbor = (z == 0) ? ((north == nullptr) ? BlockType::EMPTY : north->getBlockTypeAt(x, y, 15))
                                                   : blkTypes[16 * z + 256 * y + x - 16];
                BlockType aboveNeighbor = (y == 255) ? BlockType::EMPTY : blkTypes[16 * z + 256 * y + x + 256];
                BlockType belowNeighbor = (y == 0) ? BlockType::EMPTY : blkTypes[16 * z + 256 * y + x - 256];

                // Examine neighbors
                if (eastNeighbor == BlockType::EMPTY || eastNeighbor == BlockType::WATER || eastNeighbor == BlockType::LAVA)
                {
                    neighborsEmptyOrLiquid[0] = true;
                }
                if (southNeighbor == BlockType::EMPTY || southNeighbor == BlockType::WATER || southNeighbor == BlockType::LAVA)
                {
                    neighborsEmptyOrLiquid[1] = true;
                }
                if (westNeighbor == BlockType::EMPTY || westNeighbor == BlockType::WATER || westNeighbor == BlockType::LAVA)
                {
                    neighborsEmptyOrLiquid[2] = true;
                }
                if (northNeighbor == BlockType::EMPTY || northNeighbor == BlockType::WATER || northNeighbor == BlockType::LAVA)
                {
                    neighborsEmptyOrLiquid[3] = true;
                }
                if (aboveNeighbor == BlockType::EMPTY || aboveNeighbor == BlockType::WATER || aboveNeighbor == BlockType::LAVA)
                {
                    neighborsEmptyOrLiquid[4] = true;
                }
                if (belowNeighbor == BlockType::EMPTY || belowNeighbor == BlockType::WATER || belowNeighbor == BlockType::LAVA)
                {
                    neighborsEmptyOrLiquid[5] = true;
                }

                bool isLiquid = (blkTypes[i] == BlockType::WATER) || (blkTypes[i] == BlockType::LAVA);

                if (isLiquid)
                {
                    std::vector<bool> neighborsEmpty {false, false, false, false, false};

                    if (eastNeighbor == BlockType::EMPTY)
                    {
                        neighborsEmpty[0] = true;
                    }
                    if (southNeighbor == BlockType::EMPTY)
                    {
                        neighborsEmpty[1] = true;
                    }
                    if (westNeighbor == BlockType::EMPTY)
                    {
                        neighborsEmpty[2] = true;
                    }
                    if (northNeighbor == BlockType::EMPTY)
                    {
                        neighborsEmpty[3] = true;
                    }
                    if (aboveNeighbor == BlockType::EMPTY)
                    {
                        neighborsEmpty[4] = true;
                    }

                    createOneLiquidCube(globalPos, neighborsEmpty, blkTypes[i], countVtxWater, idxLiquid, pntLiquid, flagsLiquid);

                }
                if (!isLiquid)
                {
                    createOneCube(globalPos, neighborsEmptyOrLiquid, blkTypes[i], countVtx, idx, pnt, flags);
                }
            }
        }
    }

    // Use the data for liquid blocks to set up the VBO for liquids
    liquids->setIdx(idxLiquid);
    liquids->setPNT(pntLiquid);
    liquids->setFlags(flagsLiquid);

    liquids->create();


    // Pass data to VBO for solid blocks
    count = idx.size();

    generateIdx();
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx.size() * sizeof(GLuint), idx.data(), GL_STATIC_DRAW);

    generateAttr();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufAttr);
    context->glBufferData(GL_ARRAY_BUFFER, pnt.size() * sizeof(glm::vec4), pnt.data(), GL_STATIC_DRAW);

    generateFlag();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufFlag);
    context->glBufferData(GL_ARRAY_BUFFER, flags.size() * sizeof(glm::ivec2), flags.data(), GL_STATIC_DRAW);

}

void Chunk::createOneCube(glm::vec4 globalPos, std::vector<bool> &neighborsEmptyOrLiquid, BlockType thisType, int &countVtx,
                          std::vector<GLuint> &idx, std::vector<glm::vec4> &pnt, std::vector<glm::ivec2> &flags)
{
    glm::vec4 eastNorm = glm::vec4(1, 0, 0, 0);
    glm::vec4 southNorm = glm::vec4(0, 0, 1, 0);
    glm::vec4 westNorm = glm::vec4(-1, 0, 0, 0);
    glm::vec4 northNorm = glm::vec4(0, 0, -1, 0);
    glm::vec4 aboveNorm = glm::vec4(0, 1, 0, 0);
    glm::vec4 belowNorm = glm::vec4(0, -1, 0, 0);

    std::vector<glm::vec4> thisUV = uve[BlockWrapper(thisType)];

    // Examine the neighbor on the east
    if (neighborsEmptyOrLiquid[0])
    {
        idx.push_back(countVtx);
        idx.push_back(countVtx + 1);
        idx.push_back(countVtx + 2);

        idx.push_back(countVtx);
        idx.push_back(countVtx + 2);
        idx.push_back(countVtx + 3);

        countVtx += 4;

        pnt.push_back(globalPos + glm::vec4(1, 1, 0, 0));
        pnt.push_back(eastNorm);
        pnt.push_back(thisUV[0]);

        pnt.push_back(globalPos + glm::vec4(1, 1, 1, 0));
        pnt.push_back(eastNorm);
        pnt.push_back(thisUV[1]);

        pnt.push_back(globalPos + glm::vec4(1, 0, 1, 0));
        pnt.push_back(eastNorm);
        pnt.push_back(thisUV[2]);

        pnt.push_back(globalPos + glm::vec4(1, 0, 0, 0));
        pnt.push_back(eastNorm);
        pnt.push_back(thisUV[3]);

        flags.push_back(glm::ivec2(0, 0));
        flags.push_back(glm::ivec2(0, 0));
        flags.push_back(glm::ivec2(0, 0));
        flags.push_back(glm::ivec2(0, 0));

    }

    // Examine the neighbor on the south
    if (neighborsEmptyOrLiquid[1])
    {
        idx.push_back(countVtx);
        idx.push_back(countVtx + 1);
        idx.push_back(countVtx + 2);

        idx.push_back(countVtx);
        idx.push_back(countVtx + 2);
        idx.push_back(countVtx + 3);

        countVtx += 4;

        pnt.push_back(globalPos + glm::vec4(1, 1, 1, 0));
        pnt.push_back(southNorm);
        pnt.push_back(thisUV[4]);

        pnt.push_back(globalPos + glm::vec4(0, 1, 1, 0));
        pnt.push_back(southNorm);
        pnt.push_back(thisUV[5]);

        pnt.push_back(globalPos + glm::vec4(0, 0, 1, 0));
        pnt.push_back(southNorm);
        pnt.push_back(thisUV[6]);

        pnt.push_back(globalPos + glm::vec4(1, 0, 1, 0));
        pnt.push_back(southNorm);
        pnt.push_back(thisUV[7]);

        flags.push_back(glm::ivec2(0, 0));
        flags.push_back(glm::ivec2(0, 0));
        flags.push_back(glm::ivec2(0, 0));
        flags.push_back(glm::ivec2(0, 0));

    }

    // Examine the neighbor on the west
    if (neighborsEmptyOrLiquid[2])
    {
        idx.push_back(countVtx);
        idx.push_back(countVtx + 1);
        idx.push_back(countVtx + 2);

        idx.push_back(countVtx);
        idx.push_back(countVtx + 2);
        idx.push_back(countVtx + 3);

        countVtx += 4;

        pnt.push_back(globalPos + glm::vec4(0, 1, 1, 0));
        pnt.push_back(westNorm);
        pnt.push_back(thisUV[8]);

        pnt.push_back(globalPos + glm::vec4(0, 1, 0, 0));
        pnt.push_back(westNorm);
        pnt.push_back(thisUV[9]);

        pnt.push_back(globalPos);
        pnt.push_back(westNorm);
        pnt.push_back(thisUV[10]);

        pnt.push_back(globalPos + glm::vec4(0, 0, 1, 0));
        pnt.push_back(westNorm);
        pnt.push_back(thisUV[11]);

        flags.push_back(glm::ivec2(0, 0));
        flags.push_back(glm::ivec2(0, 0));
        flags.push_back(glm::ivec2(0, 0));
        flags.push_back(glm::ivec2(0, 0));

    }

    // Examine the neighbor on the north
    if (neighborsEmptyOrLiquid[3])
    {
        idx.push_back(countVtx);
        idx.push_back(countVtx + 1);
        idx.push_back(countVtx + 2);

        idx.push_back(countVtx);
        idx.push_back(countVtx + 2);
        idx.push_back(countVtx + 3);

        countVtx += 4;

        pnt.push_back(globalPos + glm::vec4(0, 1, 0, 0));
        pnt.push_back(northNorm);
        pnt.push_back(thisUV[12]);

        pnt.push_back(globalPos + glm::vec4(1, 1, 0, 0));
        pnt.push_back(northNorm);
        pnt.push_back(thisUV[13]);

        pnt.push_back(globalPos + glm::vec4(1, 0, 0, 0));
        pnt.push_back(northNorm);
        pnt.push_back(thisUV[14]);

        pnt.push_back(globalPos);
        pnt.push_back(northNorm);
        pnt.push_back(thisUV[15]);

        flags.push_back(glm::ivec2(0, 0));
        flags.push_back(glm::ivec2(0, 0));
        flags.push_back(glm::ivec2(0, 0));
        flags.push_back(glm::ivec2(0, 0));

    }

    // Examine the neighbor above
    if (neighborsEmptyOrLiquid[4])
    {
        idx.push_back(countVtx);
        idx.push_back(countVtx + 1);
        idx.push_back(countVtx + 2);

        idx.push_back(countVtx);
        idx.push_back(countVtx + 2);
        idx.push_back(countVtx + 3);

        countVtx += 4;

        pnt.push_back(globalPos + glm::vec4(1, 1, 0, 0));
        pnt.push_back(aboveNorm);
        pnt.push_back(thisUV[16]);

        pnt.push_back(globalPos + glm::vec4(0, 1, 0, 0));
        pnt.push_back(aboveNorm);
        pnt.push_back(thisUV[17]);

        pnt.push_back(globalPos + glm::vec4(0, 1, 1, 0));
        pnt.push_back(aboveNorm);
        pnt.push_back(thisUV[18]);

        pnt.push_back(globalPos + glm::vec4(1, 1, 1, 0));
        pnt.push_back(aboveNorm);
        pnt.push_back(thisUV[19]);

        flags.push_back(glm::ivec2(0, 0));
        flags.push_back(glm::ivec2(0, 0));
        flags.push_back(glm::ivec2(0, 0));
        flags.push_back(glm::ivec2(0, 0));


    }

    // Examine the neighbor below
    if (neighborsEmptyOrLiquid[5])
    {
        idx.push_back(countVtx);
        idx.push_back(countVtx + 1);
        idx.push_back(countVtx + 2);

        idx.push_back(countVtx);
        idx.push_back(countVtx + 2);
        idx.push_back(countVtx + 3);

        countVtx += 4;

        pnt.push_back(globalPos + glm::vec4(1, 0, 0, 0));
        pnt.push_back(belowNorm);
        pnt.push_back(thisUV[20]);

        pnt.push_back(globalPos + glm::vec4(1, 0, 1, 0));
        pnt.push_back(belowNorm);
        pnt.push_back(thisUV[21]);

        pnt.push_back(globalPos + glm::vec4(0, 0, 1, 0));
        pnt.push_back(belowNorm);
        pnt.push_back(thisUV[22]);

        pnt.push_back(globalPos);
        pnt.push_back(belowNorm);
        pnt.push_back(thisUV[23]);

        flags.push_back(glm::ivec2(0, 0));
        flags.push_back(glm::ivec2(0, 0));
        flags.push_back(glm::ivec2(0, 0));
        flags.push_back(glm::ivec2(0, 0));

    }

}

void Chunk::createOneLiquidCube(glm::vec4 globalPos, std::vector<bool> neighborsEmpty, BlockType thisType, int &countVtx, std::vector<GLuint> &idx, std::vector<glm::vec4> &pnt, std::vector<glm::ivec2> &flags)
{
    glm::vec4 eastNorm = glm::vec4(1, 0, 0, 0);
    glm::vec4 southNorm = glm::vec4(0, 0, 1, 0);
    glm::vec4 westNorm = glm::vec4(-1, 0, 0, 0);
    glm::vec4 northNorm = glm::vec4(0, 0, -1, 0);
    glm::vec4 aboveNorm = glm::vec4(0, 1, 0, 0);

    std::vector<glm::vec4> thisUV = uve[BlockWrapper(thisType)];

    // Render the side faces if the liquid touches air
    // Examine the neighbor on the east
    if (neighborsEmpty[0])
    {
        idx.push_back(countVtx);
        idx.push_back(countVtx + 1);
        idx.push_back(countVtx + 2);

        idx.push_back(countVtx);
        idx.push_back(countVtx + 2);
        idx.push_back(countVtx + 3);

        countVtx += 4;

        pnt.push_back(globalPos + glm::vec4(1, 1, 0, 0));
        pnt.push_back(eastNorm);
        pnt.push_back(thisUV[0]);

        pnt.push_back(globalPos + glm::vec4(1, 1, 1, 0));
        pnt.push_back(eastNorm);
        pnt.push_back(thisUV[1]);

        pnt.push_back(globalPos + glm::vec4(1, 0, 1, 0));
        pnt.push_back(eastNorm);
        pnt.push_back(thisUV[2]);

        pnt.push_back(globalPos + glm::vec4(1, 0, 0, 0));
        pnt.push_back(eastNorm);
        pnt.push_back(thisUV[3]);

        flags.push_back(glm::ivec2(1, 0));
        flags.push_back(glm::ivec2(1, 0));
        flags.push_back(glm::ivec2(1, 0));
        flags.push_back(glm::ivec2(1, 0));

    }

    // Examine the neighbor on the south
    if (neighborsEmpty[1])
    {
        idx.push_back(countVtx);
        idx.push_back(countVtx + 1);
        idx.push_back(countVtx + 2);

        idx.push_back(countVtx);
        idx.push_back(countVtx + 2);
        idx.push_back(countVtx + 3);

        countVtx += 4;

        pnt.push_back(globalPos + glm::vec4(1, 1, 1, 0));
        pnt.push_back(southNorm);
        pnt.push_back(thisUV[4]);

        pnt.push_back(globalPos + glm::vec4(0, 1, 1, 0));
        pnt.push_back(southNorm);
        pnt.push_back(thisUV[5]);

        pnt.push_back(globalPos + glm::vec4(0, 0, 1, 0));
        pnt.push_back(southNorm);
        pnt.push_back(thisUV[6]);

        pnt.push_back(globalPos + glm::vec4(1, 0, 1, 0));
        pnt.push_back(southNorm);
        pnt.push_back(thisUV[7]);

        flags.push_back(glm::ivec2(1, 0));
        flags.push_back(glm::ivec2(1, 0));
        flags.push_back(glm::ivec2(1, 0));
        flags.push_back(glm::ivec2(1, 0));

    }

    // Examine the neighbor on the west
    if (neighborsEmpty[2])
    {
        idx.push_back(countVtx);
        idx.push_back(countVtx + 1);
        idx.push_back(countVtx + 2);

        idx.push_back(countVtx);
        idx.push_back(countVtx + 2);
        idx.push_back(countVtx + 3);

        countVtx += 4;

        pnt.push_back(globalPos + glm::vec4(0, 1, 1, 0));
        pnt.push_back(westNorm);
        pnt.push_back(thisUV[8]);

        pnt.push_back(globalPos + glm::vec4(0, 1, 0, 0));
        pnt.push_back(westNorm);
        pnt.push_back(thisUV[9]);

        pnt.push_back(globalPos);
        pnt.push_back(westNorm);
        pnt.push_back(thisUV[10]);

        pnt.push_back(globalPos + glm::vec4(0, 0, 1, 0));
        pnt.push_back(westNorm);
        pnt.push_back(thisUV[11]);

        flags.push_back(glm::ivec2(1, 0));
        flags.push_back(glm::ivec2(1, 0));
        flags.push_back(glm::ivec2(1, 0));
        flags.push_back(glm::ivec2(1, 0));

    }

    // Examine the neighbor on the north
    if (neighborsEmpty[3])
    {
        idx.push_back(countVtx);
        idx.push_back(countVtx + 1);
        idx.push_back(countVtx + 2);

        idx.push_back(countVtx);
        idx.push_back(countVtx + 2);
        idx.push_back(countVtx + 3);

        countVtx += 4;

        pnt.push_back(globalPos + glm::vec4(0, 1, 0, 0));
        pnt.push_back(northNorm);
        pnt.push_back(thisUV[12]);

        pnt.push_back(globalPos + glm::vec4(1, 1, 0, 0));
        pnt.push_back(northNorm);
        pnt.push_back(thisUV[13]);

        pnt.push_back(globalPos + glm::vec4(1, 0, 0, 0));
        pnt.push_back(northNorm);
        pnt.push_back(thisUV[14]);

        pnt.push_back(globalPos);
        pnt.push_back(northNorm);
        pnt.push_back(thisUV[15]);

        flags.push_back(glm::ivec2(1, 0));
        flags.push_back(glm::ivec2(1, 0));
        flags.push_back(glm::ivec2(1, 0));
        flags.push_back(glm::ivec2(1, 0));

    }

    // Render the top surface
    if (neighborsEmpty[4])
    {
        idx.push_back(countVtx);
        idx.push_back(countVtx + 1);
        idx.push_back(countVtx + 2);

        idx.push_back(countVtx);
        idx.push_back(countVtx + 2);
        idx.push_back(countVtx + 3);

        countVtx += 4;

        pnt.push_back(globalPos + glm::vec4(1, 1, 0, 0));
        pnt.push_back(aboveNorm);
        pnt.push_back(thisUV[16]);

        pnt.push_back(globalPos + glm::vec4(0, 1, 0, 0));
        pnt.push_back(aboveNorm);
        pnt.push_back(thisUV[17]);

        pnt.push_back(globalPos + glm::vec4(0, 1, 1, 0));
        pnt.push_back(aboveNorm);
        pnt.push_back(thisUV[18]);

        pnt.push_back(globalPos + glm::vec4(1, 1, 1, 0));
        pnt.push_back(aboveNorm);
        pnt.push_back(thisUV[19]);

        flags.push_back(glm::ivec2(1, 0));
        flags.push_back(glm::ivec2(1, 0));
        flags.push_back(glm::ivec2(1, 0));
        flags.push_back(glm::ivec2(1, 0));
    }

}

GLenum Chunk::drawMode()
{
    return GL_TRIANGLES;
}

/// ================= Chunk Class ======================

/// ================= ChunkLiquids Class ======================

ChunkLiquids::ChunkLiquids(OpenGLContext *context) : Drawable(context), idx(), pnt(), flags()
{}

ChunkLiquids::~ChunkLiquids()
{}

void ChunkLiquids::setIdx(std::vector<GLuint> &idxLiquids)
{
    idx = idxLiquids;
}

void ChunkLiquids::setPNT(std::vector<glm::vec4> &pntLiquids)
{
    pnt = pntLiquids;
}

void ChunkLiquids::setFlags(std::vector<glm::ivec2> &flagsLiquids)
{
    flags = flagsLiquids;
}

void ChunkLiquids::create()
{
    count = idx.size();

    if (count != 0)
    {
        generateIdx();
        context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
        context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx.size() * sizeof(GLuint), idx.data(), GL_STATIC_DRAW);

        generateAttr();
        context->glBindBuffer(GL_ARRAY_BUFFER, bufAttr);
        context->glBufferData(GL_ARRAY_BUFFER, pnt.size() * sizeof(glm::vec4), pnt.data(), GL_STATIC_DRAW);

        generateFlag();
        context->glBindBuffer(GL_ARRAY_BUFFER, bufFlag);
        context->glBufferData(GL_ARRAY_BUFFER, flags.size() * sizeof(glm::ivec2), flags.data(), GL_STATIC_DRAW);
    }

    idx.clear();
    pnt.clear();
    flags.clear();
}

GLenum ChunkLiquids::drawMode()
{
    return GL_TRIANGLES;
}

/// ================= ChunkLiquids Class ======================

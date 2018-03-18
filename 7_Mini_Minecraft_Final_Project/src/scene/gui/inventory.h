#ifndef INVENTORY_H
#define INVENTORY_H

#include <QHash>
#include <scene/terrain.h>
#include "item.h"
#include "itemframe.h"

class Inventory
{
public:
    Inventory(OpenGLContext* context);
    virtual ~Inventory();

    int m_selectedListIdx;
    int itemListSize = 9;
    std::vector<Item*> m_inventory;
    int getListSize();

};

#endif // INVENTORY_H

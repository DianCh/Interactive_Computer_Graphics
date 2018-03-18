#include "inventory.h"

Inventory::Inventory(OpenGLContext* context): m_selectedListIdx(-1)
{

}

int Inventory::getListSize() {
    return itemListSize;
}

Inventory::~Inventory(){
}


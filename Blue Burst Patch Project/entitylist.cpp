#include "entitylist.h"

namespace EntityList
{
    uint32_t* playerCount = reinterpret_cast<uint32_t*>(0x00aae168);
    uint32_t* enemyCount = reinterpret_cast<uint32_t*>(0x00aae164);
    uint32_t* objectCount = reinterpret_cast<uint32_t*>(0x00aae160);
    uint32_t* totalEntityCount = reinterpret_cast<uint32_t*>(0x00aae16c);

    void** entityList = reinterpret_cast<void**>(0x00aad720);

    EntityIterator::Iterator::reference EntityIterator::Iterator::operator*() const { return *ptr; }
    EntityIterator::Iterator::pointer EntityIterator::Iterator::operator->() { return ptr; }
    EntityIterator::Iterator& EntityIterator::Iterator::operator++() { ptr++; return *this; }
    EntityIterator::Iterator EntityIterator::Iterator::operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
    EntityIterator::Iterator::Iterator(EntityIterator::Iterator::pointer ptr) : ptr(ptr) {}

    EntityIterator::EntityIterator(void** start, size_t length) : start(start), length(length) {}

    EntityIterator::Iterator EntityIterator::begin() { return Iterator(start); }
    EntityIterator::Iterator EntityIterator::end() { return Iterator(&start[length]); }

    bool operator==(const EntityIterator::Iterator& a, const EntityIterator::Iterator& b) { return a.ptr == b.ptr; }
    bool operator!=(const EntityIterator::Iterator& a, const EntityIterator::Iterator& b) { return a.ptr != b.ptr; }

    EntityIterator Players()
    {
        return EntityIterator(entityList, *playerCount);
    }

    EntityIterator Enemies()
    {
        return EntityIterator(entityList + *playerCount, *enemyCount);
    }

    EntityIterator Objects()
    {
        return EntityIterator(entityList + *playerCount + *enemyCount, *objectCount);
    }

    EntityIterator PlayersAndEnemies()
    {
        return EntityIterator(entityList, *playerCount + *enemyCount);
    }

    void* FindEntity(Entity::EntityIndex entityIndex)
    {
        if (entityIndex == -1) return nullptr;

        for (auto it = entityList, end = entityList + *totalEntityCount; it != end; it++)
        {
            auto entity = Entity::BaseEntityWrapper(*it);
            if (entity.entityIndex() == entityIndex)
            {
                return *it;
            }
        }

        return nullptr;
    }
};

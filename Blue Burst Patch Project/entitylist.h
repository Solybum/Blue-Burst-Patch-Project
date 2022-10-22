#pragma once

#include <cstdint>
#include <iterator>
#include "common.h"
#include "entity.h"

namespace EntityList
{
    extern uint32_t* playerCount;
    extern uint32_t* enemyCount;
    extern uint32_t* objectCount;
    extern uint32_t* totalEntityCount;

    extern void** entityList;

    class EntityIterator
    {
    public:
        struct Iterator
        {
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = void*;
            using pointer = void**;
            using reference = void*&;

            Iterator(pointer ptr);

            reference operator*() const;
            pointer operator->();
            Iterator& operator++();
            Iterator operator++(int);
            friend bool operator==(const Iterator& a, const Iterator& b);
            friend bool operator!=(const Iterator& a, const Iterator& b);
        
        private:
            pointer ptr;
        };

        EntityIterator(void** start, size_t length);

        Iterator begin();
        Iterator end();

    private:
        void** start;
        size_t length;
    };

    EntityIterator Players();
    EntityIterator Enemies();
    EntityIterator Objects();
    EntityIterator PlayersAndEnemies();
    void* FindEntity(Entity::EntityIndex entityIndex);
};

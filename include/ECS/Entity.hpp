#pragma once

// Includes
#include <iostream>
#include <bitset>
// #include <cstdint>

typedef std::bitset<64> Signature; // 64 -> maximum number of different components
#include "Types.hpp"

namespace ecs::internal
{

struct Entity
{
    
public:
    Entity(entity_id id_)
    : id(id_)
    , signature(0)
    { }
    
    // methods
    
    // attributes
    bool deleted = false;
    entity_id id;
    Signature signature = 0;
};

}

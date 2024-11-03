#pragma once

// Includes
#include <unordered_map>
#include <vector>
#include <stack>
#include <bitset>

// Dependencies
#include "Entity.hpp"

namespace ecs::internal
{

class EntityManager
{
public: // methods
    EntityManager()
    {
        Entity reserved(0);
        reserved.deleted = true;
        entities.push_back(reserved);   
    }
    
    entity_id createEntity()
    {
        entity_id index;

        // if some deleted entities left some place, re-use it
        if (not free_indexes.empty())
        {
            index = free_indexes.top();         // get a recyclable Entity
            entities[index].deleted = false;    // declare entity as used
            free_indexes.pop();                 // remove index from stack
        }
        else
        {   
            index = entities.size();
            entities.push_back(Entity(index));
        }
        
        return index;
    }
    
    void deleteEntity(entity_id id)
    {
        if (id >= entities.size())
        {
            throw std::invalid_argument("cannot delete entity #" + std::to_string(id));
        }
        if (entities[id].deleted == true)
        {
            throw std::invalid_argument("entity #" + std::to_string(id) + " already deleted");
        }

        // queue the index for recycling
        entities[id].deleted = true;
        free_indexes.push(id);
    }
    
    const Entity& operator[](entity_id id)
    {
        if (exists(id)) return entities[id];
        else throw std::invalid_argument("entity #" + std::to_string(id) + " doesn't exists");
    }
    
    Entity& get(entity_id id)
    {
        if (exists(id)) return entities[id];
        else throw std::invalid_argument("entity #" + std::to_string(id) + " doesn't exists");
    }
    
    bool exists(entity_id id)
    {
        if (id == 0) return false; // entity 0 is reserved (used in componentArray)
        if (id < entities.size())
        {
            return !entities[id].deleted;   
        }
        return false;
    }
    
    void setComponentSignature(uint type, entity_id id)
    {
        get(id).signature.set(type);
    }
    
    void unsetComponentSignature(uint type, entity_id id)
    {
        get(id).signature.reset(type);
    }
    
    bool has(uint type, entity_id id)
    {
        return get(id).signature.test(type);   
    }
    
    std::vector<entity_id> filter(Signature s)
    {
        std::vector<entity_id> filtered;
        
        for (Entity& e : entities)
        {
            if ((e.signature|s) == e.signature) // every component from s are already in e
            {
                filtered.push_back(e.id);
            }
        }
        return filtered;
    }
    
private: // attributes
    std::vector<Entity> entities;
    std::stack<entity_id> free_indexes;    // keep track of "deleted" entities -> reassign their index
};

}
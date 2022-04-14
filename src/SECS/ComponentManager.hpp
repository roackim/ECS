#pragma once

#include <unordered_map>
#include <typeindex>
#include <vector>

#include "ComponentArray.hpp"

class ComponentManager
{
public:
    template<class Component>
    void addComponentToEntity(Component c, uint id)     // assumes entity exists
    {
        auto t = std::type_index(typeid(Component));
        if (type_to_index.count(t) == 0) // if componentArray doesn't exists create it
        {
            uint index = component_arrays.size();
            component_arrays.push_back(new ComponentArray<Component>());
            type_to_index[t] = index;
        }
        
        auto* ptr = getComponentArrayPtr<Component>();
        ptr->addComponentToEntity(c, id);
    }
    
    template<class Component>
    ComponentArray<Component>* getComponentArrayPtr() 
    {
        auto t = std::type_index(typeid(Component));
        
        if (type_to_index.count(t) == 0) return nullptr; // TODO maybe change this, also avoid double lookup
        uint i = type_to_index[t];
        
        return static_cast<ComponentArray<Component>*>(component_arrays[i]);
    }
    
// private:
    // attributes
    std::vector<IComponentArray*> component_arrays;
    std::unordered_map<std::type_index, uint> type_to_index;
};
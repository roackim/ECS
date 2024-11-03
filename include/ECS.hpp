#pragma once

#include <vector>
#include <bitset>

#include "ECS/ComponentManager.hpp"
#include "ECS/EntityManager.hpp"

#include "Types.hpp"

namespace ecs 
{
    class ECS
    {
    public:
        ecs::internal::EntityManager em;
        ecs::internal::ComponentManager cm;
        static ECS& get_ecs()
        {
            static ECS ecs_instance;   
            return ecs_instance;   
        }
    
    private:
        ECS() {}
        ECS(ECS const&);                // Don't Implement.
        void operator=(ECS const&);     // Don't implement
    };

    
    template<class... Component>
    static void constructSignature(Signature& s)
    {
        ((s.set(ECS::get_ecs().cm.getIndexFromType<Component>())), ...); // je comprend pas pourquoi ça marche ptdr
        // -> apparement ce serait appelé une "Primary Expression" en c++
    }
}

namespace ecs::entity
{
    entity_id create() { return ECS::get_ecs().em.createEntity(); }
    
    bool exists(entity_id id) { return ECS::get_ecs().em.exists(id); }
    
    void destroy(entity_id id)
    {
        const ecs::internal::Entity& e = ECS::get_ecs().em[id];
        
        // delete all components from entity
        for (entity_id i=0; i<e.signature.size(); i++)
        {
            if (e.signature.test(i) == true)
            {
                ECS::get_ecs().cm.getComponentArrayPtr(i)->deleteComponent(id);
            }   
        }
        // delete the entity
        ECS::get_ecs().em.deleteEntity(id);
    }

    
    template<class... Components>
    bool has(entity_id id)
    {
        // usage:
        // if (ecs::entity::has<Hitbox, Velocity>(entity) {} 
        
        Signature components_s;
        constructSignature<Components...>(components_s);
        
        Signature& entity_s = ECS::get_ecs().em.get(id).signature;
        
        return ((entity_s | components_s) == entity_s); 
        // if entity_s doesn't change with OR it means it has all the bits already 
        // which means it has all the required components represented by the signature components_s
    }
    
    
    template<class... Components>
    std::vector<entity_id> filter()
    {
        Signature s;
        constructSignature<Components...>(s);
        
        return ECS::get_ecs().em.filter(s);
    }
    
}

namespace ecs::component
{
    template<class Component>
    void add(Component c, entity_id id) 
    // pass constructed component
    {
        uint type = ECS::get_ecs().cm.addComponentToEntity(c, id); // get componentArray index
        ECS::get_ecs().em.setComponentSignature(type, id);
    }
    
    template<class Component>
    void add(entity_id id)
    // pass only component type
    {
        Component c; // default initialization
        uint type = ECS::get_ecs().cm.addComponentToEntity(c, id); // get componentArray index
        ECS::get_ecs().em.setComponentSignature(type, id);
    }
    
    template<class Component>
    void remove(entity_id id) 
    { 
        ECS::get_ecs().cm.deleteComponentFromEntity<Component>(id);
        uint type = ECS::get_ecs().cm.getIndexFromType<Component>();
        ECS::get_ecs().em.unsetComponentSignature(type, id);    
    }
    
    template<class Component>
    Component& get(entity_id id) 
    { 
        ecs::internal::ComponentArray<Component>* ptr = ECS::get_ecs().cm.getComponentArrayPtr<Component>();
        return ptr->getComponent(id);
    }
}

namespace ecs::system
{
    
}

namespace secs = ecs;
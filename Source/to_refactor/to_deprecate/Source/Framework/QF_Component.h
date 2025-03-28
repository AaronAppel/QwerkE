#pragma once

#include "Libraries/Mirror/Source/Mirror.h"

#include "QF_Enums.h"

namespace QwerkE {

    class GameObject;

    class Component
    {
    public:
        virtual ~Component() = default;

        virtual void Activate() { }
        virtual void Deactivate() { }

        GameObject* GetParent() { return m_pParent; };
        const GameObject* SeeParent() const { return m_pParent; };
        eComponentTags GetTag() const { return m_ComponentTag; };

        void SetParent(GameObject* a_Parent) { m_pParent = a_Parent; };
        void SetComponentTag(eComponentTags a_Tag) { m_ComponentTag = a_Tag; };

    protected:
        Component() = default;

        MIRROR_PRIVATE_MEMBERS

        GameObject* m_pParent = nullptr;
        eComponentTags m_ComponentTag = Component_Null;
    };

}

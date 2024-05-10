#pragma once

namespace QwerkE {

    struct ComponentLight
    {
        bool m_IsOn = true;
        float m_Attenuation = 1.0f;
        float m_AttenuationDecay = 100.0f;
        vec3f m_Colour = vec3f(1.f, 1.f, 1.f);
        vec3f m_Ambient = vec3f(1.f, 1.f, 1.f);
        vec3f m_Diffuse = vec3f(1.f, 1.f, 1.f);
        vec3f m_Specular = vec3f(1.f, 1.f, 1.f);
    };

}

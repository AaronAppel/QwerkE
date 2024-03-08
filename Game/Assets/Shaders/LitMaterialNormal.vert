// LitMaterialNormal.vert
#version 330 core

// Attribute input
in vec3 a_Position;
in vec3 a_Normal;
in vec2 a_UV;
in vec3 a_Tangent;
// in vec2 a_Bitangent; // Unused

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} vs_out;

// Uniforms
uniform mat4 u_WorldMat;
uniform mat4 u_ViewMat;
uniform mat4 u_ProjMat;

uniform vec3 u_LightPos;
uniform vec3 u_CamPos;

void main()
{
    vs_out.FragPos = vec3(u_WorldMat * vec4(a_Position, 1.0));   
    vs_out.TexCoords = a_UV;
    
    mat3 normalMatrix = transpose(inverse(mat3(u_WorldMat)));
    vec3 T = normalize(normalMatrix * a_Tangent);
    vec3 N = normalize(normalMatrix * a_Normal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    
    mat3 TBN = transpose(mat3(T, B, N));    
    vs_out.TangentLightPos = TBN * u_LightPos;
    vs_out.TangentViewPos  = TBN * u_CamPos;
    vs_out.TangentFragPos  = TBN * vs_out.FragPos;
        
    gl_Position = u_ProjMat * u_ViewMat * u_WorldMat * vec4(a_Position, 1.0);
}
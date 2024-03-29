#pragma once

#include "QC_Vector.h"

namespace QwerkE {

    class Mesh;
    class Model;
    struct VertexData;

    namespace MeshFactory { // #TODO Deprecate hard coded mesh data for mesh data in files

        Mesh* ImportOBJMesh(const char* fileDirectory, vec3 objScale, vec2 UVScale, bool invertFaces);

        // #TODO Implement or deprecate
        // Model* ImportOBJModel(const char* fileDirectory, vec3 objScale = vec3(1,1,1), vec2 UVScale = vec2(1,1), bool invertFaces = false) { return new Model(); };

        Mesh* CreateTestModel();
        Mesh* CreateTestPlane();
        Mesh* CreateQuad(vec2 size);
        Mesh* CreateCircle(float radius, unsigned int points, vec2 UVScale);
        Mesh* CreateCube(vec3 size, vec2 UVScale = vec2(1.0f, 1.0f), bool invertFaces = false);
        // static Mesh* CreatePlane(vec2 size, vec2 NumOfVerts, vec2 UVScale);

        Mesh* TutorialCube(vec3 size, vec2 UVScale = vec2(1.0f, 1.0f), bool invertFaces = false);

        // #TODO Make static
        /* Mesh data assignment/re-assignment */
        void GenerateBox(Mesh* mesh, vec2 size, bool invertFaces = false);
        void GenerateCube(Mesh* mesh, vec3 size, bool invertFaces = false);
    };

}


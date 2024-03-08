#pragma once

#include <vector>

#include "QF_MeshData.h"

namespace QwerkE {

    void ScaleUVCOORDS(std::vector<vec2>& verts, vec2 UVScale);
    void ScaleVertices(std::vector<vec3>& verts, vec3 scale);


    void CalculateXZUVCoords(MeshData& data);
    void CalculateUVCoords(std::vector<vec2>& UVs);
    void CalculateUVCoords(std::vector<vec3>& vertArray, int numVertices);

    void PaintObject(MeshData& colors, vec4 color); // #TODO Consider renaming as it colors/paints points

    void InvertFaces(unsigned int oldIBO[], unsigned int arraySize);

    void GenerateTangents();
    void GenerateBitangents();

}

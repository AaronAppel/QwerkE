#ifndef __MeshUtilities_H__
#define __MeshUtilities_H__

/* Mesh modifications */
// scale
void ScaleUVCOORDS(VertexData verts[], int numVerts, vec2 UVScale);
void ScaleVertices(VertexData verts[], int numVerts, vec3 scale);

// Auto UVs
void CalculateXZUVCoords(VertexData vertArray[], int numVertices);
void CalculateUVCoords(std::vector<VertexData> &verts);
void CalculateUVCoords(VertexData verts[], int numVertices);

// Color
void PaintObject(std::vector<VertexData> &verts, vec4 color); // vertex color

// Edit IBO
void InvertFaces(unsigned int oldIBO[], unsigned int arraySize); // reverse winding order

#endif //!__MeshUtilities_H__

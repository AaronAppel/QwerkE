#ifndef __MeshFactory_H__
#define __MeshFactory_H__

struct VertexData;
class Mesh;

//  TODO: Class unnecessary

class MeshFactory
{
public:
	MeshFactory() {};
	~MeshFactory() {};

	/* Mesh creation */
	// defined in MeshShapes.cpp
	static Mesh* CreateTestModel();
	static Mesh* CreateBox(vec2 size);
	static Mesh* CreateCircle(float radius, unsigned int points, vec2 UVScale);
	static Mesh* CreateCube(vec3 size, vec2 UVScale = vec2(1.0f, 1.0f), bool invertFaces = false);
	static Mesh* CreatePlane(vec2 size, vec2 NumOfVerts, vec2 UVScale);

	static Mesh* TutorialCube(vec3 size, vec2 UVScale = vec2(1.0f, 1.0f), bool invertFaces = false);

	/* Mesh data assignment/re-assignment */
	void GenerateBox(Mesh* mesh, vec2 size, bool invertFaces = false);
	void GenerateCube(Mesh* mesh, vec3 size, bool invertFaces = false);
};

#endif //!__MeshFactory_H__

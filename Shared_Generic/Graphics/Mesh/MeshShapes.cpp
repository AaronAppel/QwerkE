#include "pch.h"

/* Mesh creation */
Mesh* Mesh::CreateBox(vec2 size)
{
	VertexData verts[4];

	verts[0] = VertexData(vec3(-size.x / 2, -size.y / 2, 0), vec4(255, 255, 255, 255), vec2(0, 0), vec3(0, 0, 0));
	verts[1] = VertexData(vec3(-size.x / 2, size.y / 2, 0), vec4(255, 255, 255, 255), vec2(0, 1), vec3(0, 0, 0));
	verts[2] = VertexData(vec3(size.x / 2, size.y / 2, 0), vec4(255, 255, 255, 255), vec2(1, 1), vec3(0, 0, 0));
	verts[3] = VertexData(vec3(size.x / 2, -size.y / 2, 0), vec4(255, 255, 255, 255), vec2(1, 0), vec3(0, 0, 0));

	unsigned int indices[6] = { 0,1,2, 2,3,0 };

	Mesh* pMesh = new Mesh();
	pMesh->BufferMeshData(4, verts, 6, indices); // GL_STATIC_DRAW

	return pMesh;
};
Mesh* Mesh::CreateCircle(float radius, unsigned int points, vec2 UVScale)
{
	std::vector<VertexData> verts;
	verts.push_back(VertexData(vec3(0, 0, 0), vec4(255, 255, 255, 255), vec2(0, 1), vec3(0, 0, 0))); // center

	float t_AngleBetweenPoints = 360.0f / points; // degrees

	vec3 Position = vec3(0, 0, 0);
	float t_IncrementAngle = 360.0f;

	for (unsigned int j = 0; j < points + 1; j++) // plot circle
	{
		Position.x = cosf(t_IncrementAngle * DEG_TO_RAD) * radius;
		Position.y = 0;
		Position.z = sinf(t_IncrementAngle * DEG_TO_RAD) * radius;

		verts.push_back(VertexData(vec3(Position.x, Position.y, Position.z), vec4(255, 255, 255, 255), vec2(0, 1), vec3(0, 0, 0)));

		t_IncrementAngle -= t_AngleBetweenPoints; // clockwise
	}
	// generate indices
	std::vector<unsigned int> indices;
	for (unsigned int i = 0; i < points; i++)
	{
		indices.push_back(0); // center first
		indices.push_back(i + 1);
		indices.push_back(i + 2);
	}

	Mesh* t_pNewMesh = new Mesh();

	// generate UVCOORDS
	t_pNewMesh->CalculateXZUVCoords(verts.data(), verts.size()); // default scale 1,1
															// scale UVCOORDS
	if (UVScale != vec2(1.0f, 1.0f))
	{
		t_pNewMesh->ScaleUVCOORDS(verts.data(), verts.size(), UVScale);
	}

	// initialize mesh
	t_pNewMesh->BufferMeshData(verts.size(), verts.data(), indices.size(), indices.data()); // GL_STATIC_DRAW
	t_pNewMesh->m_PrimitiveType = GL_TRIANGLES;
	return t_pNewMesh;
}
Mesh* Mesh::CreateCube(vec3 size, vec2 UVScale, bool invertFaces)
{
	// TODO:: Fix UV values
	// image
	int imageWidth = 500; // pixels
	int imageHeight = 500; // 0.1666666666666667
	int numOfXFrames = 1;
	int numOfYFrames = 1;

	float frameWidth = (float)(imageWidth / numOfXFrames);
	float frameHeight = (float)(imageHeight / numOfYFrames);

	float UVOffsetX = frameWidth / imageWidth;
	float UVOffsetY = imageHeight / frameHeight;

	// mesh
	VertexData verts[24];

	// cube with 6 sides, 2 triangles per side
	// face 1 - front
	verts[0] = VertexData(vec3(-size.x / 2, size.y / 2, -size.z / 2), vec4(255, 255, 255, 255), vec2(0, 1), vec3(0, 0, 0)); //top-left
	verts[1] = VertexData(vec3(-size.x / 2, -size.y / 2, -size.z / 2), vec4(255, 255, 255, 255), vec2(0, 0), vec3(0, 0, 0)); // bottom-left
	verts[2] = VertexData(vec3(size.x / 2, -size.y / 2, -size.z / 2), vec4(255, 255, 255, 255), vec2(UVOffsetX, 0.0f), vec3(0, 0, 0)); // bottom-right
	verts[3] = VertexData(vec3(size.x / 2, size.y / 2, -size.z / 2), vec4(255, 255, 255, 255), vec2(UVOffsetX, 1.0f), vec3(0, 0, 0)); // top-right

	// face 2 - left
	verts[4] = VertexData(vec3(-size.x / 2, size.y / 2, -size.z / 2), vec4(255, 255, 255, 255), vec2(UVOffsetX, 1.0f), vec3(0, 0, 0));
	verts[5] = VertexData(vec3(-size.x / 2, -size.y / 2, -size.z / 2), vec4(255, 255, 255, 255), vec2(UVOffsetX, 0.0f), vec3(0, 0, 0));
	verts[6] = VertexData(vec3(-size.x / 2, -size.y / 2, size.z / 2), vec4(255, 255, 255, 255), vec2(UVOffsetX * 2, 0.0f), vec3(0, 0, 0));
	verts[7] = VertexData(vec3(-size.x / 2, size.y / 2, size.z / 2), vec4(255, 255, 255, 255), vec2(UVOffsetX * 2, 1.0f), vec3(0, 0, 0));

	// face 3 - back
	verts[8] = VertexData(vec3(size.x / 2, size.y / 2, size.z / 2), vec4(255, 255, 255, 255), vec2(UVOffsetX * 2, 1.0f), vec3(0, 0, 0));
	verts[9] = VertexData(vec3(size.x / 2, -size.y / 2, size.z / 2), vec4(255, 255, 255, 255), vec2(UVOffsetX * 2, 0.0f), vec3(0, 0, 0));
	verts[10] = VertexData(vec3(-size.x / 2, -size.y / 2, size.z / 2), vec4(255, 255, 255, 255), vec2(UVOffsetX * 3, 0.0f), vec3(0, 0, 0));
	verts[11] = VertexData(vec3(-size.x / 2, size.y / 2, size.z / 2), vec4(255, 255, 255, 255), vec2(UVOffsetX * 3, 1.0f), vec3(0, 0, 0));

	// face 4 - right
	verts[12] = VertexData(vec3(size.x / 2, size.y / 2, -size.z / 2), vec4(255, 255, 255, 255), vec2(UVOffsetX * 3, 1.0f), vec3(0, 0, 0));
	verts[13] = VertexData(vec3(size.x / 2, -size.y / 2, -size.z / 2), vec4(255, 255, 255, 255), vec2(UVOffsetX * 3, 0.0f), vec3(0, 0, 0));
	verts[14] = VertexData(vec3(size.x / 2, -size.y / 2, size.z / 2), vec4(255, 255, 255, 255), vec2(UVOffsetX * 4, 0.0f), vec3(0, 0, 0));
	verts[15] = VertexData(vec3(size.x / 2, size.y / 2, size.z / 2), vec4(255, 255, 255, 255), vec2(UVOffsetX * 4, 1.0f), vec3(0, 0, 0));

	// face 5 - top - z axis becomes vertical + reference
	verts[16] = VertexData(vec3(-size.x / 2, size.y / 2, size.z / 2), vec4(255, 255, 255, 255), vec2(UVOffsetX * 4, 1.0f), vec3(0, 0, 0));
	verts[17] = VertexData(vec3(-size.x / 2, size.y / 2, -size.z / 2), vec4(255, 255, 255, 255), vec2(UVOffsetX * 4, 0.0f), vec3(0, 0, 0));
	verts[18] = VertexData(vec3(size.x / 2, size.y / 2, -size.z / 2), vec4(255, 255, 255, 255), vec2(UVOffsetX * 5, 0.0f), vec3(0, 0, 0));
	verts[19] = VertexData(vec3(size.x / 2, size.y / 2, size.z / 2), vec4(255, 255, 255, 255), vec2(UVOffsetX * 5, 1.0f), vec3(0, 0, 0));

	// face 6 - bottom
	verts[20] = VertexData(vec3(-size.x / 2, -size.y / 2, size.z / 2), vec4(255, 255, 255, 255), vec2(UVOffsetX * 5, 1.0f), vec3(0, 0, 0));
	verts[21] = VertexData(vec3(-size.x / 2, -size.y / 2, -size.z / 2), vec4(255, 255, 255, 255), vec2(UVOffsetX * 5, 0.0f), vec3(0, 0, 0));
	verts[22] = VertexData(vec3(size.x / 2, -size.y / 2, -size.z / 2), vec4(255, 255, 255, 255), vec2(UVOffsetX * 6, 0.0f), vec3(0, 0, 0));
	verts[23] = VertexData(vec3(size.x / 2, -size.y / 2, size.z / 2), vec4(255, 255, 255, 255), vec2(UVOffsetX * 6, 1.0f), vec3(0, 0, 0));

	unsigned int indices[36] = {
		0,3,2, 2,1,0, // front - brown
		6,7,4, 4,5,6, // left - pink
		10,9,8, 8,11,10, // back - blue
		12,15,14, 14,13,12, // right - yellow
		16,19,18, 18,17,16, // top - red
		22,23,20, 20,21,22 // bottom - green
	};

	Mesh* t_pNewMesh = new Mesh();

	// scale UVCOORDS
	if (UVScale != vec2(1.0f, 1.0f))
	{
		t_pNewMesh->ScaleUVCOORDS(verts, 24, UVScale);
	}

	if (invertFaces) // invert faces
	{
		t_pNewMesh->InvertFaces(indices, 36);
	}

	t_pNewMesh->BufferMeshData(24, verts, 36, indices); // GL_STATIC_DRAW
	t_pNewMesh->m_PrimitiveType = GL_TRIANGLES;

	return t_pNewMesh;
}
// worldSize, number of vertices, UVPos,
Mesh* Mesh::CreatePlane(vec2 size, vec2 NumOfVerts, vec2 UVScale) // pivot, size, UVStart, UVOffset
{
	//// variables
	int numVertsX = (int)NumOfVerts.x;
	int numVertsY = (int)NumOfVerts.y;
	int numVerts = numVertsX * numVertsY;

	// create array
	VertexData *verts = new VertexData[numVerts]; //  (vertexCount.x-1)*(vertexCount-1)*2 triangles per square*3 indices per triangle;

	float width = size.x;
	float height = size.y;

	// spacing
	float xSpacing = width / (numVertsX - 1);
	float ySpacing = height / (numVertsY - 1);

	// for-loop variables
	// increment
	float xIncrement = -width * 0.5f;
	float zIncrement = -height * 0.5f;

	int indexCounter = 0;

	for (int i = 0; i < numVertsX; i++)
	{
		for (int j = 0; j < numVertsY; j++)
		{
			verts[indexCounter] = VertexData(vec3(xIncrement, 0, zIncrement), vec4(255, 255, 255, 255), vec2(0, 0), vec3(0, 0, 0));
			indexCounter++;
			xIncrement += xSpacing;
		}
		xIncrement = -width * 0.5f;
		zIncrement += ySpacing;
	}

	const int numOfIndices = 6 * (numVertsX - 1) * (numVertsY - 1); // 6 points per rectangle * number of rectangles
	unsigned int* indices = new unsigned int[numOfIndices]; // 6 * numOfRectangles
	indexCounter = 0;
	int vertex = 0;

	// start at bottom left corner, create left most box by taking y point from above, then current y points (clockwise)
	// then move right, at end of x row, increment y and continue
	for (int i = 0; i < numVertsY - 1; i++) // numVertsY - 1 = number of spaces between points on that axis, which tell how many rectangles
	{
		// y
		for (int j = 0; j < numVertsX - 1; j++)
		{
			//x
			// top right triangle
			indices[indexCounter] = vertex + numVertsX;
			indices[indexCounter + 1] = vertex + numVertsX + 1;
			indices[indexCounter + 2] = vertex + 1;

			// bottom left triangle
			indices[indexCounter + 3] = vertex + 1;
			indices[indexCounter + 4] = vertex;
			indices[indexCounter + 5] = vertex + numVertsX;
			indexCounter += 6; // next triangle
			vertex++; // next point
		}
		vertex++;
	}

	Mesh* pMesh = new Mesh();

	// generate UVCOORDS
	pMesh->CalculateXZUVCoords(verts, numVerts);

	// scale UVCOORDS
	pMesh->ScaleUVCOORDS(verts, numVerts, UVScale);

	pMesh->BufferMeshData(numVerts, verts, numOfIndices, indices); // GL_STATIC_DRAW
	pMesh->m_PrimitiveType = GL_TRIANGLES;

	//pMesh->m_PrimitiveType = GL_POINTS;
	//glPointSize(10);

	//glPolygonMode(GL_FRONT, GL_LINE);

	delete[] verts;
	delete[] indices;

	return pMesh;
}
/* Mesh data assignment*/
void Mesh::GenerateBox(vec2 size)
{
	std::vector<VertexData> tempList;
	tempList.push_back(VertexData(vec3(0.5f * size.x, 0.5f * size.y, 0.0f), vec4(1,1,1,1), vec2(1,1), vec3(0,0,0)));
	tempList.push_back(VertexData(vec3(0.5f * size.x, -0.5f * size.y, 0.0f), vec4(1, 1, 1, 1), vec2(1, 0), vec3(0, 0, 0)));
	tempList.push_back(VertexData(vec3(-0.5f * size.x, -0.5f * size.y, 0.0f), vec4(1, 1, 1, 1), vec2(0, 0), vec3(0, 0, 0)));
	tempList.push_back(VertexData(vec3(-0.5f * size.x, 0.5f * size.y, 0.0f), vec4(1, 1, 1, 1), vec2(0, 1), vec3(0, 0, 0)));

	unsigned int indices[] = {
		0, 1, 3, // First Triangle
		1, 2, 3  // Second Triangle
	};

	this->BufferMeshData(4, tempList.data(), 6, indices);
}

void Mesh::GenerateCube(vec3 size)
{
	std::vector<VertexData> tempList;
	// front
	tempList.push_back(VertexData(vec3(-0.5f * size.x, -0.5f * size.y, 0.5f * size.z), vec4(1, 1, 1, 1), vec2(0, 0), vec3(0, 0, 0))); // 0
	tempList.push_back(VertexData(vec3(-0.5f * size.x, 0.5f * size.y, 0.5f * size.z), vec4(1, 1, 1, 1), vec2(0, 1), vec3(0, 0, 0)));
	tempList.push_back(VertexData(vec3(0.5f * size.x, 0.5f * size.y, 0.5f * size.z), vec4(1, 1, 1, 1), vec2(1, 1), vec3(0, 0, 0)));
	tempList.push_back(VertexData(vec3(0.5f * size.x, -0.5f * size.y, 0.5f * size.z), vec4(1, 1, 1, 1), vec2(1, 0), vec3(0, 0, 0)));
	// back
	tempList.push_back(VertexData(vec3(-0.5f * size.x, -0.5f * size.y, -0.5f * size.z), vec4(1, 1, 1, 1), vec2(0, 0), vec3(0, 0, 0))); // 4
	tempList.push_back(VertexData(vec3(-0.5f * size.x, 0.5f * size.y, -0.5f * size.z), vec4(1, 1, 1, 1), vec2(0, 1), vec3(0, 0, 0)));
	tempList.push_back(VertexData(vec3(0.5f * size.x, 0.5f * size.y, -0.5f * size.z), vec4(1, 1, 1, 1), vec2(1, 1), vec3(0, 0, 0)));
	tempList.push_back(VertexData(vec3(0.5f * size.x, -0.5f * size.y, -0.5f * size.z), vec4(1, 1, 1, 1), vec2(1, 0), vec3(0, 0, 0)));

	unsigned int indices[] ={
		// front
		0,1,2,
		2,3,0,
		// back
		6,5,4,
		4,7,6,
		// right
		3,2,6,
		6,7,3,
		// left
		4,5,1,
		1,0,4,
		// bottom
		2,1,5,
		5,6,2,
		// top
		4,0,3,
		3,7,4
	};

	this->BufferMeshData(8, tempList.data(), 36, indices);
}
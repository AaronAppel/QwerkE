#ifndef __OBJLoader_H__
#define __OBJLoader_H__

class Model;

Model* ImportOBJModel(const char* fileDirectory, vec3 objScale = 1.0f, vec2 UVScale = 1.0f, bool invertFaces = false);

#endif //!__OBJLoader_H__

#pragma once

namespace QwerkE {

	class Mesh;

	namespace Meshes {

		void Initialize();

		Mesh* CreateCube();
		Mesh* CreatePlane();
		Mesh* CreateTriangle();
	}

}

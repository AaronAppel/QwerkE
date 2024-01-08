#TODO Review and improve shader documentation. Create a shader section/folder, inside of a graphics or rendering section
// Create a rendering folder and start putting things in there.

.vertex values
// Transform
uniform mat4 u_2DTransform // 2D transform. From object's sc/ro/trans.
uniform mat4 u_WorldMat // 3D world space transform. From object's transform.
uniform mat4 u_ViewMat // 3D view space transform. From camera's transform.
uniform mat4 u_ProjMat // 3D projection space transform. From scene settings.

// Camera
uniform vec3 u_CamPos // Location of the camera in the world.

// Lighting
uniform vec3 u_LightPos // Location of the light in the world.

.fragment values
// Texture
uniform sampler2D u_Texture0; // Each texture found is counted and images are added from the object's image list

// Material
uniform float u_Shine // Modifies the shine for this object's material. From object's material.

.geometry values
// Currently unsupported
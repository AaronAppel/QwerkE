
attribute vec3 a_Position;

uniform mat4 u_WorldMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjMatrix;
uniform vec3 u_Position;
uniform vec2 u_Resoultion;

void main()
{
	vec4 objectpos = vec4(a_Position, 1);
	vec4 worldpos = u_WorldMatrix * objectpos;
	vec4 viewpos = u_ViewMatrix * worldpos;
	vec4 clippos = u_ProjMatrix * viewpos;
	gl_Position = clippos;
}

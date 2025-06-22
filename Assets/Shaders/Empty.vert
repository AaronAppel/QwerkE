#include <bgfx_shader.sh>

$input a_position

uniform vec4 u_scale;
uniform vec4 u_rotation;
uniform vec4 u_position;
uniform vec4 u_cameraPosition;
uniform vec4 u_projectionScale;

vec2 transformIntoWorldSpace(vec2 pos);

void main()
{
	vec2 objectSpacePosition = a_position;
	vec2 worldSpacePosition = transformIntoWorldSpace( objectSpacePosition );
	vec2 viewSpacePosition = worldSpacePosition - u_cameraPosition.xy;
	vec2 clipSpacePosition = viewSpacePosition * u_projectionScale.xy;
	gl_Position = a_position; // vec4( clipSpacePosition, 0, 1 );
}

vec2 transformIntoWorldSpace(vec2 pos)
{
	// Scale.
	pos *= u_scale.xy;
	
	// Rotate.
	float newX = cos(u_rotation.x)*pos.x + -sin(u_rotation.x)*pos.y;
	float newY = sin(u_rotation.x)*pos.x + cos(u_rotation.x)*pos.y;
	pos = vec2(newX, newY);
	
	// Translate.
	pos += u_position.xy;

	return pos;
}

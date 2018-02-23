// Basic2D.frag
#version 330 core

uniform vec4 u_ObjectColour;

out vec4 t_Colour;

void main()
{
    t_Colour = u_ObjectColour;
}
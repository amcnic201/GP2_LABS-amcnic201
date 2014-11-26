#version 150

in vec3 vertextPosition;

in vex2 vertexTexCoords;

out vec2 vertextTextCoordsOut;

uniform mat4 MVP;

void main()
{
	vertexTexCoordsOut = vertexTexCoords;
	gl_Position = MVP*vec4(vertexPosition,1.0);
}
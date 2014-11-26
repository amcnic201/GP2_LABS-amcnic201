#version 150

in vec2 vertextTexCoordsOut;

uniform sampler2d texture0;

out vec4 FragColor;

void main()
{
	FragColor = texture(texture0, vertextexCoordsOut);
}
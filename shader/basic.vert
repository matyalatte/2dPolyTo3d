#version 460 core

layout(location = 0) in vec3 aPos;

out vec3 vertexColor;
uniform mat4 modelRot;
uniform vec3 color;

void main()
{
	gl_Position = modelRot* vec4(aPos, 1.0);
	vertexColor = color;
}
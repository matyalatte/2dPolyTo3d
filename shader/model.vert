#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNorm;

out vec3 vertexColor;
out vec3 normal;
out vec3 pos;
uniform mat4 modelRot;
uniform vec3 color;

void main()
{
	gl_Position = modelRot* vec4(aPos, 1.0);
	pos = vec3(gl_Position);
	vertexColor = color;
	normal =vec3(modelRot*(vec4(normalize(aNorm)+aPos,1.0))-gl_Position);
}
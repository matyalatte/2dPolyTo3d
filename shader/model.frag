#version 460 core

out vec4 FragColor;
  
in vec3 vertexColor;
in vec3 normal;
in vec3 pos;

uniform vec3 lightPos;
vec3 lightDir;
float diff;

void main()
{
  lightDir = normalize(lightPos - pos); 
  diff = (max(dot(normal, lightDir), 0.0)+0.2)/1.2;
  FragColor = vec4(diff * vertexColor, 1.0);
} 
#version 330 core
out vec4 FragColor;

in vec3 LightColor2; 

uniform vec3 objectColor;

void main()
{
   FragColor = vec4(LightColor2 * objectColor, 1.0);
}
#version 330 core
out vec4 FragColor;

in aColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform float mixValue;

void main()
{
	//FragColor = vec4(lightColor * objectColor, mixValue);
	FragColor = aColor;
};
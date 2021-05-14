#version 330 core //Simple C Fragment Shader Program
	out vec4 FragColor;
	in vec3 ourColor;
	void main()
	{
		FragColor =	vec4(ourColor, 1.0); //right now this is a constant color
	};
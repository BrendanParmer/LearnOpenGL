#version 330 core //Simple C Fragment Shader Program
	out vec4 FragColor;
	in vec3 ourColor;

	uniform float mixValue;

	void main()
	{
		FragColor = vec4(ourColor, mixValue);
	};
#version 330 core //Simple C Fragment Shader Program
	out vec4 FragColor;
	in vec3 ourColor;
	in vec2 ourTex;

	uniform sampler2D ourTexture;

	void main()
	{
		FragColor =	texture(ourTexture, ourTex);
	};
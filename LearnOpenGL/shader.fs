#version 330 core //Simple C Fragment Shader Program
	out vec4 FragColor;
	in vec3 ourColor;
	in vec2 ourTex;

	uniform sampler2D texture1;
	uniform sampler2D texture2;

	uniform float value;

	void main()
	{
		FragColor = mix(texture(texture2, vec2(1.0 - ourTex.x, ourTex.y)), texture(texture1, ourTex), value);
	};
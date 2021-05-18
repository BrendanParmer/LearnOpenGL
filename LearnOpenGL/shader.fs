#version 330 core //Simple C Fragment Shader Program
	out vec4 FragColor;
	in vec3 ourColor;
	in vec2 ourTex;

	uniform sampler2D texture1;
	uniform sampler2D texture2;

	uniform float mixValue;

	void main()
	{
		FragColor = mix(texture(texture1, ourTex), texture(texture2, vec2(ourTex.y, ourTex.x)), pow(mixValue, 0.5));
	};
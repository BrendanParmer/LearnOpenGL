#version 330 core //Simple C Fragment Shader Program
	out vec4 FragColor;
	in vec3 ourColor;
	in vec2 ourTex;

	uniform sampler2D texture1;
	uniform float mixValue;
	uniform unsigned int thing;

	void main()
	{
		vec4 texColor = texture(texture1, ourTex);
		texColor.a = mixValue;
		FragColor = texColor + vec4(ourColor, mixValue);
	};
#version 330 core
	out vec4 FragColor;

	uniform float mixValue;
	uniform vec3 lightColor;

	void main()
	{
		FragColor = vec4(lightColor, mixValue);
	};
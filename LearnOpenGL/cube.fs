#version 330 core
	out vec4 FragColor;

	uniform float mixValue;
	uniform vec3 objectColor;
	uniform vec3 lightColor;

	uniform float ambientStrength;

	void main()
	{
		vec3 ambient = ambientStrength * lightColor;
		FragColor = vec4(ambient * objectColor, mixValue);
		//FragColor = vec4(1.0f, 0.5f, 1.0f, 1.0f);
	};
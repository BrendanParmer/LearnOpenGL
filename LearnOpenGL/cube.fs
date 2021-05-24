#version 330 core
	out vec4 FragColor;

	in vec3 Normal;
	in vec3 FragPos;

	uniform float mixValue;
	uniform vec3 objectColor;
	uniform vec3 lightColor;

	uniform float ambientStrength;
	uniform vec3 lightPos;

	void main()
	{
		//diffuse lighting
		vec3 norm = normalize(Normal);
		vec3 lightDir = normalize(lightPos - FragPos);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = diff * lightColor;


		vec3 ambient = ambientStrength * lightColor;
		FragColor = vec4((ambient + diffuse) * objectColor, mixValue);
		//FragColor = vec4(1.0f, 0.5f, 1.0f, 1.0f);
	};
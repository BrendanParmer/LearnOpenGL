#version 330 core
	out vec4 FragColor;

	in vec3 Normal;
	in vec3 FragPos;

	uniform float mixValue;
	uniform vec3 objectColor;
	uniform vec3 lightColor;

	uniform float ambientStrength;
	uniform vec3 lightPos;

	uniform float specularStrength;
	uniform vec3 viewPos;
	uniform int shininess;

	void main()
	{
		//ambient lighting
		vec3 ambient = ambientStrength * lightColor;

		//diffuse lighting
		vec3 norm = normalize(Normal);
		vec3 lightDir = normalize(lightPos - FragPos);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = diff * lightColor;

		//specular lighting
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
		vec3 specular = specularStrength * spec * lightColor;

		//final result
		vec3 result = (ambient + diffuse + specular) * objectColor;
		FragColor = vec4(result, mixValue);
	};
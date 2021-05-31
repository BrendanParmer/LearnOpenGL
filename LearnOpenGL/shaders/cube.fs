#version 330 core
	out vec4 FragColor;

	struct Material {
		sampler2D diffuse;
		sampler2D specular;
		sampler2D emission;
		float shininess;
	};
	struct Light {
		vec4 vector;
		vec3 ambient;
		vec3 diffuse;
		vec3 specular;

		float a;
		float b;
		float c;
	};

	in vec2 TexCoords;
	in vec3 Normal;
	in vec3 FragPos;

	uniform float mixValue;

	uniform Material material;
	uniform Light light;

	uniform vec3 viewPos;

	void main()
	{
		vec3 lightDir;
		float attenuation = 1.0f;
		if (light.vector.w == 0.0) //directional lighting
			lightDir = normalize(vec3(-light.vector));
		else //point lighting
		{
			lightDir = normalize(vec3(light.vector) - FragPos);
			float distance = length(vec3(light.vector) - FragPos);
			attenuation = 1.0 / (light.a * distance * distance + light.b * distance + light.c);
		}
		//ambient lighting
		vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
		FragColor = vec4(ambient, 1.0f);

		//diffuse lighting
		vec3 norm = normalize(Normal);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

		//specular lighting
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

		ambient  *= attenuation;
		diffuse  *= attenuation;
		specular *= attenuation;
		//emission
		vec3 emission = mixValue * vec3(texture(material.emission, TexCoords));

		//final result
		vec3 result = ambient + diffuse + specular + emission;
		FragColor = vec4(result, 1.0f);
	};

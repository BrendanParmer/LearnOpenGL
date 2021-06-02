#version 330 core
	out vec4 FragColor;

	struct Material {
		sampler2D diffuse;
		sampler2D specular;
		sampler2D emission;
		float shininess;
	};
	struct DirLight {
		vec3 direction;

		vec3 ambient;
		vec3 diffuse;
		vec3 specular;
	};
	struct PointLight {
		vec3 position;
	};

	in vec2 TexCoords;
	in vec3 Normal;
	in vec3 FragPos;

	uniform float mixValue;

	uniform Material material;
	uniform DirLight dirLight;

	uniform vec3 viewPos;

	vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

	void main()
	{
		
	};

	vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
	{
		vec3 lightDir = normalize(-light.direction);
		float diff = max(dot(normal, lightDir), 0.0);

		vec3 reflectDir = reflect(-lightDir, normal);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

		vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
		vec3 diffuse = light.diffuse * vec3(texture(material.diffuse, TexCoords));
		vec3 specular = light.specular * vec3(texture(material.diffuse, TexCoords));
		return (ambient + diffuse + specular);
	}
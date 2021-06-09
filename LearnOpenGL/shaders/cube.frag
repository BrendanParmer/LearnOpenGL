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

		//quadratic coefficients for attenuation
		float a;
		float b;
		float c;

		vec3 ambient;
		vec3 diffuse;
		vec3 specular;
	};

	struct SpotLight {
		vec3 position;
		vec3 direction;
		float cutOff; //already given as cosine of the angle
		float outerCutOff; //outer cone

		//quadratic coefficients for attenuation
		float a;
		float b;
		float c;

		vec3 ambient;
		vec3 diffuse;
		vec3 specular;
	};

	in vec2 TexCoords;
	in vec3 Normal;
	in vec3 FragPos;

	uniform float mixValue;

	uniform Material material;
	uniform DirLight dirlight;
	#define NUM_POINTS 10 //figure out how to conenct this with numPoints in main.cpp
	uniform PointLight points[NUM_POINTS];
	uniform SpotLight spotlight;

	uniform vec3 viewPos;

	vec3 CalcDirLight	(DirLight	light, vec3 norm, vec3 viewDir);
	vec3 CalcPointLight	(PointLight	light, vec3 norm, vec3 viewDir);
	vec3 CalcSpotLight	(SpotLight	light, vec3 norm, vec3 viewDir);

	void main()
	{
		vec3 norm = normalize(Normal);
		vec3 viewDir = normalize(viewPos - FragPos);

		//vec3 result = CalcDirLight(dirlight, norm, viewDir);
		vec3 result = vec3(0.0, 0.0, 0.0);
		for (int i = 0; i < NUM_POINTS; i++)
		{
			result += CalcPointLight(points[i], norm, viewDir);
		}
		result += CalcSpotLight(spotlight, norm, viewDir);
		FragColor = vec4(result, 1.0);
	};

	vec3 CalcDirLight(DirLight light, vec3 norm, vec3 viewDir)
	{
		//ambient
		vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

		//diffuse
		vec3 lightDir = normalize(-light.direction);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = light.diffuse * vec3(texture(material.diffuse, TexCoords));

		//specular
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		vec3 specular = light.specular * vec3(texture(material.diffuse, TexCoords));

		return (ambient + diffuse + specular);
	}

	vec3 CalcPointLight(PointLight light, vec3 norm, vec3 viewDir) {
		//ambient
		vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

		//diffuse
		vec3 lightDir = normalize(light.position - FragPos);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

		//specular
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

		//fall-off
		float distance = length(light.position - FragPos);
		float attenuation = 1.0 / (light.a * distance * distance + light.b * distance + light.c);

		ambient  *= attenuation;
		diffuse  *= attenuation;
		specular *= attenuation;

		return (ambient + diffuse + specular);
	}

	vec3 CalcSpotLight(SpotLight light, vec3 norm, vec3 viewDir) {
		vec3 lightDir = normalize(light.position - FragPos);
		
		//ambient
		vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

		//diffuse
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

		//specular
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

		float distance = length(light.position - FragPos);
		float attenuation = 1.0 / (light.a * distance * distance + light.b * distance + light.c);

		float theta = dot(lightDir, normalize(-light.direction));
		float epsilon = light.cutOff - light.outerCutOff;
		float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

		ambient  *= attenuation;
		diffuse  *= attenuation;
		specular *= attenuation;

		return (ambient + diffuse + specular);
	}
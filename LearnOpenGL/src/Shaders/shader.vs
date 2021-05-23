#version 330 core
	layout (location = 0) in vec3 aPos;

	out aColor;

	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	void main()
	{
		gl_Position = projection * view * model * vec4(aPos, 1.0f);
		vec4 aColor = (1.0f, 1.0f, 1.0f, 1.0f);
	};
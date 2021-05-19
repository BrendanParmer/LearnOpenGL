#version 330 core //Simple GLSL Vertex shader program
	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec3 aColor;
	layout (location = 2) in vec2 aTex;

	out vec3 ourColor;
	out vec2 ourTex;

	uniform mat4 transform;

	void main()
	{
		gl_Position = transform * vec4(aPos, 1.0f);
		ourColor = aColor;
		ourTex = aTex;
	};
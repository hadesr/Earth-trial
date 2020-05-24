#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 tex_coord;
layout (location = 2) in vec3 normal;

out vec3 vs_position;
out vec2 TexCoords;
out vec3 vs_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vs_position = aPos;
	TexCoords = vec2(tex_coord.x, tex_coord.y * -1.f);
	vs_normal = normal;
	gl_Position = projection * view * model * vec4(aPos, 1.0);
} 

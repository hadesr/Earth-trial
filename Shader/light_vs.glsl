#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 tex_coord;

out vec3 vs_position;
out vec2 vs_texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vs_position = aPos;
	vs_texcoord = vec2(tex_coord.x, tex_coord.y * -1.f);
	gl_Position = projection * view * model * vec4(aPos, 1.0);
} 

#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex_coord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 FragPos;
out vec2 TexCoords;
out vec4 Normal;
out mat3 TBN;

void main()
{
	gl_Position = projection * view  *model * vec4(aPos, 1);

	FragPos = model * vec4(aPos, 1); 
	Normal = model * vec4(aNormal, 0);
	TexCoords = vec2(aTex_coord.x, aTex_coord.y * -1.f);

	mat3 modelVector = transpose(inverse(mat3(model)));

	vec3 T = normalize(modelVector * aTangent);
	vec3 N = normalize(modelVector * aNormal);
	vec3 B = normalize(cross(N, T));
	TBN = mat3(T,B,N);
}

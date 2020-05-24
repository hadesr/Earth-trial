#version 330 core

in vec3 vs_position;
in vec2 vs_texcoord;

out vec4 FragColor;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, vs_texcoord);
}

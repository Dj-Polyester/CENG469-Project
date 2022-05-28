#version 460 core

in vec2 texCoord;
in vec3 vertexColor;
out vec4 fragColor;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
    fragColor = 0.8*texture(texture0, texCoord)+0.2*texture(texture1, texCoord);
}
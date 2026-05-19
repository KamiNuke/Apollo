// Basic texture shader

#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 uViewProjection;
uniform mat4 uTransform;

out vec2 vTexCoord;

void main()
{
    gl_Position = uViewProjection * uTransform * vec4(aPos, 1.0f);
    vTexCoord = aTexCoord;
}

#type fragment
#version 330 core
out vec4 FragColor;

in vec2 vTexCoord;

uniform vec3 uColor;
uniform sampler2D uTexture;

void main()
{
    FragColor = texture(uTexture, vTexCoord);
}
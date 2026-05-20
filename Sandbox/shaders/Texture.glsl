// Basic texture shader

#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in float aTexIndex;
layout (location = 4) in float aTilingFactor;


uniform mat4 uViewProjection;

out vec4 vColor;
out vec2 vTexCoord;
out float vTexIndex;
out float vTilingFactor;

void main()
{
    gl_Position = uViewProjection * vec4(aPos, 1.0f);
    vColor = aColor;
    vTexCoord = aTexCoord;
    vTexIndex = aTexIndex;
    vTilingFactor = aTilingFactor;
}

#type fragment
#version 330 core
out vec4 FragColor;

in float vTilingFactor;
in float vTexIndex;
in vec2 vTexCoord;
in vec4 vColor;

uniform sampler2D uTexture[32];

void main()
{
    FragColor = texture(uTexture[int(vTexIndex)], vTexCoord * vTilingFactor) * vColor;
}
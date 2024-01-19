#version 330

layout(std140) uniform globalMatrixBlock
{
    mat4 projection;
    mat4 view;
}block_matrices;

layout (location = 0) in vec3 pos;

out vec3 wPos;

uniform mat4 model;

void main()
{
    wPos = (model * vec4(pos, 1.0)).xyz;
    gl_Position = vec4(pos, 1.0);
}
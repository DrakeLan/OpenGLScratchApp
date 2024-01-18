#version 330

layout(std140) uniform globalMatrixBlock
{
    mat4 projection;
    mat4 view;
}block_matrices;

layout (location = 0) in vec3 pos;

uniform mat4 model;

void main()
{
    gl_Position = block_matrices.projection * block_matrices.view * model * vec4(pos, 1.0);
}
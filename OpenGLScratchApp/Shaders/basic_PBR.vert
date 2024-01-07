#version 410 core                                            

layout(std140) uniform globalMatrixBlock
{
    mat4 projection;
    mat4 view;
}block_matrices;


layout (location = 0) in vec3 Pos;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

out vec2 TexCoord;
out vec3 PositionWS;
out vec3 NormalWS;

uniform mat4 model;                                                      
                                                                            
void main()                                                                  
{         
    TexCoord = uv;
    PositionWS = (model * vec4(Pos, 1.0)).xyz;
    NormalWS = mat3(transpose(inverse(model))) * normal;

    gl_Position = block_matrices.projection * block_matrices.view * vec4(PositionWS, 1.0);
}
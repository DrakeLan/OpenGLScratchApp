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
out vec3 Normal; 


uniform mat4 model;                                                      
                                                                            
void main()                                                                  
{         
    gl_Position =  vec4(Pos, 1.0);

    TexCoord = uv;
    Normal = (model * vec4(normal, 0.0)).xyz;
}
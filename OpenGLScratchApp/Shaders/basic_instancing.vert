#version 410 core                                            

layout(std140) uniform globalMatrixBlock
{
    mat4 projection;
    mat4 view;
}block_matrices;

layout(std140) uniform instancingMatrixBlock
{
    mat4[1000] instanceModels;
    //vec4[1000] instancingColor;

}block_matrix_instancing;


layout (location = 0) in vec3 Pos;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

out vec2 TexCoord;
out vec3 Normal; 
flat out int InstanceID;
                                                   
                                                                            
void main()                                                                  
{         
    gl_Position = block_matrices.projection * block_matrices.view * block_matrix_instancing.instanceModels[gl_InstanceID] * vec4(Pos, 1.0);

    TexCoord = uv;
    Normal = (block_matrix_instancing.instanceModels[gl_InstanceID] * vec4(normal, 0.0)).xyz;
    InstanceID = gl_InstanceID;
}
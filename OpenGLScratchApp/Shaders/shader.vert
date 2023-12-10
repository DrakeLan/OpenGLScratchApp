#version 330                                                              

layout(std140) uniform globalMatrixBlock
{
    mat4 projection;
    mat4 view;
}block_matrices;

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;
                                                                             
out vec4 vCol;
out vec2 TexCoord;
out vec3 Normal; 
out vec3 FragPos;
out vec4 directionalLightSpacePos;                                                             
                                                                           
uniform mat4 model;                                                         
uniform mat4 directonalLightTransform;                                               
                                                                              
                                                                            
void main()                                                                  
{                                                                            
    gl_Position = block_matrices.projection * block_matrices.view * model * vec4(pos, 1.0);	  
    directionalLightSpacePos = directonalLightTransform * model * vec4(pos, 1.0);
                        
    vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);

    TexCoord = uv;

    Normal = mat3(transpose(inverse(model))) * normal; 

    FragPos = (model * vec4(pos, 1.0)).xyz;                        
}
#version 330              

layout(std140) uniform globalMatrixBlock
{
    mat4 projection;
    mat4 view;
}block_matrices;
                                                     
layout (location = 0) in vec3 pos;
layout (location = 2) in vec3 normal;

out vec3 Normal; 
out vec3 wPos;


uniform mat4 model;                                                            
                                                                            
void main()                                                                  
{         
   
    Normal = mat3(transpose(inverse(model))) * normal; 
    wPos = vec3(model * vec4(pos, 1.0));
    gl_Position = block_matrices.projection * block_matrices.view * vec4(wPos, 1.0);
     
}
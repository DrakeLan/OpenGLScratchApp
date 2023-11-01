#version 330                                                                 
                                                     
layout (location = 0) in vec3 pos;
layout (location = 2) in vec3 normal;

out vec3 Normal; 
out vec3 wPos;


uniform mat4 model;                                                      
uniform mat4 projection;
uniform mat4 view;      
                                                                            
void main()                                                                  
{         
   
    Normal = mat3(transpose(inverse(model))) * normal; 
    wPos = vec3(model * vec4(pos, 1.0));
    gl_Position = projection * view * vec4(wPos, 1.0);
     
}
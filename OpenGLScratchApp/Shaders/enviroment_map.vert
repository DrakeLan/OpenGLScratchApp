#version 330                                                                 
                                                     
layout (location = 0) in vec3 Pos;

out vec3 dir;

uniform mat4 PToWTransform;
uniform vec3 viewPosition;  

uniform mat4 model;                                                      
uniform mat4 projection;
uniform mat4 view;      
                                                                            
void main()                                                                  
{         
    gl_Position = vec4(Pos, 1.0);	
     
    dir = (PToWTransform * vec4(Pos, 1.0)).xyz;
}
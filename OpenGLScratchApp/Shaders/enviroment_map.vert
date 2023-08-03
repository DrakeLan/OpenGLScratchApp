#version 330                                                                 
                                                     
layout (location = 0) in vec3 clipPos;

out vec3 dir;

uniform mat3 PToWTransform;
uniform vec3 viewPosition;             
                                                                            
void main()                                                                  
{                                                                            
    gl_Position = vec4(clipPos, 1.0);	  
    
    dir =  (PToWTransform * clipPos) - viewPosition;
    //dir = clipPos;
}
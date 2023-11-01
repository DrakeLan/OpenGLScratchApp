#version 330													

in vec3 Normal;
in vec3 wPos;

out vec4 color; 

uniform vec3 viewPosition;

uniform samplerCube envTexture;


																			
void main()																	
{	
	vec3 I = normalize(wPos - viewPosition);
	vec3 R = reflect(I, normalize(Normal));		    	
	color = texture(envTexture, R);
																    
}
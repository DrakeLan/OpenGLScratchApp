#version 330													

in vec3 dir;

out vec4 color; 


uniform samplerCube envTexture;
uniform sampler2D baseTex;

																			
void main()																	
{	
					    	
	color = texture(envTexture, dir);
	//color = vec4(1.0, 0.0, 0.0, 1.0); 
																	    
}
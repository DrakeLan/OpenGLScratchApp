#version 330													

in vec3 dir;

out vec4 color; 


uniform samplerCube envTexture;
uniform sampler2D baseTex;

																			
void main()																	
{	
					    	
	color = textureLod(envTexture, dir, 0.0);
	//color = vec4(1.0, 0.0, 0.0, 1.0); 
																	    
}
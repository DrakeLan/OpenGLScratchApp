#version 410 core													


in float Height;
in vec2 uv;
in vec3 wnormal;

out vec4 color; 

uniform float debugFlag;
																			
void main()																	
{	

	if(debugFlag > 0.5)
	{
		color = vec4(1.0, 0.0, 0.0, 1.0);
	}else{
		color = vec4(Height, Height, Height, 1.0); 
	}	    	
	
																	    
}
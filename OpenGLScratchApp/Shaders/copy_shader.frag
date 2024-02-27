#version 330													

in vec2 texcoord;

out vec4 color; 

uniform sampler2D sourceTex;
float gamma = 2.2;
																			
void main()																	
{	
					    	
	color = texture(sourceTex, texcoord);
	
    color.rgb = pow(color.rgb, vec3(1.0/gamma));
																	    
}
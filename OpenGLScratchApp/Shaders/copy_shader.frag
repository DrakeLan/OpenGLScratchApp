#version 330													

in vec2 texcoord;

out vec4 color; 

uniform sampler2D sourceTex;
const float gamma = 2.2;
																			
void main()																	
{	
					    	
	vec4 sourceColor = texture(sourceTex, texcoord);

    //vec3 mapped = vec3(1.0) - exp(-sourceColor.rgb * 3.0);
	
    //From linear to gamma
    color.rgb = pow(sourceColor.rgb, vec3(1.0/gamma));
												    
}
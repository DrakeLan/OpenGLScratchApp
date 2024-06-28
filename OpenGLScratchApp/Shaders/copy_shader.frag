#version 330													

in vec2 texcoord;

out vec4 color; 

uniform sampler2D sourceTex;
const float gamma = 1.0;

float PositivePow(float base, float power)
{
    return pow(max(abs(base), float(1.192092896e-07)), power);
}
 
float LinearToSRGB(float c)
{
    return (c <= 0.0031308) ? (c * 12.9232102) : 1.055 * PositivePow(c, 1.0 / 2.4) - 0.055;
}
																			
void main()																	
{	
					    	
	vec4 sourceColor = texture(sourceTex, texcoord);

    //vec3 mapped = vec3(1.0) - exp(-sourceColor.rgb * 3.0);
	
    //From linear to gamma
    //color.rgb = pow(sourceColor.rgb, vec3(1.0/gamma));
    color.rgb = vec3(LinearToSRGB(sourceColor.r), LinearToSRGB(sourceColor.g), LinearToSRGB(sourceColor.b));
    color.a = 1.0;
												    
}
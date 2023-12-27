#version 330													

in vec3 Normal;
in vec3 wPos;

out vec4 color; 

uniform vec3 viewPosition;

uniform samplerCube envTexture;


																			
void main()																	
{	
	float dist = distance(vec3(0.0, 0.0, 0.0), wPos);
	float distortion = sin(dist * 5);
	vec3 I = normalize(wPos - viewPosition);
	vec3 R = reflect(I, normalize(Normal + vec3(distortion)));		    	
	color = texture(envTexture, R);
																    
}
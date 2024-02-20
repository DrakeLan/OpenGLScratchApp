#version 410 core													


in vec2 TexCoord;
in vec3 NormalWS;
in vec3 PositionWS;

out vec4 FragColor; 


struct Light
{
	vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight
{
	Light base;
	vec3 direction;
};


uniform vec3 viewPosition;
uniform float metallic;
uniform float roughness;
uniform float ao;

uniform DirectionalLight directionalLight;

uniform samplerCube irradianceMap;

const float PI = 3.14159265359;

//BRDF F
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}  

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}   

//BRDF D
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

//BRDF G
float GeometrySchlickGGX (float NdotL, float NdotV, float roughness){
    float k = roughness / 2;


    float SmithL = (NdotL)/ (NdotL * (1- k) + k);
    float SmithV = (NdotV)/ (NdotV * (1- k) + k);


	float Gs =  (SmithL * SmithV);
	return Gs;
}

																			
void main()																	
{	
	vec3 albedo = vec3(1.0);

	//Base vectors
	vec3 L = -normalize(directionalLight.direction);
	vec3 N = normalize(NormalWS);
	vec3 V = normalize(viewPosition - PositionWS);
	vec3 H = normalize(V + L);

	float NdotL = max(dot(N, L), 0.0);
	float NdotV = max(dot(N, V), 0.0);

	//Direct Specular
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metallic);
	vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);
	float D = DistributionGGX(N, H, roughness);
	float G = GeometrySchlickGGX(NdotL, NdotV, roughness);
	//float G = GeometrySmith(N, V, L, roughness);  

	vec3 directSpec = (D * F * G) / (4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0)  + 0.0001);

	//Direct Diffuse
	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;

	kD *= 1.0 - metallic;

	//Direct Final Result
	vec3 directLight = (kD * albedo/PI + directSpec) * directionalLight.base.color * NdotL;

	//Indrect Diffuse
	vec3 idkS = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
	vec3 idkD = 1.0 - idkS;
	vec3 irradiance = texture(irradianceMap, N).rgb;
	vec3 diffuse    = irradiance * albedo;
	vec3 ambient    = (idkD * diffuse) * ao; 

	FragColor = vec4(directLight + ambient, 1.0);

	FragColor.xyz = PositionWS;

																	    
}
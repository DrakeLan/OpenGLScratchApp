#version 410 core													


in vec2 TexCoord;
in vec3 NormalWS;
in vec3 TangentWS;
in vec3 BiTangengt;
in vec3 PositionWS;
in mat3 TBN;

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

layout(std140) uniform cameraDataBlock
{
    vec3 position;
    vec3 direction;
}block_camera;

layout(std140) uniform lightDataBlock
{
    vec3 color;
	vec3 direction;
	float ambientIntensity;
	float diffuseIntensity;
}block_light;

uniform float ao;

uniform sampler2D diffuseMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D normalMap;

uniform samplerCube irradianceMap;
uniform samplerCube importanceSampleMap;
uniform sampler2D BRDF_LUT;

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
	//Surface parameters
	vec3 albedo = vec3(1.0);
	albedo = texture(diffuseMap, TexCoord).xyz;
	float metallic = texture(metallicMap, TexCoord).x;
	float roughness = texture(roughnessMap, TexCoord).x;
	float safeRoughness = max(0.045, roughness);

	//Normal Map
	vec3 normal = texture(normalMap, TexCoord).xyz;
	//normal.y = 1.0 - normal.y;
	normal = normal * 2.0 - 1.0;
	normal = normalize(TBN * normal);
	
	//Base vectors
	vec3 L = -normalize(block_light.direction);
	vec3 N = normalize(normal);
	vec3 V = normalize(block_camera.position - PositionWS);
	vec3 H = normalize(V + L);

	float NdotL = max(dot(N, L), 0.0);
	float NdotV = max(dot(N, V), 0.0);

	//Direct Specular
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metallic);
	vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);
	float D = DistributionGGX(N, H, safeRoughness);
	float G = GeometrySchlickGGX(NdotL, NdotV, safeRoughness);
	//float G = GeometrySmith(N, V, L, roughness);  

	vec3 directSpec = (D * F * G) / (4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0)  + 0.0001);

	//Direct Diffuse
	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;

	kD *= 1.0 - metallic;

	//Direct Final Result
	vec3 directLight = (kD * albedo/PI + directSpec) * block_light.color * NdotL;

	//Indrect Specular
	vec3 idkS = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, safeRoughness);

	vec3 R = reflect(-V, N);
	const float MAX_REFLECTION_LOD = 4.0;
	vec3 importanceReflection = textureLod(importanceSampleMap, R, safeRoughness * MAX_REFLECTION_LOD).rgb;
	vec2 envBRDF = texture(BRDF_LUT, vec2(max(dot(N, V), 0.0), safeRoughness)).rg;
	vec3 specular = importanceReflection * ( idkS * envBRDF.x + envBRDF.y);

	//Indrect Diffuse
	vec3 idkD = 1.0 - idkS;

	vec3 irradiance = texture(irradianceMap, N).rgb;
	vec3 diffuse    = irradiance * albedo;


	//Indirect Final Result
	vec3 ambient  = (idkD * diffuse + specular) * ao; 

	FragColor = vec4(directLight + ambient, 1.0);	

	//FragColor.xyz *= 0.0;
	//FragColor.z = N.z;
}
#version 330													
																	
in vec4 vCol;	
in vec2 TexCoord;
in vec3 Normal;	
in vec3 FragPos;	
in vec4 directionalLightSpacePos;    	     											

out vec4 color; 

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

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

struct PointLight
{
	Light base;
	vec3 position;
	float constant;
	float linear;
	float exponent;
};

struct SpotLight
{
	PointLight base;
	vec3 direction;
	float edge;
};

struct OmniShadowMap
{
	samplerCube shadowMap;
	float farPlane;
};

struct Material
{
	float specularIntensity;
	float shininess;
};

uniform int pointLightCount;
uniform int spotLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform sampler2D theTexture;
uniform sampler2D directionalShadowMap;
uniform OmniShadowMap omniShadowMaps[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];

uniform Material material;

uniform vec3 viewPosition;

vec3 sampleOffsetDirection[20] = vec3[]
(
	vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
	vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
	vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
	vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
	vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1)
);

float CalcDirectionalShadowFactor(DirectionalLight light)
{
	vec3 projCoords = directionalLightSpacePos.xyz / directionalLightSpacePos.w;
	projCoords = (projCoords * 0.5) + 0.5;

	float closestDepth = texture(directionalShadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;

	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(light.direction);

	float bias = max(0.05 * (1 - dot(normal, lightDir)), 0.005);

	
	float shadow = 0;

	vec2 texelSize = 1.0 / textureSize(directionalShadowMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(directionalShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth? 1.0 : 0.0;
		}
	}
	
	shadow /= 9.0;


	if(projCoords.z > 1.0)
	{
		shadow = 0.0;
	}

	return shadow;
}

float CalcOmniShadowFactor(PointLight light, int shadowIndex)
{
	vec3 fragToLight = FragPos - light.position;
	float currentDepth = length(fragToLight);

	float viewDistance = length(viewPosition - FragPos);

	float shadow = 0.0;
	float bias = 0.05;
	int samples = 20;
	float diskRidus = (1.0 + (viewDistance / omniShadowMaps[shadowIndex].farPlane)) / 25.0;

	for(int i = 0; i < samples; i++)
	{
		float closestDepth = texture(omniShadowMaps[shadowIndex].shadowMap, fragToLight + sampleOffsetDirection[i] * diskRidus).r;
		closestDepth *= omniShadowMaps[shadowIndex].farPlane;

		if(currentDepth - bias > closestDepth)
		{
			shadow += 1.0;
		}
	}


	shadow /= samples;
	
	return shadow;
}

vec4 CalcLightByDirection(Light light, vec3 direction, float shadowFactor)
{
	vec4 ambientColor = vec4(light.color, 1.0) * light.ambientIntensity;			

	float diffuseFactor = max(dot(normalize(direction), normalize(Normal)), 0.0);
	vec4 diffuseColor = vec4(light.color, 1.0) * light.diffuseIntensity * diffuseFactor;

	vec4 speularColor = vec4(0, 0, 0, 0);
	if(diffuseFactor > 0.0)
	{
		vec3 viewDir = normalize(viewPosition - FragPos);
		//vec3 halfVec = normalize(viewDir + direction);
		//float specularFactor = dot(Normal, halfVec);
		vec3 reflectedVec = normalize(reflect(direction, normalize(Normal)));
		float specularFactor = dot(viewDir, reflectedVec);

		if(specularFactor > 0.0)
		{
			specularFactor = pow(specularFactor, material.shininess);
			speularColor = vec4(light.color, 1.0) * material.specularIntensity * specularFactor;
		}
	}
	return (ambientColor + (1.0 - shadowFactor) * (diffuseColor + speularColor));	
}

vec4 calcDirectionLight()
{
	float shadowFactor = CalcDirectionalShadowFactor(directionalLight);
	return CalcLightByDirection(directionalLight.base, directionalLight.direction, shadowFactor);
}

vec4 CalcPointLight(PointLight pLight, int shadowIndex)
{
		vec3 direction = FragPos - pLight.position;
		float distance = length(direction);
		direction = normalize(direction);

		float shadowFactor = CalcOmniShadowFactor(pLight, shadowIndex);

		vec4 lColor = CalcLightByDirection(pLight.base, direction, shadowFactor);
		float attenuation = pLight.exponent * distance * distance + pLight.linear * distance + pLight.constant;

		return (lColor / attenuation);
}

vec4 CaclcSpotLight(SpotLight sLight, int shadowIndex)
{
	vec3 rayDriection = normalize(FragPos - sLight.base.position);
	float slFactor = dot(rayDriection, sLight.direction);

	if(slFactor > sLight.edge)
	{
		vec4 color = CalcPointLight(sLight.base, shadowIndex);

		return color * (1.0f - (1.0f - slFactor) * (1.0f/(1.0f - sLight.edge)));
	} else {
		return vec4(0, 0, 0, 0);
	}
}

vec4 CalcPointLights()
{
	vec4 totalColor = vec4(0, 0, 0, 0);
	for(int i = 0; i < pointLightCount; i++)
	{
		totalColor += CalcPointLight(pointLights[i], i);
	}

	return totalColor;
}

vec4 CalSpotLights()
{
	vec4 totalColor = vec4(0, 0, 0, 0);
	for(int i = 0; i < spotLightCount; i++)
	{
		totalColor += CaclcSpotLight(spotLights[i], pointLightCount + i);
	}

	return totalColor;
}
																			
void main()																	
{	
	vec4 finalColor = calcDirectionLight();
	finalColor += CalcPointLights();	
	finalColor += CalSpotLights();					    	
	color = texture(theTexture, TexCoord) * finalColor;
   
}
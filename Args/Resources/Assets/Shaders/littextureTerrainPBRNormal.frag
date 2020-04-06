#define MAX_LIGHT_COUNT
#define PI 
#define HALF_PI

this is magic code >=D

#version 440 // for glsl version (12 is for older versions , say opengl 2.1

uniform sampler2D splatMap;
uniform sampler2DArray albedoMap;
uniform sampler2DArray normalMap;
uniform sampler2DArray metalMap;
uniform sampler2DArray roughnessMap;
uniform sampler2DArray aoMap;
uniform sampler2DArray heightMap;
uniform sampler2D terrainMap;
uniform sampler2D terrainNormalMap;
uniform float terrainScale;
uniform vec2 terrainUVScale;

uniform int lightCount;
uniform vec3 cameraPosition;
uniform float heightScale;

struct Light
{
	int type;				// 4	0
	float attenuation;		// 4	4
	float intensity;		// 4	8
	float angle;			// 4	12
	vec3 direction;			// 12	16
	float falloff;			// 4	28
	vec4 position;			// 16	32
	vec4 colour;			// 16	48
};

layout (std140) uniform LightsBlock
{
	Light lights[MAX_LIGHT_COUNT];
};

in vec2 texCoord;
in vec3 surfacePosition;
in vec3 surfaceNormal;
in vec3 surfacetangent;
in mat3 tbnMatrix;

out vec4 fragment_color;

vec4 TriplanarMap(sampler2DArray map, vec2 uvOffset, vec3 position, vec3 normal, int layer)
{
	vec3 blendingWeights = pow(abs(normal.xyz), vec3(2));
	blendingWeights /= dot(blendingWeights, vec3(1,1,1));
	vec4 xAxis = texture(map, vec3(position.yz + uvOffset, layer));
	vec4 yAxis = texture(map, vec3(position.xz + uvOffset, layer));
	vec4 zAxis = texture(map, vec3(position.xy + uvOffset, layer));

	vec4 endColor = (xAxis * blendingWeights.x) + (yAxis * blendingWeights.y) + (zAxis * blendingWeights.z);
	return endColor;
}

vec4 MixLayers(sampler2DArray map, vec2 uvOffset, vec3 position, vec3 normal, vec4 layers)
{
	vec4 layer0 = TriplanarMap(map, uvOffset, position, normal, 0);
	vec4 layer1 = TriplanarMap(map, uvOffset, position, normal, 1);
	vec4 layer2 = TriplanarMap(map, uvOffset, position, normal, 2);
	vec4 layer3 = TriplanarMap(map, uvOffset, position, normal, 3);

	vec4 endColor = (layer0 * layers.r) + (layer1 * layers.g) + (layer2 * layers.b) + (layer3 * layers.a);
	endColor /= max(layers.r + layers.g + layers.b + layers.a, 0.0001);
	return endColor;
}

vec2 ParallaxMap(float height, float scale, vec3 viewDir)
{
	float offset = (height * 2.0 - 1.0) * scale * -1;
	vec2 parallexOffset = (viewDir.xy / viewDir.z) * offset;
    return -parallexOffset;
}

vec3 CalculateNormal(vec3 normal, mat3 tbn)
{
	return normalize(tbn * normalize(normal * 2.0 - 1.0));
}

float length2(vec3 v){
        return v.x*v.x + v.y*v.y + v.z*v.z;               
}

float CalculateAttenuation(vec3 fragmentPosition, vec3 lightPosition, float attenuationRadius, float intensity)
{
	float sqrlightDistance = length2(lightPosition - fragmentPosition);
	float attenuation = pow(max(1.0 - (sqrlightDistance / (attenuationRadius * attenuationRadius)), 0.0), 2);
	return attenuation * intensity;
}

//// PBR FUNCTIONS ////

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}  

float DistributionGGX(vec3 N, vec3 H, float R)
{
    float a      = R * R;
    float a2     = a * a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);	
	denom = PI * denom * denom;

    return num / denom;
}

float GeometrySchlickGGX(float normalDotView, float R)
{
    float r = (R + 1.0);
    float k = (r * r) / 8.0;

    float num   = normalDotView;
    float denom = normalDotView * (1.0 - k) + k;
	
    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float R)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, R);
    float ggx1  = GeometrySchlickGGX(NdotL, R);
	
    return ggx1 * ggx2;
}


vec3 viewDirection;
vec2 parallaxOffset;
vec3 albedo;
vec3 fragmentNormal;
float roughness;
float metallic;
float ao;
vec3 F0;
float ambientIntensity;

vec3 ApplyLight(Light light)
{
	vec3 lightDirection = vec3(0.0);
	float lightIntensity = 0.0;
	vec3 normal = vec3(0.0);
	vec3 lightColor = vec3(1.0);
	switch(light.type)
	{
		case 0:
			lightDirection = normalize(light.position.xyz - surfacePosition);
			normal = fragmentNormal;
			lightIntensity = light.intensity;
			lightColor = light.colour.rgb;
			break;
		case 1:
			lightDirection = light.direction;	
			normal = fragmentNormal;
			float dayTimeScalar = max(0.0, dot(lightDirection, vec3(0, 1, 0)));
			lightIntensity = dayTimeScalar * light.intensity;
			lightColor = vec3(light.colour.r, light.colour.g * dayTimeScalar,  light.colour.b * pow(dayTimeScalar, 1.1));
			break;
		case 2:
			lightDirection = normalize(light.position.xyz - surfacePosition);
			normal = fragmentNormal;
			lightIntensity = pow(max((dot(normalize(light.direction), lightDirection) - cos(light.angle*0.5)) / (1.0 - cos(light.angle*0.5)), 0.0), light.falloff) * light.intensity;
			lightColor = light.colour.rgb;
			break;
	}

	ambientIntensity += (light.intensity*1.5) - (lightIntensity*0.5);

	float attenuation = CalculateAttenuation(surfacePosition, light.position.xyz, light.attenuation, lightIntensity);

	if(attenuation <= 0)
		return vec3(0);

	vec3 radiance = lightColor * attenuation;
	vec3 halfwayVector = normalize(lightDirection + viewDirection);

	// cook-torrance brdf
	vec3 F = fresnelSchlick(max(dot(halfwayVector, viewDirection), 0.0), F0);
	float NDF = DistributionGGX(normal, halfwayVector, roughness);
	float G = GeometrySmith(normal, viewDirection, lightDirection, roughness);

	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - metallic;

	vec3 numerator = NDF * G * F;
	float denominator = 4.0 * max(dot(normal, viewDirection), 0.0) * max(dot(normal, lightDirection), 0.0);
	vec3 specular = numerator / max(denominator, 0.001);

	float normalDotLightDir = max(dot(normal, lightDirection), 0.0);  	
	return (kD * albedo.rgb / PI + specular) * radiance * normalDotLightDir;
}

// MAIN
void main( void )
{
	vec4 weights = texture(splatMap, texCoord);

	vec3 terrainNormal = texture(terrainNormalMap, texCoord).xyz;
	fragmentNormal = normalize(tbnMatrix * normalize(terrainNormal * 2.0 - 1.0));

	vec3 worldTangent = normalize(surfacetangent);

	worldTangent = normalize(worldTangent - dot(worldTangent, fragmentNormal) * fragmentNormal);
	vec3 worldBitangent = normalize(cross(fragmentNormal, worldTangent));
	mat3 fragmentTBNMatrix = mat3(worldTangent, worldBitangent, fragmentNormal);

	viewDirection = normalize(cameraPosition - surfacePosition);

	parallaxOffset = ParallaxMap(MixLayers(heightMap, vec2(0.0), surfacePosition, surfaceNormal, weights).r, heightScale * 0.015, fragmentTBNMatrix * viewDirection);

 	albedo = pow(MixLayers(albedoMap, parallaxOffset, surfacePosition, surfaceNormal, weights).rgb, vec3(2.2));
 	fragmentNormal = CalculateNormal(MixLayers(normalMap, parallaxOffset, surfacePosition, surfaceNormal, weights).rgb, fragmentTBNMatrix);
 	roughness = MixLayers(roughnessMap, parallaxOffset, surfacePosition, surfaceNormal, weights).r;
 	metallic = MixLayers(metalMap, parallaxOffset, surfacePosition, surfaceNormal, weights).r;
 	ao = MixLayers(aoMap, parallaxOffset, surfacePosition, surfaceNormal, weights).r;
 	F0 = mix(vec3(0.04), albedo.rgb, metallic.xxx);

	vec3 lighting = vec3(0.0);

	for(int i = 0; i < lightCount && i < MAX_LIGHT_COUNT; i++)
	{
		lighting += ApplyLight(lights[i]);
	}

	vec3 ambient = pow(ambientIntensity, 1.1) * 0.0001 * ao * albedo;

	vec3 surfaceColor = ambient + lighting;

 	surfaceColor.rgb = surfaceColor.rgb / (surfaceColor.rgb + vec3(1.0));
    surfaceColor.rgb = pow(surfaceColor.rgb, vec3(1.0/2.2)); 
	vec3 temp = surfacePosition.xyz;
	fragment_color = vec4(surfaceColor, MixLayers(albedoMap, parallaxOffset, surfacePosition, surfaceNormal, weights).a);
}
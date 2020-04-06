#define MAX_LIGHT_COUNT
#define PI
#define HALF_PI

this is magic code >=D

#version 450 // for glsl version (12 is for older versions , say opengl 2.1

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metalMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;
uniform sampler2D heightMap;
uniform sampler2D emissiveMap;

uniform int lightCount;
uniform vec3 cameraPosition;
uniform float heightScale;

struct Light
{
	int type;			// 4	0
	float attenuation;	// 4	4
	float intensity;	// 4	8
	float meta;			// 4	12
	vec3 direction;		// 12	16
	float falloff;		// 4	28
	vec3 position;		// 12	32
	float angle;		// 4	44
	vec3 colour;		// 12	48
	float meta2;		// 4	60
};

layout (std140) uniform LightsBlock
{
	Light lights[MAX_LIGHT_COUNT];
};

in vec2 texCoord;
in vec3 surfacePosition;
in vec3 surfaceNormal;
in mat3 tbnMatrix;

out vec4 fragment_color;

vec2 ParallaxMap(sampler2D map, float scale, vec2 uv, vec3 viewDir)
{
	float offset = (texture(map, uv).r * 2.0 - 1.0) * scale * -1;
	vec2 parallexOffset = (viewDir.xy / viewDir.z) * offset;
    return uv - parallexOffset;
}

vec3 CalculateNormal(sampler2D map, vec2 uv, mat3 tbn)
{
	return normalize(tbn * normalize(texture(map, uv).xyz * 2.0 - 1.0));
}

float length2(vec3 v)
{
    return v.x*v.x + v.y*v.y + v.z*v.z;
}

float CalculateAttenuation(vec3 fragmentPosition, vec3 lightPosition, float attenuationRadius, float intensity)
{
	float sqrlightDistance = length2(lightPosition - fragmentPosition);
	float attenuation = pow(max(1.0 - (sqrlightDistance / (attenuationRadius * attenuationRadius)), 0.0), 2);
	return attenuation * intensity;
}

//// PBR FUNCTIONS ////

// Fresnel reflection function
// Simple interpolation between 1 (ultimate metal fresnel reflection) and
// the fresnel approximation function: (1-cos(θᵢ))⁵
//
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

// GX normal distribution
// Calculates the partition of the microfacet normals that align with the halfway vector to reflect
// the light into the view direction
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
vec3 fragmentNormal;

vec2 textureCoords;

// material properties
vec3 albedo;
float roughness;
float metallic;
vec3 emissive;
float ao;

// Fresnel reflection color at an angle of incidence of 0°
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
		case 0: // point
			lightDirection = normalize(light.position - surfacePosition);
			normal = fragmentNormal;
			lightIntensity = light.intensity;
			lightColor = light.colour;
			break;
		case 1: // directional
			lightDirection = light.direction;
			normal = fragmentNormal;
			//float dayTimeScalar = max(0.0, dot(lightDirection, vec3(0, 1, 0)));
			lightIntensity = light.intensity;
			lightColor = light.colour;
			break;
		case 2: // spot
			lightDirection = normalize(light.position - surfacePosition);
			normal = fragmentNormal;
			lightIntensity = pow(max((dot(normalize(light.direction), lightDirection) - cos(light.angle*0.5)) / (1.0 - cos(light.angle*0.5)), 0.0), light.falloff) * light.intensity;
			lightColor = light.colour;
			break;
	}

	ambientIntensity += (light.intensity*1.5) - (lightIntensity*0.5);

	float attenuation = CalculateAttenuation(surfacePosition, light.position, light.attenuation, lightIntensity);

	if(attenuation <= 0)
		return vec3(0);

	vec3 radiance = lightColor * attenuation;

	// Microfacet normal that will reflect the incoming light into the viewing direction.
	// Technically only applies if the nanogeometry is perfectly smooth, but due to the inherent
	// inaccuracy of working with fragments as the smallest size of measurement we can ignore
	// nanogeometry for now.
	vec3 halfwayVector = normalize(lightDirection + viewDirection);

	// cook-torrance brdf
	vec3 fresnelReflection = fresnelSchlick(max(dot(halfwayVector, viewDirection), 0.0), F0);
	float normalDistribution = DistributionGGX(normal, halfwayVector, roughness);
	float geometryAttenuation = GeometrySmith(normal, viewDirection, lightDirection, roughness);

	// Start magic
	vec3 kS = fresnelReflection;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - metallic;

	vec3 numerator = normalDistribution * geometryAttenuation * fresnelReflection;
	float denominator = 4.0 * max(dot(normal, viewDirection), 0.0) * max(dot(normal, lightDirection), 0.0);
	vec3 specular = numerator / max(denominator, 0.001);

	float normalDotLightDir = max(dot(normal, lightDirection), 0.0);
	return (kD * albedo / PI + specular) * radiance * normalDotLightDir;
	// End magic
}


// MAIN
void main( void )
{
	viewDirection = normalize(cameraPosition - surfacePosition);
	textureCoords = ParallaxMap(heightMap, heightScale * 0.015, texCoord, transpose(tbnMatrix) * viewDirection);

 	albedo = pow(texture(albedoMap, textureCoords).rgb, vec3(2.2));
 	fragmentNormal = CalculateNormal(normalMap, textureCoords, tbnMatrix);
 	roughness = texture(roughnessMap, textureCoords).r;
 	metallic = texture(metalMap, textureCoords).r;
	emissive = texture(emissiveMap, textureCoords).rgb;
 	ao = texture(aoMap, textureCoords).r;

	// Assumption that dielectrics all have a fresnell reflectance at 0° of about 4%
	// of course not accurate for some dielectrics like water (2%),
	// crystals (5-8%) and diamond like materials (10-20%).
	// Semi-conductors also have other values (20-40%)
 	F0 = mix(vec3(0.04), albedo, metallic.xxx);

	vec3 lighting = vec3(0.0);

	for(int i = 0; i < lightCount && i < MAX_LIGHT_COUNT; i++)
		lighting += ApplyLight(lights[i]);

	vec3 ambient = pow(ambientIntensity, 1.1) * 0.0001 * ao * albedo;

	vec3 surfaceColor = ambient + lighting + emissive;
	surfaceColor.rgb = surfaceColor.rgb / (surfaceColor.rgb + vec3(1.0));
    surfaceColor.rgb = pow(surfaceColor.rgb, vec3(1.0/2.2));

	fragment_color = vec4(surfaceColor, 1);
}

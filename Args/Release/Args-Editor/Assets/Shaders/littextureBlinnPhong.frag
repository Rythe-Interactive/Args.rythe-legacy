#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metalMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;
uniform sampler2D heightMap;

uniform int lightCount;
uniform vec3 cameraPosition;
uniform float heightScale;

struct Light
{
	int type;				// 4	0
	float attenuation;		// 4	4
	float intensity;		// 4	8
	float meta;				// 4	12
	vec4 direction;			// 16	16
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
in mat3 tbnMatrix;

out vec4 fragment_color;

vec2 ParallaxMap(sampler2D map, float scale, vec2 uv, vec3 viewDir)
{
	float height = texture(map, uv).r;    
	viewDir = transpose(tbnMatrix) * viewDir;
    vec2 parallexOffset = viewDir.xy / viewDir.z * (height * scale);
    return uv - parallexOffset;
}

vec3 CalculateNormal(sampler2D map, vec2 uv, mat3 tbn)
{
	return normalize(tbn * normalize(texture(map, uv).xyz * 2.0 - 1.0));
}

float CalculateAttenuation(vec3 fragmentPosition, vec3 lightPosition, float attenuationRadius)
{
	float lightDistance = length(lightPosition - fragmentPosition);
	float attenuation = clamp(1.0 - ((lightDistance * lightDistance) / (attenuationRadius * attenuationRadius)), 0.0, 1.0);
	attenuation *= attenuation;
	return attenuation;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}  

vec3 viewDirection;
vec2 textureCoords;
vec3 albedo;
vec3 fragmentNormal;
float roughness;
float metallic;
float ao;
vec3 F0;

vec3 ApplyLight(Light light)
{	
	float attenuation = CalculateAttenuation(surfacePosition, light.position.xyz, light.attenuation);

	if(attenuation <= 0)
		return vec3(0);
	
	vec3 lightDirection = vec3(0.0);
	float lightIntensity = 0.0;
	vec3 normal = vec3(0.0);

	switch(light.type)
	{
		case 0:
			lightDirection = normalize(light.position.xyz - surfacePosition);
			normal = fragmentNormal;
			lightIntensity = light.intensity;
			break;
		case 1:
			lightDirection = light.direction.xyz;	
			normal = fragmentNormal;
			lightIntensity = light.intensity;
			break;
		case 2:
			lightDirection = normalize(light.position.xyz - surfacePosition);
			normal = normalize(light.direction.xyz);
			lightIntensity = clamp((dot(normal, lightDirection) - cos(light.meta)) / (1.0 - cos(light.meta)), 0.0, 1.0) * light.intensity;
			break;
	}

	vec3 halfwayVector = normalize(lightDirection + viewDirection);
	vec3 F = fresnelSchlick(max(dot(halfwayVector, viewDirection), 0.0), F0);

	float specularIntensity = max(0.0, dot(normal, halfwayVector));
	vec3 diffuseIntensity = (vec3(1.0) - F) * (1.0 - metallic);
	specularIntensity = pow(specularIntensity, (1.0-roughness) * 400.0)*(PI*2) + pow(specularIntensity, (1.0-roughness) * 100.0)*PI + pow(specularIntensity, (1.0-roughness) * 25.0) * (PI*0.5);

	vec3 diffuseColor = diffuseIntensity * albedo / PI;
	vec3 specularColor = specularIntensity * F * (0.1 + metallic);
	
	vec3 radiance = light.colour.rgb * attenuation * lightIntensity;
	float normalDotLightDir = max(dot(normal, lightDirection), 0.0);  	
	return (diffuseColor + specularColor) * radiance * normalDotLightDir;
}

void main( void )
{
	viewDirection = normalize(cameraPosition - surfacePosition);
	textureCoords = ParallaxMap(heightMap, heightScale, texCoord, viewDirection);

	// if(textureCoords.x > 1.0 || textureCoords.y > 1.0 || textureCoords.x < 0.0 || textureCoords.y < 0.0)
    // 	discard;

 	albedo = pow(texture(albedoMap, textureCoords).rgb, vec3(2.2));
 	fragmentNormal = CalculateNormal(normalMap, textureCoords, tbnMatrix);
 	roughness = texture(roughnessMap, textureCoords).r;
 	metallic = texture(metalMap, textureCoords).r;
 	ao = texture(aoMap, textureCoords).r;
 	F0 = mix(vec3(0.04), albedo, vec3(metallic));

	vec3 ambient = (1.0/MAX_LIGHT_COUNT) * lightCount * 0.06 * albedo * ao;
	vec3 lighting = vec3(0.0);

	for(int i = 0; i < lightCount && i < MAX_LIGHT_COUNT; i++)
	{
		lighting += ApplyLight(lights[i]);
	}

	vec3 surfaceColor = ambient + lighting;

 	surfaceColor.rgb = surfaceColor.rgb / (surfaceColor.rgb + vec3(1.0));
    surfaceColor.rgb = pow(surfaceColor.rgb, vec3(1.0/2.2)); 

	fragment_color = vec4(surfaceColor, texture(albedoMap, textureCoords).a);
}

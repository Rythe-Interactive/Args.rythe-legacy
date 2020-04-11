//DIFFUSE TEXTURE VERTEX SHADER
#version 440 // for glsl version (12 is for older versions , say opengl 2.1

uniform	mat4 viewProjectionMatrix;
uniform sampler2D terrainMap;
uniform sampler2D terrainNormalMap;
uniform float terrainScale;

in mat4 modelMatrix;
in vec3 vertex;
in vec3 normal;
in vec2 uv;
in vec3 tangent;

out vec2 texCoord; //make sure the texture coord is interpolated
out vec3 surfacePosition;
out vec3 surfaceNormal;
out vec3 surfacetangent;
out mat3 tbnMatrix;


void main( void )
{
	float heightmapHeight = texture(terrainMap, uv).r * terrainScale;
    gl_Position = viewProjectionMatrix * modelMatrix * vec4(vertex.x, vertex.y + heightmapHeight,vertex.z, 1.0);
	texCoord = uv;
	surfacePosition = (modelMatrix * vec4(vertex.x, vertex.y + heightmapHeight,vertex.z, 1.0)).xyz;
	surfacetangent = (modelMatrix * vec4(tangent, 0.0)).xyz;

	surfaceNormal = normalize((modelMatrix * vec4(normal, 0.0)).xyz);
	vec3 worldTangent = normalize((modelMatrix * vec4(tangent,   0.0)).xyz);

	worldTangent = normalize(worldTangent - dot(worldTangent, surfaceNormal) * surfaceNormal);
	vec3 worldBitangent = normalize(cross(surfaceNormal, worldTangent));
	tbnMatrix = mat3(worldTangent, worldBitangent, surfaceNormal);

	vec3 terrainNormal = texture(terrainNormalMap, uv).xyz;
	surfaceNormal = normalize((tbnMatrix * normalize(terrainNormal * 2.0 - 1.0)) * vec3(1.0, 0.5, 1.0));


}

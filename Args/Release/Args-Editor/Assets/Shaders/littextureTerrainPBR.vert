//DIFFUSE TEXTURE VERTEX SHADER
#version 440 // for glsl version (12 is for older versions , say opengl 2.1

uniform	mat4 viewProjectionMatrix;
uniform sampler2D terrainMap;
uniform float terrainScale;
uniform vec2 terrainUVScale;

in mat4 modelMatrix;
in vec3 vertex;
in vec3 normal;
in vec2 uv;
in vec3 tangent;

out vec2 texCoord; //make sure the texture coord is interpolated
out vec3 surfacePosition;
out vec3 surfaceNormal;
out mat3 tbnMatrix;


void main( void )
{
	float heightmapHeight = texture(terrainMap, uv).r * terrainScale;
    gl_Position = viewProjectionMatrix * modelMatrix * vec4(vertex.x, vertex.y + heightmapHeight,vertex.z, 1.0);
	texCoord = uv;
	surfacePosition = (modelMatrix * vec4(vertex.x, vertex.y + heightmapHeight,vertex.z, 1.0)).xyz;
	vec3 worldTangent = (modelMatrix * vec4(tangent, 0.0)).xyz;

	vec2 vertexUVScale = terrainUVScale * ((vec2(1.0)/terrainUVScale) / vec2(65));
	vec3 right = vec3(vertexUVScale.x, texture(terrainMap, uv + vec2(vertexUVScale.x, 0)).r * terrainScale, 0);
	vec3 left = vec3(-vertexUVScale.x,  texture(terrainMap, uv + vec2(-vertexUVScale.x, 0)).r * terrainScale, 0);
	vec3 down = vec3(0,  texture(terrainMap, uv + vec2(0, vertexUVScale.y)).r * terrainScale, -vertexUVScale.y);
	vec3 up =  vec3(0, texture(terrainMap, uv + vec2(0, -vertexUVScale.y)).r * terrainScale, vertexUVScale.y);

	vec3 lefttoright = right - left;
	vec3 uptodown = down - up;

	vec3 heightMapNormal = normalize(cross(lefttoright, uptodown));
	surfaceNormal = normalize((modelMatrix * vec4(heightMapNormal, 0.0)).xyz);

	worldTangent = normalize(worldTangent - dot(worldTangent, surfaceNormal) * surfaceNormal);
	vec3 worldBitangent = normalize(cross(surfaceNormal, worldTangent));
	tbnMatrix = mat3(worldTangent, worldBitangent, surfaceNormal);
}

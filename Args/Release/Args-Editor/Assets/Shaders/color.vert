//DIFFUSE COLOR VERTEX SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

in mat4 modelMatrix;
uniform	mat4 viewProjectionMatrix;

in vec3 vertex;
in vec3 normal;
in vec2 uv;
in vec3 tangent;

void main( void ){
	gl_Position = viewProjectionMatrix * modelMatrix * vec4(vertex, 1.f);
}

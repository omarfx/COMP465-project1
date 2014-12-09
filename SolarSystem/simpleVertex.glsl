/* 
SimpleVertex.glsl

Vertex shader with position, color, normal and ModelViewProject
input and color output.

Mike Barnes / Mike Landers
8/17/2013
*/

# version 330 core

in vec4 vPosition;
in vec4 vColor;
in vec3 vNormal;

out vec3 Position;
out vec3 Normal;
out vec4 color;

uniform mat3 NormalMatrix; // not bound in SolarMain yet
uniform mat4 ModelView; //bound in display in SolarMain
uniform mat4 ModelViewProjection;  // = projection * view * model

void main() {
	gl_Position = ModelViewProjection * vPosition;
	Position = (ModelView * vPosition).xyz;
	Normal = normalize(NormalMatrix * vNormal);
	color = vColor;
}
/*
 pyramidVertex.glsl

 Mike Barnes
 8/1/2014
 */

# version 330 core

// layout(location = 0) in vec4 vPosition;
// layout(location = 1) in vec4 vColor;

in vec4 vPosition;
in vec4 vColor;

uniform mat4 ModelViewProject;
out vec4 color;

void main() {
  color = vColor;
  gl_Position = ModelViewProject * vPosition;
  }
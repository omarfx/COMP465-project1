/*
textureModelVertex.glsl

Vertex shader for colored and 
textured surfaces.

Mike Barnes
12/15/2013
*/

# version 330 core
        
in vec4 vPosition, vColor;
in vec2 vTexCoord;
in vec3 vNormal;
out vec2 vs_texCoord;      
out vec3 vs_normal, vs_worldPos;
out vec4 vs_color;

uniform bool IsTexture;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

void main(void) {
  vec4 position = MVP * vPosition;
  gl_Position = position;
  vs_worldPos = position.xyz;
  vs_normal = NormalMatrix * vNormal; 
  if (IsTexture)  {  // texture is used, not color
      vs_texCoord = vTexCoord;
      vs_color = vec4(0,0,0,0)
	  ; }
    else { // color is used, not texture
      vs_texCoord =vec2(0, 0);
      vs_color = vColor; }
  }
 
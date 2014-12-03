/*
textureModelFragment.glsl

Fragment shader for colored and 
textured surfaces.
There is a single "headlamp" light source directed into the scene.

Mike Barnes
12/15/2013
*/

# version 330 core
        
in vec2 vs_texCoord;
in vec4 vs_color;
in vec3 vs_normal, vs_worldPos;
out vec4 color;

uniform bool IsTexture;
uniform sampler2D Texture;
uniform vec3 light_position = vec3(50.0f, 50.0f, 100.0f);

void main(void) {
   float ambient = 0.2f;   
   vec3 light_direction = normalize(light_position - vs_worldPos);
   vec3 normal = normalize(vs_normal);
   float diffuse = max(0.0, dot(normal, light_direction));
   if (IsTexture)   // use texture on surface
        color = texture(Texture, vs_texCoord);
      else         // use color for surface
        color = (ambient + diffuse) * vs_color; 
   }

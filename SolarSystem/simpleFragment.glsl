/* 
SimpleFragment.glsl

Fragment shader with color input and output.

Mike Barnes
8/17/2013
*/

# version 330 core

in vec4 color;
in vec3 Position;
in vec3 Normal;

uniform vec3 HeadLightPosition;
uniform vec3 HeadLightIntensity;
uniform vec3 PointLightPosition;
uniform vec3 PointLightIntensity;

uniform bool HeadLightOn;
uniform bool PointLightOn;
uniform bool DebugOn;
uniform bool IsTheSun;

vec3 ambientColor = vec3(1.0, 0.0, 0.0); // debug ambient red
vec3 diffuseColor = vec3(0.0, 1.0, 0.0); // debug diffuse green

out vec4 fragColor;

vec3 vLight(vec3 LightPosition, vec3 LightIntensity, bool directional){
	float ambient = 0.2f;
	float diffuse = 0.0f;
	vec3 n, s;

	if(directional)
		s = normalize(LightPosition);
	else{
		s = normalize(LightPosition - Position);
		ambient = 0.0f;
	}

	n = normalize(Normal);
	diffuse = max(dot(s,n), 0.0);
	if (DebugOn)
		return ambient * ambientColor + diffuse * diffuseColor;
	else
		return ambient * LightIntensity + diffuse * LightIntensity;
}

void main() {
	vec3 tempColor = vec3(color) * 0.1f;
	if(IsTheSun)
		tempColor = vec3(1.0,1.0,0.0);
	else{
		if(PointLightOn)
			tempColor += vLight(PointLightPosition, PointLightIntensity, false);
		if(HeadLightOn)
			tempColor += vLight(HeadLightPosition, HeadLightIntensity, true);
	}
	
	fragColor = vec4(tempColor, 1.0);
}
#version 150

in vec3 normal;
in vec3 position;
in vec4 color;

uniform vec3 light0Position;
uniform vec3 light1Position;
uniform vec3 light0Color;
uniform vec3 light1Color;
uniform vec3 ambientColor;
uniform float specularIntensity;
uniform float shininess;
uniform vec3 cameraPosition;

out vec4 fragColor;

vec3 applyLight(vec3 lightPosition, vec3 lightColor, vec3 surfaceColor, vec3 normal, vec3 surfacePosition, vec3 surfaceToCamera) {
	vec3 surfaceToLight = normalize(lightPosition - surfacePosition);
	float lightDistance = length(surfaceToLight);
	float brightness = dot(normal, surfaceToLight) / lightDistance;
	brightness = clamp(brightness, 0, 1);
	
	float specularCoefficient = 0.0;
	if (brightness > 0.0) {
		specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), shininess);
	}
	
	return surfaceColor * lightColor * brightness + vec3(specularCoefficient * specularIntensity);
}

void main() {
	vec3 litColor;
	vec3 scaledNormal = normalize(normal);
	vec3 surfaceToCamera = normalize(cameraPosition - position);
	
	litColor = applyLight(light0Position, light0Color, vec3(color), scaledNormal, position, surfaceToCamera);
	litColor += applyLight(light1Position, light1Color, vec3(color), scaledNormal, position, surfaceToCamera);
	
	fragColor = vec4(litColor + ambientColor, color.a);
}

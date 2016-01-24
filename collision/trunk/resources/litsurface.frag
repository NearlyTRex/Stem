#version 120

varying vec3 normal;
varying vec4 color;
varying vec4 position;

uniform vec3 diffusePosition;
uniform vec3 diffuseColor;
uniform vec3 ambientColor;
uniform float specularIntensity;
uniform float shininess;
uniform vec3 cameraPosition;

void main() {
	vec3 scaledNormal = normalize(normal);
	vec3 surfaceToLight = normalize(diffusePosition - vec3(position));
	vec3 surfaceToCamera = normalize(cameraPosition - vec3(position));
	float lightDistance = length(surfaceToLight);
	float brightness = dot(scaledNormal, surfaceToLight) / lightDistance;
	brightness = clamp(brightness, 0, 1);
	
	float specularCoefficient = 0.0;
	if (brightness > 0.0) {
		specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, scaledNormal))), shininess);
	}
	
	gl_FragColor = vec4(ambientColor + vec3(gl_Color) * diffuseColor * brightness, gl_Color.a) + vec4(specularCoefficient * specularIntensity);
}

#version 120

varying vec3 normal;
varying vec4 color;
varying vec4 position;

uniform vec3 light0;
uniform float ambient;

void main() {
	float lightDistance;
	float brightness;
	vec3 scaledNormal;
	
	scaledNormal = normalize(normal);
	lightDistance = length(light0 - vec3(position));
	brightness = dot(scaledNormal, light0 - vec3(position)) / lightDistance;
	brightness = clamp(brightness, 0, 1);
	brightness = max(ambient, brightness);
	
	gl_FragColor = vec4(vec3(gl_Color) * brightness, gl_Color.a);
}

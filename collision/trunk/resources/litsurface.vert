#version 120

varying vec3 normal;
varying vec4 color;
varying vec4 position;

void main() {
	normal = gl_Normal;
	position = gl_Vertex;
	gl_FrontColor = gl_Color;
	gl_Position = ftransform();
}

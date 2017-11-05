#version 120

uniform mat4 projectionTransform;
uniform mat4 viewTransform;
uniform mat4 modelTransform;

varying vec3 normal;
varying vec4 position;

void main() {
	normal = gl_Normal;
	position = gl_Vertex;
	gl_FrontColor = gl_Color;
	//gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = projectionTransform * viewTransform * modelTransform * gl_Vertex;
}

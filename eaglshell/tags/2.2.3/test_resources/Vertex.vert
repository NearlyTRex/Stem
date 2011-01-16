attribute vec2 vertexPosition;

uniform mat4 projectionMatrix;
uniform mat4 modelviewMatrix;

void main(void) {
	gl_Position = projectionMatrix * modelviewMatrix * vec4(vertexPosition, 0.0, 1.0);
}

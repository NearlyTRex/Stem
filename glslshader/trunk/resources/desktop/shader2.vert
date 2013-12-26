attribute vec4 vertexPosition;
attribute vec4 colorAttribute;

varying vec4 colorVarying;

void main() {
	colorVarying = colorAttribute;
	gl_Position = vertexPosition * vec4(0.5, 0.5, 0.5, 1.0);
}

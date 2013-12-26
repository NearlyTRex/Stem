attribute vec4 vertexPositionAttrib;
attribute vec2 vertexTexCoordAttrib;
attribute vec4 vertexColorAttrib;

uniform mat4 matrix;

varying lowp vec4 vertexColorVarying;
varying lowp vec2 vertexTexCoordVarying;

void main() {
	vertexColorVarying = vertexColorAttrib;
	vertexTexCoordVarying = vertexTexCoordAttrib;
	gl_Position = matrix * vertexPositionAttrib;
}

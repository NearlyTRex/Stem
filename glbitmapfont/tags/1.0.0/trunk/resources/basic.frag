varying lowp vec4 vertexColorVarying;
varying lowp vec2 vertexTexCoordVarying;

uniform sampler2D texture;

void main() {
	gl_FragColor = vertexColorVarying * texture2D(texture, vertexTexCoordVarying);
}

#version 150

in vec4 color;
in vec2 texCoord;

uniform sampler2D colorTexture;

out vec4 fragColor;

void main() {
	fragColor = texture(colorTexture, texCoord) * color;
}

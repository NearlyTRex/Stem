#version 150

in vec2 inPosition;
in vec2 inTexCoord;
in vec4 inColor;

uniform mat4 projectionTransform;
uniform mat4 viewTransform;
uniform mat4 modelTransform;
uniform vec4 foreColor;
uniform vec4 backColor;

out vec4 color;
out vec2 texCoord;

void main() {
	texCoord = inTexCoord;
	// TODO: How to foreColor/backColor math? (black backColor, white foreColor = normal; white backColor, black foreColor = invert; white both = white, black both = black)
	color = (inColor * foreColor) + backColor;
	
	gl_Position = projectionTransform * viewTransform * modelTransform * vec4(inPosition, 0.0, 1.0);
}

#version 150

in vec3 inPosition;
in vec2 inTexCoord;
in vec3 inNormal;
in vec4 inColor;

uniform mat4 projectionTransform;
uniform mat4 viewTransform;
uniform mat4 modelTransform;

out vec3 normal;
out vec3 position;
out vec4 color;
out vec2 texCoord;

void main() {
	texCoord = inTexCoord;
	color = inColor;
	normal = mat3(transpose(inverse(modelTransform))) * inNormal;
	position = vec3(modelTransform * vec4(inPosition, 1.0));
	gl_Position = projectionTransform * viewTransform * modelTransform * vec4(inPosition, 1.0);
}

#version 150

in vec3 inPosition;
in vec2 inTexCoord;
in vec3 inNormal;
in vec4 inColor;
in vec4 inBoneID;
in vec4 inBoneWeight;

uniform mat4 projectionTransform;
uniform mat4 viewTransform;
uniform mat4 modelTransform;

out vec3 normal;
out vec3 position;
out vec4 color;

void main() {
	position = inPosition;
	//texCoord = inTexCoord;
	normal = inNormal;
	color = inColor;
	//gl_Position = projectionTransform * viewTransform * modelTransform * inPosition + boneID * 0.0 + boneWeight * 0.0;
	gl_Position = projectionTransform * viewTransform * modelTransform * vec4(inPosition, 1.0);
}

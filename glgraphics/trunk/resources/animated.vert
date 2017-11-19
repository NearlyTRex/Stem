#version 150

#define BONE_COUNT_MAX 128

in vec3 inPosition;
in vec2 inTexCoord;
in vec3 inNormal;
in vec4 inColor;
in uvec4 inBoneID;
in vec4 inBoneWeight;

uniform mat4 projectionTransform;
uniform mat4 viewTransform;
uniform mat4 modelTransform;
uniform mat4 boneTransforms[BONE_COUNT_MAX];

out vec3 normal;
out vec3 position;
out vec4 color;

void main() {
	vec4 positions[4];
	vec4 weightedPosition;
	
	position = inPosition;
	//texCoord = inTexCoord;
	normal = inNormal;
	color = inColor;
	positions[0] = boneTransforms[inBoneID.x] * vec4(inPosition, 1.0) * inBoneWeight.x;
	positions[1] = boneTransforms[inBoneID.y] * vec4(inPosition, 1.0) * inBoneWeight.y;
	positions[2] = boneTransforms[inBoneID.z] * vec4(inPosition, 1.0) * inBoneWeight.z;
	positions[3] = boneTransforms[inBoneID.w] * vec4(inPosition, 1.0) * inBoneWeight.w;
	weightedPosition = positions[0] + positions[1] + positions[2] + positions[3];
	gl_Position = projectionTransform * viewTransform * modelTransform * weightedPosition;
}

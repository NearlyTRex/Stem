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
	vec4 weightedPositions[4];
	vec3 weightedNormals[4];
	vec4 weightedPosition;
	
	//texCoord = inTexCoord;
	color = inColor;
	weightedNormals[0] = mat3(boneTransforms[inBoneID.x]) * inNormal * inBoneWeight.x;
	weightedNormals[1] = mat3(boneTransforms[inBoneID.y]) * inNormal * inBoneWeight.x;
	weightedNormals[2] = mat3(boneTransforms[inBoneID.z]) * inNormal * inBoneWeight.x;
	weightedNormals[3] = mat3(boneTransforms[inBoneID.w]) * inNormal * inBoneWeight.x;
	normal = weightedNormals[0] + weightedNormals[1] + weightedNormals[2] + weightedNormals[3];
	weightedPositions[0] = boneTransforms[inBoneID.x] * vec4(inPosition, 1.0) * inBoneWeight.x;
	weightedPositions[1] = boneTransforms[inBoneID.y] * vec4(inPosition, 1.0) * inBoneWeight.y;
	weightedPositions[2] = boneTransforms[inBoneID.z] * vec4(inPosition, 1.0) * inBoneWeight.z;
	weightedPositions[3] = boneTransforms[inBoneID.w] * vec4(inPosition, 1.0) * inBoneWeight.w;
	weightedPosition = weightedPositions[0] + weightedPositions[1] + weightedPositions[2] + weightedPositions[3];
	position = vec3(weightedPosition);
	gl_Position = projectionTransform * viewTransform * modelTransform * weightedPosition;
}

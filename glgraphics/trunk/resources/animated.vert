#version 150

in vec3 inPosition;
in vec2 inTexCoord;
in vec3 inNormal;
in vec4 inTangent;
in vec4 inColor;
in vec4 inBoneID;
in vec4 inBoneWeight;

#define BONE_COUNT_MAX 128

uniform mat4 projectionTransform;
uniform mat4 viewTransform;
uniform mat4 modelTransform;
uniform vec4 materialColor;
uniform mat4 boneTransforms[BONE_COUNT_MAX];

out vec3 position;
out vec4 color;
out vec2 texCoord;
out mat3 tbnMatrix;

void main() {
	vec4 weightedPositions[4];
	vec3 weightedNormals[4];
	vec4 weightedPosition;
	vec3 normal, tangent, bitangent;
	mat3 normalMatrix;
	
	texCoord = inTexCoord;
	color = inColor * materialColor;
	
	weightedNormals[0] = mat3(boneTransforms[int(inBoneID.x)]) * inNormal * inBoneWeight.x;
	weightedNormals[1] = mat3(boneTransforms[int(inBoneID.y)]) * inNormal * inBoneWeight.x;
	weightedNormals[2] = mat3(boneTransforms[int(inBoneID.z)]) * inNormal * inBoneWeight.x;
	weightedNormals[3] = mat3(boneTransforms[int(inBoneID.w)]) * inNormal * inBoneWeight.x;
	normal = weightedNormals[0] + weightedNormals[1] + weightedNormals[2] + weightedNormals[3];
	
	normalMatrix = mat3(transpose(inverse(modelTransform))); // TODO: Calculate this outside the shader
	normal = normalMatrix * normal;
	tangent = normalMatrix * inTangent.xyz;
	bitangent = inTangent.w * cross(normal, tangent);
	tbnMatrix = mat3(tangent, bitangent, normal);
	
	weightedPositions[0] = boneTransforms[int(inBoneID.x)] * vec4(inPosition, 1.0) * inBoneWeight.x;
	weightedPositions[1] = boneTransforms[int(inBoneID.y)] * vec4(inPosition, 1.0) * inBoneWeight.y;
	weightedPositions[2] = boneTransforms[int(inBoneID.z)] * vec4(inPosition, 1.0) * inBoneWeight.z;
	weightedPositions[3] = boneTransforms[int(inBoneID.w)] * vec4(inPosition, 1.0) * inBoneWeight.w;
	weightedPosition = weightedPositions[0] + weightedPositions[1] + weightedPositions[2] + weightedPositions[3];
	position = vec3(modelTransform * weightedPosition);
	
	gl_Position = projectionTransform * viewTransform * modelTransform * weightedPosition;
}

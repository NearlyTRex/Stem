#version 150

in vec3 inPosition;
in vec2 inTexCoord;
in vec3 inNormal;
in vec4 inTangent;
in vec4 inColor;

uniform mat4 projectionTransform;
uniform mat4 viewTransform;
uniform mat4 modelTransform;

out vec3 position;
out vec4 color;
out vec2 texCoord;
out mat3 tbnMatrix;

void main() {
	vec3 normal, tangent, bitangent;
	mat3 normalMatrix;
	
	texCoord = inTexCoord;
	color = inColor;
	
	normalMatrix = mat3(transpose(inverse(modelTransform))); // TODO: Calculate this outside the shader
	normal = normalMatrix * inNormal;
	tangent = normalMatrix * inTangent.xyz;
	bitangent = inTangent.w * cross(normal, tangent);
	tbnMatrix = mat3(tangent, bitangent, normal);
	
	position = vec3(modelTransform * vec4(inPosition, 1.0));
	gl_Position = projectionTransform * viewTransform * modelTransform * vec4(inPosition, 1.0);
}

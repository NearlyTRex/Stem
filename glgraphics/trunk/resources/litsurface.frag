#version 150

in vec3 position;
in vec4 color;
in vec2 texCoord;
in mat3 tbnMatrix;

uniform vec3 light0Position;
uniform vec3 light1Position;
uniform vec3 light0Color;
uniform vec3 light1Color;
uniform vec3 ambientColor;
uniform float specularity;
uniform float shininess;
uniform float emissiveness;
uniform vec3 cameraPosition;
uniform sampler2D colorTexture;
uniform sampler2D normalTexture;

out vec4 fragColor;

vec3 applyLight(vec3 lightPosition, vec3 lightColor, vec3 ambientColor, vec3 surfaceColor, vec3 normal, vec3 surfacePosition, vec3 surfaceToCamera) {
	vec3 surfaceToLight = normalize(lightPosition - surfacePosition);
	float diffuseStrength = max(dot(normal, surfaceToLight), 0.0);
	vec3 diffuseColor = diffuseStrength * lightColor;
	
	vec3 reflection = reflect(-surfaceToLight, normal);
	float specularStrength = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), shininess);
	vec3 specularColor = specularity * specularStrength * lightColor;
	
	return (ambientColor + diffuseColor + specularColor) * surfaceColor;
}

void main() {
	vec4 textureColor;
	vec3 litColor;
	vec3 surfaceToCamera = normalize(cameraPosition - position);
	vec3 adjustedNormal;
	
	adjustedNormal = texture(normalTexture, texCoord).rgb;
	adjustedNormal = adjustedNormal * 2.0 - 1.0;
	adjustedNormal = normalize(tbnMatrix * adjustedNormal);
	
	textureColor = texture(colorTexture, texCoord) * color;
	litColor = applyLight(light0Position, light0Color, ambientColor, textureColor.rgb, adjustedNormal, position, surfaceToCamera);
	litColor += applyLight(light1Position, light1Color, ambientColor, textureColor.rgb, adjustedNormal, position, surfaceToCamera);
	
	// TODO: Specular reflections should override translucency and increase alpha
	fragColor = vec4(emissiveness * vec3(textureColor) + (1.0 - emissiveness) * litColor, textureColor.a);
}

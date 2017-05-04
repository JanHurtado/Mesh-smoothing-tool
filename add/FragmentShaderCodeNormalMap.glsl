#version 430

out vec4 outColor;

in vec3 normalWorld;
in vec3 vertexPositionWorld;
in vec3 theColor;
in vec3 normalModel;

uniform vec3 lightPositionVector;
uniform vec3 cameraPositionVector;
uniform vec4 lightVector;

void main()
{
	vec3 lightVectorWorld = normalize(lightPositionVector - vertexPositionWorld);
	float brightness = dot(lightVectorWorld, normalize(normalWorld));
	vec4 diffuseLight = vec4(brightness, brightness, brightness, 1.0);

	// Specular
	vec3 reflectedLightVectorWorld = reflect(-lightVectorWorld, normalWorld);
	vec3 eyeVectorWorld = normalize(cameraPositionVector - vertexPositionWorld);
	float s = clamp(dot(reflectedLightVectorWorld, eyeVectorWorld), 0, 1);
	s = pow(s, 25);
	vec4 specularLight = vec4(s, s, s, 1);

	outColor = lightVector + clamp(diffuseLight, 0, 1) + specularLight;
	outColor = vec4(theColor[0]*outColor[0],theColor[1]*outColor[1],theColor[2]*outColor[2],1.0);
	outColor = vec4(normalModel*theColor,0.5);
}
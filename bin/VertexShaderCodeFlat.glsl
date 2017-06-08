#version 430

in layout(location=0) vec3 position;
in layout(location=1) vec3 vertexColor;
in layout(location=2) vec3 vertexNormal;

uniform mat4 modelToProjectionMatrix;
uniform mat4 modelToWorldMatrix;
uniform vec3 aditionalProperties;

flat out vec3 normalWorld;
out vec3 vertexPositionWorld;
out vec3 theColor;

void main()
{
	gl_Position = modelToProjectionMatrix * vec4(position, 1);
	normalWorld = vec3(modelToWorldMatrix * vec4(vertexNormal, 0));
	vertexPositionWorld = vec3(modelToWorldMatrix * vec4(position,1));
	theColor = vertexColor;
	float ps = aditionalProperties[0];
	gl_PointSize = ps;
	//gl_PointSize = 10.f;
}
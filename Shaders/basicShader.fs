#version 120

varying vec2 txtCoord0;
varying vec3 normal0;

uniform sampler2D sampler;

void main()
{
	gl_FragColor = vec4(1.0, 0.0, 0.0, 0.0) * clamp(dot(-vec3(0.0f, 0.0f, 1.0f), normal0), 0.0f, 1.0f);
	/*gl_FragColor = texture2D(sampler, texCoord0)
		* clamp(dot(-vec3(0.0f, 0.0f, 1.0f), normal0), 0.0f, 1.0f);*/
}
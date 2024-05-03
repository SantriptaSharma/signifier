in vec2 fragTexCoord;
in vec4 fragColor;

out vec4 finalColor;

uniform vec3 clearColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

void main()
{
	finalColor = texture2D(texture0, fragTexCoord);

	finalColor.rgb = finalColor.a == 1 ? clearColor : finalColor.rgb;

	finalColor.a = 1;
}
#version 330

#define MAX_OBJECTS 100

struct Object {
	int type;
	mat4 transform;
	vec3 color;
}

in vec2 fragTexCoord;
in vec4 fragColor;

out vec4 finalColor;

uniform vec3 viewEye;
uniform vec3 viewCenter;
uniform float runTime;
uniform vec2 resolution;
uniform Object objects[MAX_OBJECTS];

vec2 castRay( in vec3 ro, in vec3 rd )
{
    float tmin = 0.2;
    float tmax = 30.0;


}

mat3 setCamera( in vec3 ro, in vec3 ta, float cr )
{
    vec3 cw = normalize(ta-ro);
    vec3 cp = vec3(sin(cr), cos(cr),0.0);
    vec3 cu = normalize( cross(cw,cp) );
    vec3 cv = normalize( cross(cu,cw) );
    return mat3( cu, cv, cw );
}

void main()
{
    vec3 tot = vec3(0.0);
	vec2 p = (-resolution.xy + 2.0*gl_FragCoord.xy)/resolution.y;

	vec3 ro = viewEye;
	vec3 ta = viewCenter;

	// camera-to-world transformation
	mat3 ca = setCamera( ro, ta, 0.0 );
	// ray direction (baked assumption of focal plane distance)
	vec3 rd = ca * normalize( vec3(p.xy,2.0) );

	vec3 col = render( ro, rd );

	// gamma correct
	col = pow( col, vec3(0.4545) );

    finalColor = vec4(col, 1.0);
}
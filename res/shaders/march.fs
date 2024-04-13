#define MAX_OBJECTS 800
#define MAX_DISTANCE 1000.0

struct Object {
	int type;
	mat4 transform;
	vec3 color;
};

in vec2 fragTexCoord;
in vec4 fragColor;

out vec4 finalColor;

uniform vec3 viewEye;
uniform vec3 viewCenter;
uniform vec2 resolution;
uniform Object objects[MAX_OBJECTS];
uniform int objectCount;

mat3 setCamera( in vec3 ro, in vec3 ta, float cr )
{
    vec3 cw = normalize(ta-ro);
    vec3 cp = vec3(sin(cr), cos(cr),0.0);
    vec3 cu = normalize( cross(cw,cp) );
    vec3 cv = normalize( cross(cu,cw) );
    return mat3( cu, cv, cw );
}

// float sdfSphere(in Object o, vec3 point) {
		
// }

// float sdfScene(vec3 point) {
// 	float minDistance = MAX_DISTANCE;

// 	for (int i = 0; i < objectCount; i++) {
// 		switch (objects[i].type) {
// 			case 0: 

// 			break;
// 		}
// 	}
// }

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

	// vec3 col = render( ro, rd );
	vec3 col = vec3(1.0);

	// gamma correct
	col = pow( col, vec3(0.4545) );

    finalColor = vec4(p, 0.0, 1.0);
}
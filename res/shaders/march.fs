#define MAX_OBJECTS 128
#define MAX_DISTANCE 1000.0
#define MAX_ITERATIONS 512 
#define EPS 0.001

struct Object {
	int type;
	mat4 invTransform;
	vec3 size;
	vec3 color;
};

struct Hit {
	float dist;
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

uniform vec3 clearColor;

mat3 setCamera( in vec3 ro, in vec3 ta, float cr )
{
    vec3 cw = normalize(ta-ro);
    vec3 cp = vec3(sin(cr), cos(cr),0.0);
    vec3 cu = normalize( cross(cw,cp) );
    vec3 cv = normalize( cross(cu,cw) );
    return mat3( cu, cv, cw );
}

Hit sdfSphere(in Object o, in vec3 point) {
	vec3 p = (o.invTransform * vec4(point, 1.0)).xyz;
	return Hit(length(p) - o.size.x, o.color);
}

Hit sdfInfPlane(in Object o, in vec3 point) {
	vec3 p = (o.invTransform * vec4(point, 1.0)).xyz;
	return Hit(abs(p.y), o.color);
}

// polynomial smooth minimum with exposed blending factor from https://iquilezles.org/articles/smin/
vec2 smin( float a, float b, float k )
{
    float h = 1.0 - min( abs(a-b)/(4.0*k), 1.0 );
    float w = h*h;
    float m = w*0.5;
    float s = w*k;
    return (a<b) ? vec2(a-s,m) : vec2(b-s,1.0-m);
}

Hit hitUnion(Hit a, Hit b) {
	vec2 res = smin(a.dist, b.dist, 0.3);

	return Hit(res.x, mix(a.color, b.color, res.y));
}

Hit sdfScene(in vec3 point) {
	Hit minHit = Hit(MAX_DISTANCE, vec3(0.0));

	for (int i = 0; i < objectCount; i++) {
		switch (objects[i].type) {
			case 0: 
				Hit hit = sdfSphere(objects[i], point);
				minHit = hitUnion(minHit, hit);
			break;

			case 1:
				hit = objects[i].size.x == 0.0 ? sdfInfPlane(objects[i], point) : sdfInfPlane(objects[i], point);
				minHit = hitUnion(minHit, hit);
			break;
		}
	}

	return minHit;
}

// TODO: add AA

vec3 march(in vec3 ro, in vec3 rd) {
	float t = 0.0;
	int i;

	for (i = 0; i < MAX_ITERATIONS; i++) {
		vec3 point = ro + rd * t;
		Hit hit = sdfScene(point);
		if (hit.dist < EPS) {
			return hit.color;
		}

		t += hit.dist;
		if (t > MAX_DISTANCE) {
			break;
		}
	}
	
	return clearColor;
}

void main()
{
    vec3 tot = vec3(0.0);
	vec2 p = (-resolution.xy + 2.0*gl_FragCoord.xy)/resolution.y;
	
	vec3 ro = viewEye;
	vec3 ta = viewCenter;

	// camera-to-world transformation
	mat3 ca = setCamera( ro, ta, 0.0);
	// ray direction
	vec3 rd = ca * normalize( vec3(p.xy, 2) );

	vec3 col = march(ro, rd);
	// gamma correct
	col = pow( col, vec3(0.4545));

	finalColor = vec4(col, 1.0);
}
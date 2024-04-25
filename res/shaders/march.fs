#define MAX_OBJECTS 128
#define MAX_LIGHTS 8
#define MAX_DISTANCE 1000.0
#define MAX_ITERATIONS 512 
#define MAX_SHAD_ITERATIONS 128
#define MAX_SHAD_DISTANCE 100.0
#define EPS 0.001
#define SHAD_EPS 0.1
#define NORMAL_EPS 0.0001
#define LIGHT_ZERO_POINT 0.01
#define SHININESS 10
#define SPEC_COLOR (vec3(1.0))

#define AA_ROUNDS 3

struct Object {
	int type;
	int combineType;
	mat4 invTransform;
	vec3 size;
	vec3 color;
};

struct Light {
	vec3 position;
	vec3 color;
	int type;
	float intensity;
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
uniform Light lights[MAX_LIGHTS];
uniform int lightCount;

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
	return Hit(abs(p.y) - EPS * 10, o.color);
}

// Hit sdfBox(in Object o, in vec3 point) {
// 	vec3 p = (o.invTransform * vec4(point, 1.0)).xyz;
// 	vec3 q = abs(p) - o.size;

// 	float dist = 
// }

// polynomial smooth minimum with exposed blending factor from https://iquilezles.org/articles/smin/
vec2 smin( float a, float b, float k )
{
	float h = 1.0 - min( abs(a-b)/(4.0*k), 1.0 );
	float w = h*h;
	float m = w*0.5;
	float s = w*k;
	return (a<b) ? vec2(a-s,m) : vec2(b-s,1.0-m);
}

Hit hitSUnion(Hit a, Hit b) {
	vec2 res = smin(a.dist, b.dist, 0.3);

	return Hit(res.x, mix(a.color, b.color, res.y));
}

Hit hitUnion(Hit a, Hit b) {
	return a.dist < b.dist ? a : b;
}

Hit hitIntersection(Hit a, Hit b) {
	return a.dist > b.dist ? a : b;
}

Hit hitDifference(Hit a, Hit b) {
	return Hit(max(a.dist, -b.dist), a.color);
}

Hit hitJoin(Hit a, Hit b, int combineType) {
	switch (combineType) {
		case 0:
			return hitUnion(a, b);
		break;

		case 1:
			return hitSUnion(a, b);
		break;

		case 2:
			return hitDifference(a, b);
		break;

		case 3:
			return hitIntersection(a, b);
		break;

		default:
			return hitUnion(a, b);
		break;
	}
}

Hit sdfScene(in vec3 point) {
	Hit minHit = Hit(MAX_DISTANCE, vec3(0.0));

	for (int i = 0; i < objectCount; i++) {
		switch (objects[i].type) {
			case 0: 
				Hit hit = sdfSphere(objects[i], point);
				minHit = hitJoin(minHit, hit, objects[i].combineType);
			break;

			case 1:
				hit = objects[i].size.x == 0.0 ? sdfInfPlane(objects[i], point) : sdfInfPlane(objects[i], point);
				minHit = hitJoin(minHit, hit, objects[i].combineType);
			break;
		}
	}

	return minHit;
}

vec3 estimateNormal(in vec3 point) {
	vec3 eps = vec3(NORMAL_EPS, 0.0, 0.0);
	vec3 normal = vec3(
		sdfScene(point + eps.xyy).dist - sdfScene(point - eps.xyy).dist,
		sdfScene(point + eps.yxy).dist - sdfScene(point - eps.yxy).dist,
		sdfScene(point + eps.yyx).dist - sdfScene(point - eps.yyx).dist
	);

	return normalize(normal);
}

vec4 get_dir_intensity(in vec3 point, in vec3 normal, in Light light) {
	vec3 diff = light.position - point;
	vec3 lightDir = (light.type == 0) ? normalize(diff) : -light.position;
	float dist = length(diff);
	float intensity = (light.type == 0) ? (light.intensity)/(dist * dist) : light.intensity;

	return vec4(lightDir, intensity);
}

float soft_shadow(in vec3 ro, in vec3 rd, float k) {
	float res = 1.0;
	float t = SHAD_EPS;

	for(int i=0; i < MAX_SHAD_ITERATIONS && t < MAX_SHAD_DISTANCE; i++)
	{
		float h = sdfScene(ro + rd * t).dist;
		if(h < EPS)
			return 0.0;
		res = min(res, k * h / t);
		t += h;
	}

	return res;
}

vec3 lighting(in vec3 point, in vec3 normal, in vec3 color, in vec3 view) {
	vec3 lightColor = vec3(LIGHT_ZERO_POINT) * color;
	for (int i = 0; i < lightCount; i++) {
		vec4 dir_intensity = get_dir_intensity(point, normal, lights[i]);
		vec3 lightDir = dir_intensity.xyz;
		float intensity = dir_intensity.w;

		float shad = soft_shadow(point, lightDir, 16.0);

		// TODO: allow control for 0-point (ambient scene color), and specular lighting parameters
		float diff = max(dot(normal, lightDir), LIGHT_ZERO_POINT);
		float specular = 0.0;

		if (diff > 0.0) {
			vec3 H = normalize(lightDir + view);
			float spec = max(dot(H, normal), 0.0);
			specular = pow(spec, SHININESS);
		}

		lightColor += (lights[i].color * diff * intensity * color + SPEC_COLOR * specular * intensity * color) * shad;
	}

	return lightColor;
}

// TODO: add AA

vec3 march(in vec3 ro, in vec3 rd) {
	float t = 0.0;
	int i;

	for (i = 0; i < MAX_ITERATIONS; i++) {
		vec3 point = ro + rd * t;
		Hit hit = sdfScene(point);
		if (hit.dist < EPS) {
			vec3 normal = estimateNormal(point);
			return lighting(point, normal, hit.color, -rd);
		}

		t += hit.dist;
		if (t > MAX_DISTANCE) {
			break;
		}
	}
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
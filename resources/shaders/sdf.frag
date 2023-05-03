#version 410
// Fragment shader

// Defined in shading.glsl
vec3 phongShading(vec3 matcolour, vec3 coords, vec3 normal);
vec3 normalShading(vec3 normal);

layout(location = 0) in vec3 vertcoords_vs;

out vec4 fColor;

uniform mat4 toworldmatrix;
uniform mat4 modelviewmatrix;
uniform mat4 projectionmatrix;
uniform sampler2D image;

const float radius = .4;
const vec3 backgroundCol = vec3(0.2);
const vec3 matCol = vec3(0.7, 0.8, 1);
const int numSteps = 256;

vec3 center = vec3(0);

float sdfSphere(vec3 pos, vec3 center) {
    return distance(pos, center) - radius;
}

float sdf(vec3 pos) {
    return sdfSphere(pos, center);
}

vec3 calcNormal(in vec3 pos) {
    vec2 e = vec2(1.0, -1.0) * 0.5773;
    const float eps = 0.0005;
    return normalize(vec3(e.xy, e.y) * sdf(pos + vec3(e.xy, e.y) * eps) +
    vec3(e.yy, e.x) * sdf(pos + vec3(e.yy, e.x) * eps) +
    vec3(e.yx, e.y) * sdf(pos + vec3(e.yx, e.y) * eps) +
    vec3(e.xx, e.x) * sdf(pos + vec3(e.xx, e.x) * eps));
}

// TODO: get aspect ratio of image
// Render image as blocks
void main() {
    // Generate ray
    float aspectRatio = projectionmatrix[1][1] / projectionmatrix[0][0];
    vec2 uv = vertcoords_vs.xy * vec2(aspectRatio, 1.0) / projectionmatrix[1][1];
//    vec3 ro = vec3(0.0);
//    vec3 rd = normalize(vec3(uv, -1.0));

    vec4 rayOrigin = toworldmatrix * vec4(0.0, 0.0, 0.0, 1.0);
    vec4 rayDirection = toworldmatrix * vec4(uv, -1.0, 0.0);
    vec3 ro = vec3(rayOrigin);
    vec3 rd = normalize(vec3(rayDirection));

    const float tmax = 5.0;
    float t = 0.0;
    // raymarch
    for (int i = 0; i < numSteps; i++) {
        vec3 pos = ro + t * rd;
        float h = sdf(pos);
        if (h < 0.0001 || t > tmax) break;
        t += h;
    }

    // shading/lighting
    vec3 col = backgroundCol;
    if (t < tmax) {
        vec3 pos = ro + t * rd;
        //    pos = vec3(inverse(modelviewmatrix) * vec4(pos,1));
        vec3 normal = calcNormal(pos);
        col = phongShading(matCol, pos, normal);
    }
    fColor = vec4(col, 1.0);
}

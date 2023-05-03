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

uniform int imwidth;
uniform int imheight;

const vec3 backgroundCol = vec3(0.2);
const vec3 matCol = vec3(0.7, 0.8, 1);
const int numSteps = 4096;

float sdSphere(vec3 p, float d) { return length(p) - d; }

float sdBox( vec3 p, vec3 b )
{
    vec3 d = abs(p) - b;
    return min(max(d.x,max(d.y,d.z)),0.0) +
    length(max(d,0.0));
}

// TODO: replace with image
bool getVoxel(ivec3 c) {
    vec3 p = vec3(c) + vec3(0.5);
    float d = min(max(-sdSphere(p, 7.5), sdBox(p, vec3(6.0))), -sdSphere(p, 25.0));
    return d < 0.0;
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

    ivec3 mapPos = ivec3(floor(ro + 0.));

    vec3 deltaDist = abs(vec3(length(rd)) / rd);

    ivec3 rayStep = ivec3(sign(rd));

    vec3 sideDist = (sign(rd) * (vec3(mapPos) - ro) + (sign(rd) * 0.5) + 0.5) * deltaDist;

    bvec3 mask;

    for (int i = 0; i < 64; i++) {
        if (getVoxel(mapPos)) continue;
        mask = lessThanEqual(sideDist.xyz, min(sideDist.yzx, sideDist.zxy));
        sideDist += vec3(mask) * deltaDist;
        mapPos += ivec3(vec3(mask)) * rayStep;
    }

    // shading/lighting
    vec3 color;
    if (mask.x) {
        color = vec3(0.5);
    }
    if (mask.y) {
        color = vec3(1.0);
    }
    if (mask.z) {
        color = vec3(0.75);
    }
    fColor = vec4(color, 1.0);
}

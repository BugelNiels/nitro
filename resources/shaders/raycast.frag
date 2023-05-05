#version 410
// Fragment shader

// Defined in shading.glsl
vec3 phongShading(vec3 matcolour, vec3 coords, vec3 normal);
vec3 normalShading(vec3 normal);

layout(location = 0) in vec3 vertcoords_vs;

out vec4 fColor;

uniform mat4 toWorldMatrix;
uniform mat4 projectionMatrix;
uniform sampler2D image;

uniform int imWidth;
uniform int imHeight;

uniform bool enableImageColors;
uniform bool enableOrthographic;

const vec3 backgroundCol = vec3(0.2);
const vec3 matCol = vec3(0.7, 0.8, 1);
const int numSteps = 4096;

float sdSphere(vec3 p, float d) { return length(p) - d; }

float sdBox(vec3 p, vec3 b)
{
    vec3 d = abs(p) - b;
    return min(max(d.x, max(d.y, d.z)), 0.0) +
    length(max(d, 0.0));
}

// TODO: replace with image
//bool getVoxel(ivec3 c) {
//    vec3 p = vec3(c) + vec3(0.5);
//    float d = min(max(-sdSphere(p, 7.5), sdBox(p, vec3(6.0))), -sdSphere(p, 25.0));
//    return d < 0.0;
//}

bool getVoxel(ivec3 c) {

    c += ivec3(imWidth / 2, imHeight / 2, 128);

    //    vec3 pos = c - vec3(imwidth / 2.0, imheight / 2.0, 255 / 2);
    if (c.x < 0 || c.x >= imWidth || c.y < 0 || c.y >= imHeight || c.z < 0 || c.z > 255) {
        return false;
    }
    vec2 texUv = vec2(float(c.x) / float(imWidth), float(c.y) / float(imHeight));
    float z = vec3(texture(image, texUv)).r * 255 + 1;
    vec3 boxDims = vec3(imWidth, imHeight, z);
    //    vec3 boxDims = vec3(2);
    return sdBox(c, boxDims) < 0.0;
    //    return sdBox(c, vec3(6.0)) < 0.0;
}

vec3 normals(vec3 pos) {
    vec3 fdx = dFdx(pos);
    vec3 fdy = dFdy(pos);
    return normalize(cross(fdx, fdy));
}
vec2 matcap(vec3 eye, vec3 normal) {
    vec3 reflected = reflect(eye, normal);
    float m = 2.8284271247461903 * sqrt(reflected.z + 1.0);
    return reflected.xy / m + 0.5;
}

// TODO: get aspect ratio of image
// Render image as blocks
void main() {

    vec3 ro, rd;
    if (enableOrthographic) {
        // Generate ray
        float aspectRatio = projectionMatrix[1][1] / projectionMatrix[0][0];
        vec4 rayOrigin = toWorldMatrix * vec4(vertcoords_vs.xy * vec2(aspectRatio, 1.0) * 255, 0.0, 1.0);// orthographic
        vec4 rayDirection = toWorldMatrix * vec4(0, 0, -1.0, 0.0);// orthographic
        ro = vec3(rayOrigin);
        rd = normalize(vec3(rayDirection));
    } else {
        // Generate ray
        float aspectRatio = projectionMatrix[1][1] / projectionMatrix[0][0];
        vec2 uv = vertcoords_vs.xy * vec2(aspectRatio, 1.0) / projectionMatrix[1][1];
        vec4 rayOrigin = toWorldMatrix * vec4(0.0, 0.0, 0.0, 1.0);// perspective
        vec4 rayDirection = toWorldMatrix * vec4(uv, -1.0, 0.0);// perspective
        ro = vec3(rayOrigin);
        rd = normalize(vec3(rayDirection));
    }


    // Starts at ray origin
    ivec3 mapPos = ivec3(floor(ro + 0.));

    // abs(1 / rd)
    vec3 deltaDist = abs(vec3(length(rd)) / rd);

    ivec3 rayStep = ivec3(sign(rd));

    vec3 sideDist = (sign(rd) * (vec3(mapPos) - ro) + (sign(rd) * 0.5) + 0.5) * deltaDist;

    bvec3 mask;

    bool background = true;

    // Set proper start position
    //    ;

    //    mapPos += ivec3((sdBox(mapPos, vec3(imwidth / 2, imheight / 2, 128))-5) * rd);

    // TODO: efficient
    for (int i = 0; i < 2048; i++) {
        if (getVoxel(mapPos)) {
            // TODO cleaner
            background = false;
            break;
        }
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
    if (background) {
        color = vec3(0.2);
    } else {
        if (enableImageColors) {
            color *= (mapPos.z + 128) / 255.0f;
        }
    }
    fColor = vec4(color, 1.0);
}

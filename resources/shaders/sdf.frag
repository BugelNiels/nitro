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

//vec3 center = vec3(0);

//float sdfSphere(vec3 pos, vec3 center) {
//    return distance(pos, center) - radius;
//}


float sdBox(vec3 p, vec3 b)
{
    vec3 q = abs(p) - b;
    return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}


vec3 opRepLim(vec3 p, float s, vec3 lima, vec3 limb)
{
    return p-s*clamp(round(p/s), lima, limb);
}


float sdf(vec3 pos) {
    vec2 texUv = vec2(pos.x / imwidth + 0.5, pos.y / imheight+ 0.5);
    float z = vec3(texture(image, texUv)).r * 255;
    vec3 imDim = vec3(imwidth, imheight, z);
    vec3 r = opRepLim(pos, 1.0, - imDim / 2, imDim / 2);
    //    return sdBox(r, vec3(imwidth / 2, imheight / 2, 255));
    return sdBox(r, vec3(1));
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

    const float tmax = 150096.0;
    float t = 0.0;
    // raymarch
    for (int i = 0; i < numSteps; i++) {
        vec3 pos = ro + t * rd;
        float h = sdf(pos);
        if (h < 0|| t > tmax) break;
        t += h;
    }

    // shading/lighting
    vec3 col = backgroundCol;
    if (t < tmax) {
        vec3 pos = ro + t * rd;
        vec2 texUv = vec2(pos.x / imwidth + 0.5, pos.y / imheight + 0.5);
//        col = vec3(texture(image, texUv));
        col = vec3(pos.z) / 255.0;
        //    pos = vec3(inverse(modelviewmatrix) * vec4(pos,1));
        //        vec3 normal = calcNormal(pos);
        //        col = phongShading(matCol, pos, normal);
    }
    fColor = vec4(col, 1.0);
}

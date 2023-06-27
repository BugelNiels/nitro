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
uniform bool minecraft;

const vec3 backgroundCol = vec3(0.2, 0.2, 0.22);


float sdBox(vec3 p, vec3 b) {
    vec3 q = abs(p) - b;
    return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}

bool getVoxel(ivec3 c) {
    vec2 texUv =  c.xy / vec2(imWidth, imHeight);
    float z = vec3(texture(image, texUv)).r * 255 + 1;
    return sdBox(c, vec3(imWidth, imHeight, z)) < 0.0;
}

float insideBox3D(vec3 v, vec3 bottomLeft, vec3 topRight) {
    vec3 s = step(bottomLeft, v) - step(topRight, v);
    return s.x * s.y * s.z;
}

// source: https://gamedev.stackexchange.com/questions/18436/most-efficient-aabb-vs-ray-collision-algorithms
bool rayboxintersect(vec3 raypos, vec3 raydir, vec3 boxmin, vec3 boxmax, out float t)
{
    float t1 = (boxmin.x - raypos.x) / raydir.x;
    float t2 = (boxmax.x - raypos.x) / raydir.x;
    float t3 = (boxmin.y - raypos.y) / raydir.y;
    float t4 = (boxmax.y - raypos.y) / raydir.y;
    float t5 = (boxmin.z - raypos.z) / raydir.z;
    float t6 = (boxmax.z - raypos.z) / raydir.z;

    // Max with 0; in case we are inside the box we want a t of 0
    t = max(max(max(min(t1, t2), min(t3, t4)), min(t5, t6)), 0);
    float tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));

    if (tmax < 0.0) {
        return false; // box on ray but behind ray origin
    }

    if (t > tmax) {
        return false; // ray doesn't intersect box
    }
    return true;
}

vec3 getVoxelCol(bvec3 mask, ivec3 pos) {
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
    if (enableImageColors) {
        color *= pos.z / 255.0f;
    }
    return color;
}

// I don't know why I did this
vec3 getMinecraftCol(bvec3 mask, ivec3 pos) {
    vec2 texUv =  pos.xy / vec2(imWidth, imHeight);
    int z = int(vec3(texture(image, texUv)).r * 255 + 1);
    vec3 color;
    int distFromSurface = abs(pos.z - z);
    if(distFromSurface <= 1) {
        vec3 grassCol = vec3(0.27, 0.41, 0.235);
        if (mask.x) {
            color = grassCol * 0.5;
        }
        if (mask.y) {
            color = grassCol;
        }
        if (mask.z) {
            color = grassCol * 0.75;
        }
    } else if(distFromSurface < z / 16) {
        vec3 dirtCol = vec3(0.39, 0.27, 0.19);
        if (mask.x) {
            color = dirtCol * 0.5;
        }
        if (mask.y) {
            color = dirtCol;
        }
        if (mask.z) {
            color = dirtCol * 0.75;
        }
    } else {
        vec3 stoneCol = vec3(0.39, 0.39, 0.39);
        if (mask.x) {
            color = stoneCol * 0.5;
        }
        if (mask.y) {
            color = stoneCol;
        }
        if (mask.z) {
            color = stoneCol * 0.75;
        }

    }
    return color;
}

// Render image as blocks
void main() {
    float aspectRatio = projectionMatrix[1][1] / projectionMatrix[0][0];
    vec3 ro, rd;
    if (enableOrthographic) {
        // Generate ray
        vec4 rayOrigin = toWorldMatrix * vec4(vertcoords_vs.xy * vec2(aspectRatio, 1.0) * 255, 0.0, 1.0);// orthographic
        vec4 rayDirection = toWorldMatrix * vec4(0, 0, -1.0, 0.0);// orthographic
        ro = vec3(rayOrigin);
        rd = normalize(vec3(rayDirection));
    } else {
        // Generate ray
        vec2 uv = vertcoords_vs.xy * vec2(aspectRatio, 1.0) / projectionMatrix[1][1];
        vec4 rayOrigin = toWorldMatrix * vec4(0.0, 0.0, 0.0, 1.0);// perspective
        vec4 rayDirection = toWorldMatrix * vec4(uv, -1.0, 0.0);// perspective
        ro = vec3(rayOrigin);
        rd = normalize(vec3(rayDirection));
    }
    vec3 boxSize = vec3(imWidth, imHeight, 256);
    vec3 offset = boxSize / 2.0;
    float t;
    if (rayboxintersect(ro, rd, -offset, offset, t)) {
        ro += offset;
        ro += (t - 0.0001) * rd;
        ivec3 mapPos = ivec3(floor(ro + 0.));
        vec3 deltaDist = abs(vec3(length(rd)) / rd);
        ivec3 rayStep = ivec3(sign(rd));
        vec3 sideDist = (sign(rd) * (vec3(mapPos) - ro) + (sign(rd) * 0.5) + 0.5) * deltaDist;
        bvec3 mask = bvec3(0);
        mask = lessThanEqual(sideDist.xyz, min(sideDist.yzx, sideDist.zxy));
        int maxSteps = imWidth + imHeight + 256;
        for (int i = 0; i < maxSteps; i++) {
            if (getVoxel(mapPos)) {
                if(minecraft) {
                    fColor = vec4(getMinecraftCol(mask, mapPos), 1.0);
                } else {
                    fColor = vec4(getVoxelCol(mask, mapPos), 1.0);
                }
                return;
            }
            mask = lessThanEqual(sideDist.xyz, min(sideDist.yzx, sideDist.zxy));
            sideDist += vec3(mask) * deltaDist;
            mapPos += ivec3(vec3(mask)) * rayStep;
            if (mapPos.x < 0 || mapPos.x > imWidth || mapPos.y < 0 || mapPos.y > imHeight || mapPos.z < 0 || mapPos.z > 256) {
                break;
            }
        }
    }
    if(minecraft) {
        fColor = vec4(0.54, 0.67, 1.0, 1.0);
    } else {
        fColor = vec4(backgroundCol, 1.0);
    }
}

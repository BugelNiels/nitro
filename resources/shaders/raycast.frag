#version 410
// Fragment shader

// Defined in util.glsl
vec3 phongShading(vec3 matcolour, vec3 coords, vec3 normal);
vec3 normalShading(vec3 normal);

layout(location = 0) in vec3 vertcoords_vs;

out vec4 fColor;

uniform mat4 modelviewmatrix;
uniform mat4 projectionmatrix;
uniform vec3 boundp1;
uniform vec3 boundp2;
uniform vec3 boundp3;
uniform vec3 boundp4;
uniform float boxheight;

// Normal calculation of quadric
vec3 surfaceNormal(vec3 p) {
  return vec3(1, 0, 0);
}

// Ray-quadric intersection
bool intersectSurface(vec3 ro, vec3 rd, out float t0, out float t1) {
  t0 = t1 = 1;
  return true;
}

// Manually calculate the frag depth, since the geometry we get from the vertex
// shader is a square directly in front of the camera.
void setFragDepth(vec3 pos) {
  vec4 tpoint = projectionmatrix * vec4(pos, 1.0);
  gl_FragDepth = (tpoint.z / tpoint.w + 1.0) * 0.5 - 0.001;
}

// Determines whether a position is inside the bounding box
bool insideboundingBox(vec3 pos) {
  vec3 i = boundp2 - boundp1;
  vec3 j = boundp3 - boundp1;
  vec3 k = boundp4 - boundp1;
  vec3 v = pos - boundp1;

  float vi = dot(v, i);
  float vj = dot(v, j);
  float vk = dot(v, k);
  float ii = dot(i, i);
  float jj = dot(j, j);
  float kk = dot(k, k);
  return vi > 0 && vi < ii && vj > 0 && vj < jj && vk > 0 && vk < kk;
}

// Produce color based on position of the quadric
vec4 shade(vec3 rd, vec3 hitPos) {
  vec3 normal = surfaceNormal(hitPos);
  vec3 color;

  if (dot(normal, rd) > 0) {
    normal *= -1;
    // Shade inside of quadric slightly darker
    color *= 0.5;
  }
  setFragDepth(hitPos);
  return vec4(phongShading(color, hitPos, normal), 1.0);
}

// Generate color based on where the ray hits the surface. Discards the fragment
// if the ray does not hit.
vec4 surfaceColor(vec3 ro, vec3 rd) {
  float t0, t1;
  if (intersectSurface(ro, rd, t0, t1)) {
    vec3 hitPos = ro + t0 * rd;
    if (insideboundingBox(hitPos)) {
      return shade(rd, hitPos);
    }
    hitPos = ro + t1 * rd;
    if (insideboundingBox(hitPos)) {
      return shade(rd, hitPos);
    }
  }
  discard;
}

/**
 * Raycaster. Shoots rays in every pixel of the screen and calculates the
 * intersection with the quadric.
 */
void main() {
  // Generate ray
  float aspectRatio = projectionmatrix[1][1] / projectionmatrix[0][0];
  vec2 uv = vertcoords_vs.xy * vec2(aspectRatio, 1.0) / projectionmatrix[1][1];
  vec3 ro = vec3(0.0);
  vec3 rd = normalize(vec3(uv, -1.0));

  fColor = surfaceColor(ro, rd);
}

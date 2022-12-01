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

// TODO, but first do connected component stuff

void main() {
  // Generate ray
  float aspectRatio = projectionmatrix[1][1] / projectionmatrix[0][0];
  vec2 uv = vertcoords_vs.xy * vec2(aspectRatio, 1.0) / projectionmatrix[1][1];
  vec3 ro = vec3(0.0);
  vec3 rd = normalize(vec3(uv, -1.0));

  // TODO: construct 3D voxel grid based on image?

  // TODO: intersect ray with voxel grid
  fColor = vec4(0, 0, 0, 1);
}

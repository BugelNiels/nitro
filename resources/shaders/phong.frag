#version 410
// Fragment shader

layout(location = 0) in vec3 vertcoords_fs;
layout(location = 1) in vec3 vertnormal_fs;

out vec4 fColor;

// Defined in shading.glsl
vec3 phongShading(vec3 matCol, vec3 coords, vec3 normal);

void main() {
  vec3 matcolour = vec3(0.53, 0.80, 0.87);
  vec3 col = phongShading(matcolour, vertcoords_fs, vertnormal_fs);
  fColor = vec4(col, 1.0);
}

#version 410
// Vertex shader

layout(location = 0) in vec3 vertcoords_vs;
layout(location = 1) in vec3 vertnormal_vs;

uniform mat4 modelviewmatrix;
uniform mat4 projectionmatrix;
uniform mat3 normalmatrix;

layout(location = 0) out vec3 vertcoords_fs;
layout(location = 1) out vec3 vertnormal_fs;

void main() {
  gl_Position = projectionmatrix * modelviewmatrix * vec4(vertcoords_vs, 1.0);

  vertcoords_fs = vec3(modelviewmatrix * vec4(vertcoords_vs, 1.0));
  vertnormal_fs = normalize(normalmatrix * vertnormal_vs);
}

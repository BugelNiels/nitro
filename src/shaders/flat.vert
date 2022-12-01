#version 410
// Vertex shader
layout (location = 0) in vec2 vertcoords_world_vs;
layout (location = 1) in vec3 vertcolour_in;

uniform mat4 modelviewmatrix;
uniform mat4 projectionmatrix;

out vec3 vertcolour;

void main() {

  gl_Position = projectionmatrix * modelviewmatrix * vec4(vertcoords_world_vs, -1.0, 1.0);
  vertcolour = vertcolour_in;
}

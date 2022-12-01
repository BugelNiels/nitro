#version 410
// Vertex shader

layout(location = 0) in vec3 vertcoords_world_vs;
layout(location = 0) out vec3 vertcoords_vs;

/**
 * Vertex shader. Coordinates are only those of a quad directly in front of the
 * camera.
 */
void main() {
  gl_Position = vec4(vertcoords_world_vs, 1.0);
  vertcoords_vs = vertcoords_world_vs;
}

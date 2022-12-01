#version 410
// Fragment shader
in vec3 vertcolour;

out vec4 fColor;

void main() {

  fColor = vec4(vertcolour, 1.0);

}

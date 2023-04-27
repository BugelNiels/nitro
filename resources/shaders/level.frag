#version 410
// Fragment shader

layout(location = 0) in vec3 vertcoords_vs;

out vec4 fColor;

uniform mat4 modelviewmatrix;
uniform mat4 projectionmatrix;
uniform sampler2D image;
uniform int depth;

uniform int level;

void main() {
  float aspectRatio = projectionmatrix[1][1] / projectionmatrix[0][0];
  vec2 uv = vertcoords_vs.xy * vec2(aspectRatio, 1.0) / projectionmatrix[1][1];
  uv = (vertcoords_vs.xy + 1) / 2.0;

  vec4 val = texture(image, uv) * depth;
  vec3 col = vec3(0);
  if(val.r >= level) {
    col = vec3(1);
  }

  fColor = vec4(col, 1.0);
}

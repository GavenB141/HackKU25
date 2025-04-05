#version 330 core

in vec4 fragColor;
in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D texture0;

void main () {
  vec4 color = texture(texture0, fragTexCoord);
  if (color == vec4(0.0f, 0.0f, 0.0f, 1.0f)) {
    finalColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
  } else {
    finalColor = color;
  }
}

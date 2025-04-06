#version 100

precision mediump float;

varying vec4 fragColor;
varying vec2 fragTexCoord;

uniform sampler2D texture0;

void main () {
  vec4 color = texture2D(texture0, fragTexCoord);
  if (color == vec4(0.0, 0.0, 0.0, 1.0)) {
    gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);
  } else {
    gl_FragColor = color;
  }
}

#version 100

precision mediump float;

varying vec4 fragColor;
varying vec2 fragTexCoord;

uniform sampler2D texture0;

void main () {
  gl_FragColor = vec4(fragTexCoord, 0.0, 1.0);
}

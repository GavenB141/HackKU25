#version 100

precision mediump float;

varying vec4 fragColor;
varying vec2 fragTexCoord;

uniform sampler2D texture0;

void main () {
  gl_FragColor = texture2D(texture0, mod(fragTexCoord, 1.0)) * fragColor;
}

varying vec3 f_color;

void main(void) {
  gl_FragColor = vec4(f_color.x, f_color.y, f_color.z * floor(
    mod(sqrt(pow(gl_FragCoord.x, 3) +
    pow(gl_FragCoord.y, 3)), 9.0)), 1.0);
}

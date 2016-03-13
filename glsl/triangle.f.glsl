void main(void) {
  gl_FragColor[0] = gl_FragCoord.x / 1000.0;
  gl_FragColor[1] = gl_FragCoord.y / 900.0;
  gl_FragColor[2] = 0.5;
  gl_FragColor[3] =
    floor(
      mod(sqrt(pow(gl_FragCoord.x, 3) +
      pow(gl_FragCoord.y, 3)), 9.0)
    ) * 3;
};

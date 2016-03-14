#ifdef GL_ES
#  ifdef GL_FRAGMENT_PRECISION_HIGH
     precision highp float;
#  else
     precision mediump float;
#  endif
#else
// Ignore unsupported precision specifiers
#  define lowp
#  define mediump
#  define highp
#endif

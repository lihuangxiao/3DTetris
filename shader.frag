#version 330

uniform vec3 frag_color;

out vec4 finalColor;


void main()
{
    if (frag_color[1] == 0.8f) {
      finalColor = vec4(1.0,1.0,1.0,0.0);
    } else if (frag_color[1] == 0.81f) {
      finalColor = vec4(0.0,0.0,0.0,0.0);
    } else if (frag_color[1] == 0.82f) {
      finalColor = vec4(0.0,0.0,1.0,0.0);
    } else if (frag_color[1] == 0.83f) {
      finalColor = vec4(0.0,1.0,0.0,0.0);
    } else if (frag_color[1] == 0.84f) {
      finalColor = vec4(0.0,1.0,1.0,0.0);
    } else if (frag_color[1] == 0.85f) {
      finalColor = vec4(1.0,1.0,0.0,0.0);
    }  else {
      finalColor = vec4(frag_color, 0.0);
    }
}

#version 330\n \n
	  \n
	  smooth in vec3 color; \n
          \n
          out vec4 frag_color; \n
          \n
          void main(void) \n
          { \n
             frag_color = vec4(color.rgb, 1.0); \n
          } \n


#version 330
          
          in vec3 color;

          //Output
          layout (location = 0) out vec4 fcolor;


          void main(void)
          { 
          	fcolor = vec4(color, 1.0);
          } 

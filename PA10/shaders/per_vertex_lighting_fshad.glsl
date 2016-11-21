#version 330
          
          in vec4 color;

          //Output
          layout (location = 0) out vec4 fcolor;

          uniform sampler2D gSampler;


          void main(void)
          { 
          	fcolor = color;
          } 

#version 330
          
          in vec3 color;
          in vec2 v_texture;

          //Output
          layout (location = 0) out vec4 fcolor;

          uniform sampler2D gSampler;


          void main(void)
          { 
          	fcolor = vec4(color, 1.0) + texture2D(gSampler, v_texture.xy);
          } 

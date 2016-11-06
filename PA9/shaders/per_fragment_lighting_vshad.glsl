#version 330
          
          layout (location = 0) in vec4 v_position; 
          layout (location = 1) in vec3 normal; 

          uniform mat4 projectionMatrix; 
          uniform mat4 viewMatrix; 
          uniform mat4 modelMatrix;

          out VS_OUT{
            vec3 N;  //normal
            vec3 L;  //light source
            vec3 V;  //view
          } vs_out;

          //light position
          uniform vec3 light_pos = vec3(100.0, 100.0, 100.0);
          
          void main(void) 
          { 

            //view space coordinate
            vec4 p = modelMatrix * normal;

            // Calculate normal in view space
            vs_out.N = mat3(modelMatrix) * normal;

            //Caluclate light vector
            vs_out.L = light_pos - p.xyz;

            //calculate view vector
            vs_out.V = -p.xyz;

            gl_Position = projectionMatrix * p; 
          } 
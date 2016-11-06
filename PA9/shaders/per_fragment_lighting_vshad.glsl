#version 330
          
          layout (location = 0) in vec4 v_position; 
          layout (location = 1) in vec3 normal;
          layout (location = 2) in vec3 diffuse;


          uniform mat4 projectionMatrix; 
          uniform mat4 viewMatrix; 
          uniform mat4 modelMatrix;
          uniform vec3 scalar;

          out VS_OUT{
            vec3 N;  //normal
            vec3 L;  //light source
            vec3 V;  //view
            vec3 diffuse; //color
            vec3 scalar;  //scalar
          } vs_out;

          //light position
          uniform vec3 light_pos = vec3(0.0, 50.0, -50.0);
          uniform vec3 spotlight_pos = vec3(0.0, 10.0, 0.0);
          
          void main(void) 
          { 

            //view space coordinate
            vec4 p = modelMatrix * v_position;

            // Calculate normal in view space
            vs_out.N = mat3(modelMatrix) * normal;

            //Caluclate light vector
            vs_out.L = light_pos - p.xyz;

            //calculate view vector
            vs_out.V = -p.xyz;

            gl_Position = projectionMatrix * viewMatrix * p;

            vs_out.diffuse = diffuse; 
            vs_out.scalar = scalar;
          } 
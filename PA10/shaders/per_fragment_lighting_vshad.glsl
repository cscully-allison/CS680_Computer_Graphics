#version 330
          
          layout (location = 0) in vec4 position;
          layout (location = 1) in vec3 normal;
          layout (location = 2) in vec3 Kd; 
          layout (location = 3) in vec3 Ka;
          layout (location = 4) in vec3 Ks;
          layout (location = 5) in vec3 emissive;
          layout (location = 6) in vec3 transparent;
          layout (location = 7) in vec3 ballPosition;


          uniform mat4 projectionMatrix; 
          uniform mat4 viewMatrix; 
          uniform mat4 modelMatrix;
          uniform vec4 ball;  

          out VS_OUT{
            vec3 N;  //normal
            vec3 L;  //light source
            vec3 V;  //view

            // material properties
            vec3 Ka;
            vec3 Kd;
            vec3 Ks;
            vec3 e;
            vec3 t;

            // spotlight
            vec3 spotlightL;
            vec3 spotlightV;
          } vs_out;

          //light position
          vec3 light_pos = vec3(0, 10.0, 5.0);
          vec3 spotlight_pos = vec3(ball.x, 1.0, ball.z);
          
          void main(void) 
          { 
            //view space coordinate
            vec4 p = modelMatrix * position;

            // Calculate normal in view space
            vs_out.N = mat3(modelMatrix) * normal;

            //Caluclate light vector
            vs_out.L = light_pos - p.xyz;

            //calculate view vector
            vs_out.V = -p.xyz;

            vs_out.spotlightL = spotlight_pos - p.xyz;
            vs_out.spotlightV = -ball.xyz;

            gl_Position = projectionMatrix * viewMatrix * p;

            // pass other values to the fragment shader
            vs_out.Ks = Ks;
            vs_out.Ka = Ka;
            vs_out.Kd = Kd;
            vs_out.e = emissive;
            vs_out.t = transparent;
          } 

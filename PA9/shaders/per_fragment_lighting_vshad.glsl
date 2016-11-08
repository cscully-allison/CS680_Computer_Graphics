#version 330
          
          layout (location = 0) in vec4 v_position; 
          layout (location = 1) in vec3 normal;
          layout (location = 2) in vec3 diffuse;
          layout (location = 3) in vec3 Ka;
          layout (location = 4) in vec3 Kd;
          layout (location = 5) in vec3 Ks;

          uniform mat4 projectionMatrix; 
          uniform mat4 viewMatrix; 
          uniform mat4 modelMatrix;
          uniform vec3 scalar;
          uniform vec3 spec;
           uniform vec4 ballPosition;

          out VS_OUT{
            vec3 N;  //normal
            vec3 L;  //light source
            vec3 spotL;
            vec3 V;  //view
            vec3 diffuse; //color
            vec3 scalar;  //scalar
            vec3 spec;  //scalar
            vec3 Ka;
            vec3 Kd;
            vec3 Ks;
          } vs_out;

          //light position
          uniform vec3 light_pos = vec3(0.0, 50.0, -50.0);
          vec3 spotlight_pos = vec3(ballPosition.x + 2, 2.0, ballPosition.z + 2);
          
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
            
         

            //Caluclate light vector
            vs_out.spotL = spotlight_pos - p.xyz;


            gl_Position = projectionMatrix * viewMatrix * p;

            vs_out.diffuse = diffuse; 
            vs_out.scalar = scalar;
            vs_out.spec = spec;
            vs_out.Ks = Ks;
            vs_out.Ka = Ka;
            vs_out.Kd = Kd;
          } 

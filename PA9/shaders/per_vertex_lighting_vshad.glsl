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


          //light position
          uniform vec3 light_pos = vec3(0.0, 50.0, -50.0);
          vec3 spotlight_pos = vec3 (ballPosition.x, 2.0, ballPosition.z);

          //material properties
          uniform float specular_power = 16.0;
          
          // scalar to change ambient
          vec3 ambient = scalar;

          out vec3 color;

          void main(void) 
          { 

            //view space coordinate
            vec4 p = modelMatrix * v_position;

            // Calculate normal in view space
            vec3 N =normalize( mat3(modelMatrix) * normal);

            //Caluclate light vector
            vec3 L = normalize(light_pos - p.xyz);

            //calculate view vector
            vec3 V = normalize(-p.xyz);

            //calculate R locally
            vec3 R = normalize( reflect(-L, N) );

            //compute the diffuse and specular components for each
            //fragments
            vec3 diffuse = diffuse * max(dot(N,L), 0.0);
            
            // multiple here to increase specularity
            vec3 specular = pow(max(dot(R, V), 0.0), specular_power) * spec;

            color = ambient + diffuse + specular;


            gl_Position = projectionMatrix * viewMatrix * p; 
          } 

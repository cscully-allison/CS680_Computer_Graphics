#version 330
          
          layout (location = 0) in vec4 v_position; 
          layout (location = 1) in vec3 normal; 
          layout (location = 2) in vec3 diffuse;

          uniform mat4 projectionMatrix; 
          uniform mat4 viewMatrix; 
          uniform mat4 modelMatrix;
          uniform vec3 scalar;
          uniform vec4 ballPosition;


          //light position
          uniform vec3 light_pos = vec3(0.0, 50.0, -50.0);
          vec3 spotlight_pos = vec3 (ballPosition.x, 2.0, ballPosition.z);

          //material properties
          vec3 diffuse_albedo = diffuse;
          vec3 specular_albedo = scalar;
          uniform float specular_power = 16.0;
          uniform vec3 ambient = vec3(0.1, 0.1, 0.1);

          out vec3 color;

          void main(void) 
          { 

            //view space coordinate
            vec4 p = modelMatrix * v_position;

            // Calculate normal in view space
            vec3 N = mat3(modelMatrix) * normal;

            //Caluclate light vector
            vec3 L = light_pos - p.xyz;

            //calculate view vector
            vec3 V = -p.xyz;

            //normalize the incoming N, L, and V vectors
            N = normalize(N);
            L = normalize(L);
            V = normalize(V);

            //calculate R locally
            vec3 R = reflect(-L, N);

            //compute the diffuse and specular components for each
            //fragments
            vec3 diffuse = max(dot(N,L), 0.0) * diffuse_albedo;
            vec3 specular = pow(max(dot(R, V), 0.0), specular_power) * specular_albedo;

            color = ambient + diffuse + specular;

            // spotlight
            L = normalize (spotlight_pos.xyz);
            V = normalize (- ballPosition.xyz);

            if (dot (L,V) > 2){
               R = reflect(-L, N);
               diffuse = max(dot(N,L), 0.0) * diffuse_albedo;
               specular = pow(max(dot(R, V), 0.0), specular_power) * vec3 (0.0, 0.0, 0.0);

               vec3 colorTemp = ambient + diffuse + specular;
               color += colorTemp;
            }


            gl_Position = projectionMatrix * viewMatrix  * p; 
          } 
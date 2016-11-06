#version 330
          
          layout (location = 0) in vec4 v_position; 
          layout (location = 1) in vec3 normal; 

          uniform mat4 projectionMatrix; 
          uniform mat4 viewMatrix; 
          uniform mat4 modelMatrix;


          //light position
          uniform vec3 light_pos = vec3(100.0, 100.0, 100.0);
          //material properties
          uniform vec3 diffuse_albedo = vec3(0.5, 0.2, 0.7);
          uniform vec3 specular_albedo = vec3(0.7);
          uniform float specular_power = 128.0;
          uniform vec3 ambient = vec3(0.1, 0.1, 0.1);

          smooth out vec3 color;

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
            //fragment
            vec3 diffuse = max(dot(N,L), 0.0) * diffuse_albedo;
            vec3 specular = pow(max(dot(R, V), 0.0), specular_power) * specular_albedo;

            color = ambient + diffuse + specular;


            gl_Position = projectionMatrix * p; 
          } 
#version 330
          
          layout (location = 0) in vec4 position;
          layout (location = 1) in vec3 normal;
          layout (location = 2) in vec2 texture; 
          layout (location = 3) in vec3 Ka;
          layout (location = 4) in vec3 Ks;
          layout (location = 5) in vec3 emissive;
          layout (location = 6) in vec3 transparent;

          uniform mat4 projectionMatrix; 
          uniform mat4 viewMatrix; 
          uniform mat4 modelMatrix;
          uniform vec3 scalar;
          uniform vec3 spec;
          uniform vec3 spot;
          uniform vec4 ballPosition;
          uniform float height;


          //light position
          uniform vec3 light_pos = vec3(0.0, 50.0, -50.0);
          vec3 spotlight_pos = vec3 (ballPosition.x, height, ballPosition.z);

          //material properties
          uniform float specular_power = 16.0;
          
          // scalar to change ambient
          vec3 ambient = scalar;

          out vec3 color;
          out vec2 v_texture;

          void main(void) 
          { 

            //view space coordinate
            vec4 p = modelMatrix * position;

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
            vec3 diffuse = vec3(0.5,0.5,0.5) * max(dot(N,L), 0.0);
            
            // multiple here to increase specularity
            vec3 specular = pow(max(dot(R, V), 0.0), specular_power) * spec;

            color = ambient + diffuse + specular;


            // spotlight

            float NdotL = max (dot(N,L),0.0);
            if ( NdotL > 0.0){    
                L = normalize(spotlight_pos - p.xyz);
                //calculate R locally
                R = normalize( reflect(-L, N) );
                
                float spotLight = dot (L,V);
                if (spotLight < 0.01){
                   // spot = pow (spot, 4);
                   // float dist = length (L);
                    //float att = spot / (1 +1*dist+1*dist*dist);

                    diffuse = diffuse * max(dot(N,L), 0.0);
                    specular = pow(max(dot(R, V), 0.0), 100) * vec3 (1.0);

                     color += spot + diffuse + specular;
                     }
            }

            gl_Position = projectionMatrix * viewMatrix * p;

            v_texture = texture; 
          } 

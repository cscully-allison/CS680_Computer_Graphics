#version 330
          
          //Output
          out vec4 color;

          //Input from vertex shader
          in VS_OUT
          {
          	vec3 N;
          	vec3 L;
          	vec3 V;

            vec3 Ka;
            vec3 Kd;
            vec3 Ks;
            vec3 e;
            vec3 t;

            // spotlight
            vec3 spotlightL;
            vec3 spotlightV;
          } fs_in;


          //material properties
          uniform float specular_power = 128.0;
          
          void main(void)
          { 
            float intensity = 0.0;
            vec3 spec = vec3 (0.0);
          	//normalize the incoming N, L, and V vectors
          	vec3 N = normalize(fs_in.N);
          	vec3 L = normalize(fs_in.L);
          	vec3 V = normalize(fs_in.V);

          	//calculate the halfway
          	vec3 H = normalize (L + V);

          	//compute the diffuse and specular components for each fragment
          	vec3 diffuse = max(dot(N,L), 0.0) * fs_in.Kd;
            diffuse = clamp (diffuse, 0.0,1.0);
          	vec3 specular = pow(max(dot(N, H), 0.0),  specular_power) * fs_in.Ks;
            specular = clamp (specular, 0.0, 1.0);

          	color = vec4(fs_in.Ka + diffuse + specular, 1.0);

            //spotlight;
          /*  if (max (dot (N,L), 0.0) > 0.0){
              L = normalize(fs_in.spotlightL);

              V = normalize(fs_in.spotlightV);
               H = normalize (L + V);
              intensity = degrees(acos(dot (L,V)));

              if ( intensity > 20){
                H = normalize (L + V);
                diffuse = max(dot(N,L), 0.0) * fs_in.Kd;
                spec = fs_in.Ks * pow(max(dot(H, V), 0.0), 1.0);
              }
               color += vec4(diffuse, 1.0) + vec4 (specular,1);
            }*/ 
          }

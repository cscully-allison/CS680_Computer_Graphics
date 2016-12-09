#version 330
          
          //Output
          out vec4 color;

          //Input from vertex shader
          in VS_OUT
          {
          	vec3 N;
          	vec3 L;
          	vec3 V;
            //vec3 L2;

            vec3 Ka;
            vec2 texture;
            vec3 Ks;

            vec3 diffuse;
            vec3 scalar;
            vec3 spec;  //scalar
            vec3 spot;
            vec3 light_color;

          } fs_in;


          //material properties
          uniform float specular_power = 128.0;
          uniform sampler2D gSampler;

          void main(void)
          { 
          	//normalize the incoming N, L, and V vectors
          	vec3 N = normalize(fs_in.N);
          	vec3 L = normalize(fs_in.L);
          	vec3 V = normalize(fs_in.V);

          	//calculate the halfway
          	vec3 R = normalize( reflect(-L, N) );    

            vec4 texel = texture2D(gSampler, fs_in.texture.xy); 

            vec3 diffuse = texel.rgb * fs_in.light_color * max(dot(N,L), 0.0);

          	//compute the diffuse and specular components for each fragment;
          	vec3 specular = pow(max(dot(R,V), 0.0),  specular_power) * fs_in.spec;
            color = vec4(fs_in.scalar + diffuse, 1.0) + vec4 (specular,1);

            //vec3 L2 = normalize(fs_in.L2);

            //calculate the halfway
            //R = normalize( reflect(-L2, N) );    
            //diffuse = texel.rgb * fs_in.light_color.rgb * max(dot(N,L2), 0.0);

            //compute the diffuse and specular components for each fragment;
            specular = pow(max(dot(R,V), 0.0),  specular_power) * fs_in.spec;
            color += vec4(fs_in.scalar + diffuse, 1.0) + vec4 (specular,1);

            


/*              float NdotL = max (dot(N,L),0.0);
                if ( NdotL > 0.0){
                   L = normalize(fs_in.spotlightL);
                   R = normalize( reflect(-L, N) );
                   float spotLight = dot (L,V);
                   if (spotLight < 0.01){
                      V = normalize(fs_in.spotlightV);
                      diffuse = max(dot(N,L), 0.0) *  fs_in.diffuse;
                      specular = pow(max(dot(R, V), 0.0), 100.0) *  vec3 (1.0);
                      color += vec4(fs_in.spot + diffuse, 1.0) + vec4 (specular,1);
                   }
              }*/

          }

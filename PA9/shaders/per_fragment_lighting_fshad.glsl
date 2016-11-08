#version 330
          
          //Output
          out vec4 color;

          //Input from vertex shader
          in VS_OUT
          {
          	vec3 N;
          	vec3 L;
          	vec3 spotL;
          	vec3 V;
                vec3 diffuse;
                vec3 scalar;
                vec3 spec;  //scalar
                vec3 spot;
                vec3 Ka;
                vec3 Kd;
                vec3 Ks;
          } fs_in;


          //material properties
          vec3 diffuse_albedo = fs_in.diffuse;
          uniform vec3 specular_albedo = vec3(0.7);
          uniform float specular_power = 2.0;
          vec3 ambient = fs_in.scalar; 
          vec3 spotlightAmbient = vec3(0.3);
          vec3 spotlightSpec = vec3 (0.4);

          void main(void)
          { 
          	//normalize the incoming N, L, and V vectors
          	vec3 N = normalize(fs_in.N);
          	vec3 L = normalize(fs_in.L);
          	vec3 V = normalize(fs_in.V);

          	//calculate R locally
          	vec3 R = normalize(reflect(-L, N));

          	//compute the diffuse and specular components for each fragment
          	vec3 diffuse = max(dot(N,L), 0.0) * diffuse_albedo;
          	vec3 specular = pow(max(dot(R, V), 0.0), specular_power) * fs_in.spec;

          	color = vec4(ambient + diffuse, 1.0) + vec4 (specular,1);
          	
          	// spotlight

                float NdotL = max (dot(N,L),0.0);
                if ( NdotL > 0.0){
                       L = normalize(fs_in.spotL);
          	       //calculate R locally
          	       R = normalize(reflect(-L, N));
                       
                       float spotLight = dot (L,V);
                       if (spotLight < 0.01){
                                //spot = pow (spot, 4);
                                //float dist = length (L);
                                //float att = spot / (20*dist+20*dist*dist);
                                
          	                //compute the diffuse and specular components for each fragment
          	                diffuse = max(dot(N,L), 0.0) * diffuse_albedo;
          	                specular = pow(max(dot(R, V), 0.0), 1.0) * spotlightSpec;

          	                color += vec4(fs_in.spot + diffuse, 1.0) + vec4 (specular,1);
          	       }
          	}
          } 

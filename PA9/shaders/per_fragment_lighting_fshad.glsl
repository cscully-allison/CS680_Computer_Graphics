#version 330
          
          //Output
          layout (location = 0) out vec4 color;

          //Input from vertex shader
          in VS_OUT
          {
          	vec3 N;
          	vec3 L;
          	vec3 V;
               vec3 diffuse;
               vec3 scalar;
          } fs_in;


          //material properties
          vec3 diffuse_albedo = fs_in.diffuse;
          uniform vec3 specular_albedo = vec3(0.7);
          uniform float specular_power = 2.0;
          vec3 ambient= fs_in.scalar; 

          void main(void)
          { 
          	//normalize the incoming N, L, and V vectors
          	vec3 N = normalize(fs_in.N);
          	vec3 L = normalize(fs_in.L);
          	vec3 V = normalize(fs_in.V);

          	//calculate R locally
          	vec3 R = reflect(-L, N);

          	//compute the diffuse and specular components for each fragment
          	vec3 diffuse = max(dot(N,L), 0.0) * diffuse_albedo;
          	vec3 specular = pow(max(dot(R, V), 0.0), specular_power) * specular_albedo;

          	color = vec4(ambient + diffuse, 1.0) + vec4 (specular,1);
          } 

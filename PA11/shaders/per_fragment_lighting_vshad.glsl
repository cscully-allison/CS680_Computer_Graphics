#version 330
          
          layout (location = 0) in vec4 position;
          layout (location = 1) in vec3 normal;
          layout (location = 2) in vec2 texture; 
          layout (location = 3) in vec3 Ka;
          layout (location = 4) in vec3 Ks;

          uniform mat4 projectionMatrix; 
          uniform mat4 viewMatrix; 
          uniform mat4 modelMatrix;
          
          uniform vec4 ballPosition;
          uniform vec3 diffuse;
          uniform vec3 scalar;
          uniform vec3 spec;
          uniform vec3 spot;
          uniform float height;  

          out VS_OUT{
            vec3 N;  //normal
            vec3 L;  //light source
            vec3 V;  //view
            vec3 L2;  //2nd light source

            // material properties
            vec3 Ka;
            vec2 texture;
            vec3 Ks;

            vec3 diffuse;
            vec3 scalar;
            vec3 spec;  //scalar
            vec3 spot;            

            // spotlight
            vec3 spotlightL;
            vec3 spotlightV;
          } vs_out;

          //light position
          vec3 light_pos = vec3(0, 10.0, 5.0);
          vec3 spotlight_pos = vec3(ballPosition.x, height, ballPosition.z);
          vec3 light_pos2 = vec3(0, 0.0, 0.0);

          void main(void) 
          { 
            //view space coordinate
            vec4 p = modelMatrix * position;

            // Calculate normal in view space
            vs_out.N = mat3(modelMatrix) * normal;

            //Caluclate light vector
            vs_out.L = light_pos - p.xyz;

            //Caluclate 2nd light vector
            vs_out.L2 = light_pos2 - p.xyz;

            //calculate view vector
            vs_out.V = -p.xyz;

            vs_out.spotlightL = spotlight_pos - p.xyz;
            vs_out.spotlightV = -ballPosition.xyz;

            gl_Position = projectionMatrix * viewMatrix * p;

            // pass other values to the fragment shader
            vs_out.Ks = Ks;
            vs_out.Ka = Ka;
            vs_out.texture = texture;

            vs_out.diffuse = diffuse; 
            vs_out.scalar = scalar;
            vs_out.spec = spec;
            vs_out.spot = spot;
          } 

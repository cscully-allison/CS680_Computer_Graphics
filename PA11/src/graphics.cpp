#include "graphics.h"
#include <glm/gtx/matrix_decompose.hpp>
#include <math.h>
#include <time.h>

#define PI 3.14159265

Graphics::Graphics()
{
  // create physics world objects
  broadphase = new btDbvtBroadphase();
  collisionConfig = new btDefaultCollisionConfiguration();
  dispatcher = new btCollisionDispatcher(collisionConfig);
  solver = new btSequentialImpulseConstraintSolver;
  dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);
  dynamicsWorld->setGravity(btVector3(0,-1,0));

  // set inital game variables
  score = 0;
  gamestate = true;

}

Graphics::~Graphics()
{
    // delete physics world objects
    delete dynamicsWorld;
    delete solver;
    delete dispatcher;
    delete collisionConfig;
    delete broadphase;
}

bool Graphics::getGameState()
{
  // returns if the player has run out of balls
  return gamestate;
}

bool Graphics::Initialize(int width, int height)
{

  // Used for the linux OS
  #if !defined(__APPLE__) && !defined(MACOSX)
    // cout << glewGetString(GLEW_VERSION) << endl;
    glewExperimental = GL_TRUE;

    auto status = glewInit();

    // This is here to grab the error that comes from glew init.
    // This error is an GL_INVALID_ENUM that has no effects on the performance
    glGetError();

    //Check for error
    if (status != GLEW_OK)
    {
      std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
      return false;
    }
  #endif

  srand(time(NULL));

  // For OpenGL 3
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);
  glEnable(GL_LIGHTING);

  // Init Camera
  m_camera = new Camera();
  if(!m_camera->Initialize(width, height))
  {
    printf("Camera Failed to Initialize\n");
    return false;
  }

  // Create the object
  m_land = new Object ("ground.obj", .8, 0, 0, 0, btVector3(0.0,0.0,0.0));
  m_sky =  new Object ("skybox_bigger.obj", 0, 0, 0, 0, btVector3(0.0,0.0,0.0));
  int negate;
  //make terrian objects
  for(int object = 0; object < 20; object++){
      float x =(float) ((rand() % 10)+1)*55;
      negate = rand() % 2;
      if (negate == 0)
      {
        x *=-1;
      }
      float z =(float) ((rand() % 10)+1)*55;
      negate = rand() % 2;
      if (negate == 0)
      {
        z *=-1;
      }
      float y =(float) (rand() % 5);

      negate = rand()%2;

      if (negate == 0)
      {
      terrian_objects[object] =  new Object("rockthing.obj", 0, 0, 0, 0, btVector3(x,y,z));
      }
      else
      {
       terrian_objects[object] =  new Object("pyramid.obj", 0, 0, 0, 0, btVector3(x,5,z)); 
      }
      dynamicsWorld->addRigidBody(terrian_objects[object]->GetRigidBody());
      terrian_objects[object]->setOrientation();
  }

  dynamicsWorld->addRigidBody (m_land->GetRigidBody());
  dynamicsWorld->addRigidBody (m_sky->GetRigidBody());
  



  //bldg = new Object("bldg.obj", 0, 0, 0, 0);
  //dynamicsWorld->addRigidBody(bldg->GetRigidBody());

  // start AI
  m_AI = new TankAI(dynamicsWorld);

  // start user
  m_user = new UserTank();
  dynamicsWorld->addRigidBody (m_user->GetBase()->GetRigidBody());

  // start health
  m_health = new Health();

  
  // // Initalize the Gouraund Shader
  // gouraund_shader = new Shader();
  // if(!gouraund_shader->Initialize())
  // {
  //   printf("Shader Failed to Initialize\n");
  //   return false;
  // }

  // // Add the vertex shader to the Gouraund Shader
  // if(!gouraund_shader->AddShader(GL_VERTEX_SHADER, "per_vertex_lighting_vshad.glsl"))
  // {
  //   printf("Vertex Shader failed to Initialize\n");
  //   return false;
  // }

  // // Add the fragment shader to the Gouraund Shader
  // if(!gouraund_shader->AddShader(GL_FRAGMENT_SHADER, "per_vertex_lighting_fshad.glsl"))
  // {
  //   printf("Fragment Shader failed to Initialize\n");
  //   return false;
  // }

  // // Connect the Gouraund Shader to the program
  // if(!gouraund_shader->Finalize())
  // {
  //   printf("Program to Finalize\n");
  //   return false;
  // }
  
  // Initialize the Phong Shader
  phong_shader = new Shader();
  if(!phong_shader->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }

  // Add the vertex shader to the Phong Shader
  if(!phong_shader->AddShader(GL_VERTEX_SHADER, "per_fragment_lighting_vshad.glsl"))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader to the Phong Shader
  if(!phong_shader->AddShader(GL_FRAGMENT_SHADER, "per_fragment_lighting_fshad.glsl"))
  {
    printf("Fragment Shader failed to Initialize\n");
    return false;
  }

  // Connect the Phong Shader to the program
  if(!phong_shader->Finalize())
  {
    printf("Program to Finalize\n");
    return false;
  }


  // Enable Phong Shader as inital shader
  phong_shader->Enable();

  // Locate the projection matrix in the shader
  m_projectionMatrix = phong_shader->GetUniformLocation("projectionMatrix");
  if (m_projectionMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_projectionMatrix not found\n");
    return false;
  }

  // Locate the view matrix in the shader
  m_viewMatrix = phong_shader->GetUniformLocation("viewMatrix");
  if (m_viewMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_viewMatrix not found\n");
    return false;
  }

  // Locate the model matrix in the shader
  m_modelMatrix = phong_shader->GetUniformLocation("modelMatrix");
  if (m_modelMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_modelMatrix not found\n");
    return false;
  }


  //Locate the scalar variable in the Phong Shader
  scalar.location = phong_shader->GetUniformLocation("scalar");
  if (scalar.location == INVALID_UNIFORM_LOCATION)
  {
    printf("scalar not found\n");
    return false;
  }
  
  //Locate the height variable in the Phong Shader
   spotlightHeight.location = phong_shader->GetUniformLocation("height");
  if (spotlightHeight.location == INVALID_UNIFORM_LOCATION)
  {
    printf("spotlight height not found\n");
    return false;
  }
  
  //Locate the spot variable in the Phong Shader
  spotlight.location = phong_shader->GetUniformLocation("spot");
  if (spotlight.location == INVALID_UNIFORM_LOCATION)
  {
    printf("spotlight not found\n");
    return false;
  }
  
  //Locate the spec variable in the Phong Shader
  specularity.location = phong_shader->GetUniformLocation("spec");
  if (specularity.location == INVALID_UNIFORM_LOCATION)
  {
    printf("m_spec not found\n");
    return false;
  }

  eyePos.location = phong_shader->GetUniformLocation("eyePos");
  if (eyePos.location == INVALID_UNIFORM_LOCATION)
  {
    printf("eyePos not found\n");
    return false;
  }

  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  return true;
}


void Graphics::Update(unsigned int dt, std::vector <unsigned int> keyPress, int mouseMovement, int launch)
{ 
  //things used for camera positioning
  float camRotation;
  glm::mat4 transformation;
  glm::vec3 scale;
  glm::quat rotation;
  glm::vec3 translation;
  glm::vec3 skew;
  glm::vec4 perspective;

  //displacement from camera for forwards vector
  float xDisplace;
  float zDisplace;

  // update the dynamics world step
  dynamicsWorld->stepSimulation(btScalar(dt), btScalar(5));
  // senses any collision; returned variable not used
  collisionDetection(dt);

  /*m_AI->UpdateWrapper(dt, dynamicsWorld);
  m_user->Update(keyPress, mouseMovement, launch, dynamicsWorld, dt, xDisplace, zDisplace, userPos);
  m_health->Update (dynamicsWorld, dt);
*/


  //get data from model position of tank
  transformation = glm::translate(m_user->GetBase()->GetModel(), glm::vec3(0.0f, 0.0f, 0.0f));
  glm::vec4 userPos = transformation * glm::vec4 (1.0,1.0,1.0,1.0);


  //breaks down the transform into each respected variables values
  glm::decompose(transformation, scale, rotation, translation, skew,perspective);

  //decompose returns a non-conjugated quaternion
  rotation = glm::conjugate(rotation);

    //extract x,y and z rotation from quaternion
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
        float qw = rotation.w;
        float qx = rotation.x;
        float qy = rotation.y;
        float qz = rotation.z;
        float qw2 = qw*qw;
        float qx2 = qx*qx;
        float qy2 = qy*qy;
        float qz2 = qz*qz;
        float test= qx*qy + qz*qw;

        //y rotation in radians
        float h = atan2(2*qy*qw-2*qx*qz,1-2*qy2-2*qz2);

        //z rotation in radians
        float a = asin(2*qx*qy+2*qz*qw);

        //x in radians
        float b = atan2(2*qx*qw-2*qy*qz,1-2*qx2-2*qz2);

        //convert y roation into degrees
        h = h * 180 / PI;


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //if the value of the y rotation is negative, we move it to the corresponding position with a positive value
  if (h<0)
  {
    h +=360;
  }

  //calculates forwards vector with an x and z displacement
  xDisplace = cos((h/180)*PI)*8 * (-1);
  zDisplace = sin((h/180)*PI)*8;


  glm::vec3 forwardsVec (xDisplace, userPos.y+5 ,zDisplace);


  //default camera position and point to look
  glm::vec4 tankPos = m_user->getPosition();
  m_camera->lookAt(glm::vec3(userPos.x+xDisplace, userPos.y+5, userPos.z+zDisplace), glm::vec3( userPos.x, userPos.y+5, userPos.z));


  // start updates
  m_AI->UpdateWrapper(dt, m_user->getPosition(), dynamicsWorld);
  m_user->Update(keyPress, mouseMovement, launch, dynamicsWorld, dt, forwardsVec, rotation);
  m_health->Update (dynamicsWorld, dt);
  // check if user is out of lives
  if (m_user->GetLives() <= 0){
    gamestate = false;
  }

}

void Graphics::collisionDetection (unsigned int dt){
  // 0 = land/skybox
  // 1-4 = associated AI
  // 5 = user
  // 6 = health pack
  // 11-15 the projectiles of the associated tank - 10

  // cycle through manifolds in the dynamics world
  for (int i = 0; i < dynamicsWorld->getDispatcher()->getNumManifolds(); i++) {
    btPersistentManifold* contactManifold =  dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
    // find 2 objects that are colliding
    const btCollisionObject* collisionObject = contactManifold->getBody1();
    const btCollisionObject* collisionObject2 = contactManifold->getBody0();
    
    // cycle through contact points of the objects    
    for (int j = 0; j < contactManifold->getNumContacts(); j++) { 
      //std::cout << collisionObject->getUserIndex() << " " << collisionObject2->getUserIndex() << std::endl;
      // if the objects involved with the collision are not the table 
      if (
          collisionObject->getUserIndex() != -1 && collisionObject2->getUserIndex() != -1 && 
          collisionObject->getUserIndex() != collisionObject2->getUserIndex())
      {
        //std::cout << collisionObject->getUserIndex() << " " << collisionObject2->getUserIndex() << std::endl;
        btManifoldPoint& pt = contactManifold->getContactPoint(j);
        // and collided
        if (pt.getDistance() < 0.01f ){
          // if health pack
          if (collisionObject->getUserIndex() == 6){
            // do things to the health pack
            m_health->Collision(dynamicsWorld);
            // add health if AI
            if (collisionObject2->getUserIndex() < 5){
              m_AI->AddHealth(m_AI->GetTank (collisionObject2->getUserIndex()));
            }
            // add health if user
            else{
              m_user->AddHealth ();
            }
               
          }

          // if projectile collision
          else if (collisionObject2->getUserIndex() < 5 && collisionObject2->getUserIndex() > 0 || collisionObject2->getUserIndex() > 10){
            // figure out which object was which
            int tankProjectile = max (collisionObject->getUserIndex(), collisionObject2->getUserIndex());
            int tankOrGround = min (collisionObject->getUserIndex(), collisionObject2->getUserIndex());
            // if AI shot projectile
            if (tankProjectile < 15){
              //AIstuff
              m_AI->ProjectileHit(dynamicsWorld, tankOrGround, tankProjectile);
              // user hit
              if (tankOrGround == 5){
               m_user->Hit();
              }
              else{
                m_AI->Hit (dynamicsWorld, tankOrGround);
              }
            }
            // if user shot projectile
            else {         
              score += m_user->ProjectileHit (dynamicsWorld, tankOrGround);
              m_AI->Hit (dynamicsWorld, tankOrGround);
            }
          }
        }

      }

    }

  }

}


void Graphics::Render(vector <unsigned int>  keyPress)
{
  //clear the screen
  glClearColor(0.0, 0.0, 0.2, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //get the eye position per rendered frame
  eyePos.value = m_camera->GetPosition();

for (int i =0; i < keyPress.size(); i++){
      // //g or default  
      // if(keyPress[i] == 103 ){
      //   // Start the correct program
      //   gouraund_shader->Enable();
      //     // Locate the projection matrix in the shader
      // m_projectionMatrix = gouraund_shader->GetUniformLocation("projectionMatrix");
      // // Locate the view matrix in the shader
      // m_viewMatrix = gouraund_shader->GetUniformLocation("viewMatrix");
      // // Locate the model matrix in the shader
      // m_modelMatrix = gouraund_shader->GetUniformLocation("modelMatrix");
      // //Locate the scalar in the shader
      // scalar.location = gouraund_shader->GetUniformLocation("scalar");
      // spotlightHeight.location = gouraund_shader->GetUniformLocation("height");
      // spotlight.location = gouraund_shader->GetUniformLocation("spot");
      // specularity.location = gouraund_shader->GetUniformLocation("spec");

      // }
      // //keyboard input p
      // else if(keyPress[i] == 112){
      // phong_shader->Enable();
      //  m_projectionMatrix = phong_shader->GetUniformLocation("projectionMatrix");
      // // Locate the view matrix in the shader
      // m_viewMatrix = phong_shader->GetUniformLocation("viewMatrix");
      // // Locate the model matrix in the shader
      // m_modelMatrix = phong_shader->GetUniformLocation("modelMatrix");
      // //Locate the scalar in the shader
      // scalar.location = phong_shader->GetUniformLocation("scalar");
      // spotlightHeight.location = phong_shader->GetUniformLocation("height");
      // spotlight.location = phong_shader->GetUniformLocation("spot");
      // specularity.location = phong_shader->GetUniformLocation("spec");
      // }

      //numpad + ambient lighting
      if(keyPress[i] == 1073741911 && scalar.value.x < 10.0){
        scalar.value += glm::vec3(0.01);
      }
      // numpad - ambient lighting
      else if(keyPress[i] == 1073741910 && scalar.value.x > 0){
        scalar.value -= glm::vec3(0.01);
      }
      
      // numpad * spotlight ambient
       else if(keyPress[i] == 1073741909 && spotlight.value.x < 10.0){
        spotlight.value += glm::vec3(0.01);
      }
      
      // numpad / spotlight ambient
      else if(keyPress[i] == 1073741908 && spotlight.value.x > 0){
        spotlight.value -= glm::vec3(0.01);
      }
      
      //numpad 6 spotlight height
      else if (keyPress[i] == 1073741918 && spotlightHeight.value.x > 1){
          spotlightHeight.value -= glm::vec3(0.01);
        }
        
      // numpad 9 spotlight height 
      else if (keyPress[i]== 1073741921 && spotlightHeight.value.x < 20){
          spotlightHeight.value += glm::vec3(0.01);
      }
       //numpad 0 + for land
        if (keyPress[i] == 1073741922){
          m_land->setSpec (glm::vec3(0.1));
        }
        
        //numpad . - for land 
        else if (keyPress[i]== 1073741923){
          m_land->setSpec (glm::vec3(-0.1));
        }
       //numpad 1 + for bumper
       else if (keyPress[i] == 1073741913){
          m_sky->setSpec (glm::vec3(0.1));
        }
        //numpad 2 - for bumper
       else if (keyPress[i] == 1073741914){
          m_sky->setSpec (glm::vec3(-0.1));
        }
       
       //numpad 4 + for user tank
       else if (keyPress[i] == 1073741916){
          m_user->setSpec (glm::vec3(0.1));
        } 
        
        //numpad 5 - for user tank
        else if (keyPress[i] == 1073741917){
          m_user->setSpec (glm::vec3(-0.1));
        }   
       
       //numpad 7 + for enemy tank
        else if (keyPress[i] == 1073741919){
          m_AI->setSpec (glm::vec3(0.1));
        } 
        
         //numpad 8 - for enemy tank
        else if (keyPress[i] == 1073741920){
          m_AI->setSpec (glm::vec3(-0.1));
        } 
  }

  // Send in the projection and view to the shader
  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection())); 
  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView())); 

  // render land
  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_land->GetModel()));
  m_land->Render(scalar, specularity, eyePos);

  //render sky
  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_sky->GetModel()));
  m_sky->Render(scalar, specularity, eyePos);

  //render thang
  for(int object = 0; object < 20; object++){
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(terrian_objects[object]->GetModel()));
    terrian_objects[object]->Render(scalar, specularity, eyePos);
  }



  m_AI->RenderWrapper(GetModelMatrix(),scalar, specularity, spotlight, spotlightHeight, eyePos);
  m_user->Render(GetModelMatrix(),scalar, specularity, spotlight, spotlightHeight, eyePos);
  m_health->Render (GetModelMatrix(),scalar, specularity, spotlight, spotlightHeight, eyePos);


  //bldg->Render(scalar, specularity, spotlight, spotlightHeight);

  // Render the table object
  
  //Get any errors from OpenGL
  auto error = glGetError();
  if ( error != GL_NO_ERROR )
  {
    string val = ErrorString( error );
    std::cout<< "Error initializing OpenGL! " << error << ", " << val << std::endl;
  }
}

GLint Graphics::GetModelMatrix(){
  return m_modelMatrix;
}

int Graphics::getScore()
{
  return score;
}

std::string Graphics::ErrorString(GLenum error)
{
  if(error == GL_INVALID_ENUM)
  {
    return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
  }

  else if(error == GL_INVALID_VALUE)
  {
    return "GL_INVALID_VALUE: A numeric argument is out of range.";
  }

  else if(error == GL_INVALID_OPERATION)
  {
    return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
  }

  else if(error == GL_INVALID_FRAMEBUFFER_OPERATION)
  {
    return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
  }

  else if(error == GL_OUT_OF_MEMORY)
  {
    return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
  }
  else
  {
    return "None";
  }
}


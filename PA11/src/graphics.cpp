#include "graphics.h"

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
  m_land = new Object ("ground.obj", .8, 0, 0, 0);
  m_sky =  new Object ("skybox.obj", 0, 0, 0, 0);

  dynamicsWorld->addRigidBody (m_land->GetRigidBody());
  dynamicsWorld->addRigidBody (m_sky->GetRigidBody());

  //bldg = new Object("bldg.obj", 0, 0, 0, 0);
  //dynamicsWorld->addRigidBody(bldg->GetRigidBody());


  m_AI = new TankAI(dynamicsWorld);
  m_user = new UserTank();
  dynamicsWorld->addRigidBody (m_user->GetBase()->GetRigidBody());
  dynamicsWorld->addRigidBody (m_user->GetHead()->GetRigidBody());

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

  //default camera position and point to look
  m_camera->lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 5.0f));

  //once tank is loaded and available with movement
  //m_camera->lookAt(glm::vec3(pos.x, pos.y, pos.z), glm::vec3(tankpos.x, tankpos.y, tankpos.z));

  // update the dynamics world step
  dynamicsWorld->stepSimulation(btScalar(dt), btScalar(5));
  // senses any collision; returned variable not used
  collisionDetection(dt);

  m_AI->UpdateWrapper(dt);
  m_user->Update(keyPress, mouseMovement, launch, dynamicsWorld);
  m_health->Update (dynamicsWorld, dt);
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
      // if the objects involved with the collision are not the table 
      if (
          collisionObject->getUserIndex() != -1 && collisionObject2->getUserIndex() != -1 && 
          collisionObject->getUserIndex() != collisionObject2->getUserIndex())
      {
        //std::cout << collisionObject->getUserIndex() << " " << collisionObject2->getUserIndex() << std::endl;
        btManifoldPoint& pt = contactManifold->getContactPoint(j);
        // and collided
        if (pt.getDistance() < 0.01f ){
          if (collisionObject->getUserIndex() == 6){
            m_health->Collision(dynamicsWorld);
            if (collisionObject2->getUserIndex() < 5){
              m_AI->AddHealth(m_AI->GetTank (collisionObject2->getUserIndex()));
            }
            else{
              m_user->AddHealth ();
            }
               
          }
          else if (collisionObject2->getUserIndex() > 10 || collisionObject2->getUserIndex() > 10){
            int tankProjectile = max (collisionObject->getUserIndex(), collisionObject2->getUserIndex());
            int tankOrGround = min (collisionObject->getUserIndex(), collisionObject2->getUserIndex());
            if (tankProjectile < 15){
              //AIstuff
            }
            else {
              score = m_user->ProjectileHit (dynamicsWorld, tankOrGround);
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
  }

  // Send in the projection and view to the shader
  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection())); 
  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView())); 

  // render land
  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_land->GetModel()));
  m_land->Render(scalar, specularity, spotlight, spotlightHeight, eyePos);

  //render sky
  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_sky->GetModel()));
  m_sky->Render(scalar, specularity, spotlight, spotlightHeight, eyePos);

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


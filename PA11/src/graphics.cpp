#include "graphics.h"

Graphics::Graphics()
{
  // create physics world objects
  broadphase = new btDbvtBroadphase();
  collisionConfig = new btDefaultCollisionConfiguration();
  dispatcher = new btCollisionDispatcher(collisionConfig);
  solver = new btSequentialImpulseConstraintSolver;
  dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);
  dynamicsWorld->setGravity(btVector3(-.333,-1,0));

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
  m_land = new Object ("ground.obj", 0, 0, 0, 0);

 //create the ball and add to dynamics world
  // Initalize the Gouraund Shader
  gouraund_shader = new Shader();
  if(!gouraund_shader->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }

  // Add the vertex shader to the Gouraund Shader
  if(!gouraund_shader->AddShader(GL_VERTEX_SHADER, "per_vertex_lighting_vshad.glsl"))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader to the Gouraund Shader
  if(!gouraund_shader->AddShader(GL_FRAGMENT_SHADER, "per_vertex_lighting_fshad.glsl"))
  {
    printf("Fragment Shader failed to Initialize\n");
    return false;
  }

  // Connect the Gouraund Shader to the program
  if(!gouraund_shader->Finalize())
  {
    printf("Program to Finalize\n");
    return false;
  }
  
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
  
  // locates the ball position in the Phong Shader
  ball = phong_shader->GetUniformLocation("ballPosition");
  if (ball == INVALID_UNIFORM_LOCATION)
  {
    printf("ball not found\n");
    return false;
  }

  //Locate the scalar variable in the Phong Shader
  m_scalar = phong_shader->GetUniformLocation("scalar");
  if (m_scalar == INVALID_UNIFORM_LOCATION)
  {
    printf("m_scalar not found\n");
    return false;
  }
  
  //Locate the height variable in the Phong Shader
   m_height = phong_shader->GetUniformLocation("height");
  if (m_height == INVALID_UNIFORM_LOCATION)
  {
    printf("m_height not found\n");
    return false;
  }
  
  //Locate the spot variable in the Phong Shader
  m_spot = phong_shader->GetUniformLocation("spot");
  if (m_spot == INVALID_UNIFORM_LOCATION)
  {
    printf("m_spot not found\n");
    return false;
  }
  
  //Locate the spec variable in the Phong Shader
  m_spec = phong_shader->GetUniformLocation("spec");
  if (m_spec == INVALID_UNIFORM_LOCATION)
  {
    printf("m_spec not found\n");
    return false;
  }

  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  return true;
}


void Graphics::Update(unsigned int dt, std::vector <unsigned int> keyPress, int force)
{ 

  // update the dynamics world step
  dynamicsWorld->stepSimulation(btScalar(dt), btScalar(5));
  // senses any collision; returned variable not used
  collisionDetection(dt);

}

void Graphics::collisionDetection (unsigned int dt){

  // cycle through manifolds in the dynamics world
  for (int i = 0; i < dynamicsWorld->getDispatcher()->getNumManifolds(); i++) {
    btPersistentManifold* contactManifold =  dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
    // find 2 objects that are colliding
    const btCollisionObject* collisionObject = contactManifold->getBody1();
    const btCollisionObject* collisionObject2 = contactManifold->getBody0();
    
    // cycle through contact points of the objects    
    for (int j = 0; j < contactManifold->getNumContacts(); j++) {
      btManifoldPoint& pt = contactManifold->getContactPoint(j);
      // if the objects involved with the collision are not the table 
      if (collisionObject->getUserIndex() != 0 && collisionObject2->getUserIndex() != 0 && collisionObject->getUserIndex() != -1 && collisionObject2->getUserIndex() != -1){
        // and collided
        if (pt.getDistance() < 0.1f ){
          // find the object that is not the ball; the minimum user index
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

for (int i =0; i < keyPress.size(); i++){
      //g or default  
      if(keyPress[i] == 103 ){
        // Start the correct program
        gouraund_shader->Enable();
          // Locate the projection matrix in the shader
      m_projectionMatrix = gouraund_shader->GetUniformLocation("projectionMatrix");
      // Locate the view matrix in the shader
      m_viewMatrix = gouraund_shader->GetUniformLocation("viewMatrix");
      // Locate the model matrix in the shader
      m_modelMatrix = gouraund_shader->GetUniformLocation("modelMatrix");
      //Locate the scalar in the shader
      m_scalar = gouraund_shader->GetUniformLocation("scalar");
      m_height = gouraund_shader->GetUniformLocation("height");
      m_spot = gouraund_shader->GetUniformLocation("spot");
      m_spec = gouraund_shader->GetUniformLocation("spec");
      ball = gouraund_shader->GetUniformLocation("ballPosition");

      }
      //keyboard input p
      else if(keyPress[i] == 112){
      phong_shader->Enable();
       m_projectionMatrix = phong_shader->GetUniformLocation("projectionMatrix");
      // Locate the view matrix in the shader
      m_viewMatrix = phong_shader->GetUniformLocation("viewMatrix");
      // Locate the model matrix in the shader
      m_modelMatrix = phong_shader->GetUniformLocation("modelMatrix");
      //Locate the scalar in the shader
      m_scalar = phong_shader->GetUniformLocation("scalar");
      m_height = phong_shader->GetUniformLocation("height");
      m_spot = phong_shader->GetUniformLocation("spot");
      m_spec = phong_shader->GetUniformLocation("spec");
      ball = phong_shader->GetUniformLocation("ballPosition");
      }

      //numpad + ambient lighting
      if(keyPress[i] == 1073741911 && scalar.x < 10.0){
        scalar += glm::vec3(0.01);
      }
      // numpad - ambient lighting
      else if(keyPress[i] == 1073741910 && scalar.x > 0){
        scalar -= glm::vec3(0.01);
      }
      
      // numpad * spotlight ambient
       else if(keyPress[i] == 1073741909 && spot.x < 10.0){
        spot += 0.01;
      }
      
      // numpad / spotlight ambient
      else if(keyPress[i] == 1073741908 && spot.x > 0){
        spot -= 0.01;
      }
      
      //numpad 6 spotlight height
      else if (keyPress[i] == 1073741918 && height > 1){
          height -=.1;
        }
        
      // numpad 9 spotlight height 
      else if (keyPress[i]== 1073741921 && height < 20){
          height +=.1;
        }
      
  }

  // Send in the projection and view to the shader
  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection())); 
  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView())); 

  // Render the table object
  
  // Get any errors from OpenGL
  auto error = glGetError();
  if ( error != GL_NO_ERROR )
  {
    string val = ErrorString( error );
    std::cout<< "Error initializing OpenGL! " << error << ", " << val << std::endl;
  }
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


#include "graphics.h"

Graphics::Graphics()
{
  broadphase = new btDbvtBroadphase();
  collisionConfig = new btDefaultCollisionConfiguration();
  dispatcher = new btCollisionDispatcher(collisionConfig);
  solver = new btSequentialImpulseConstraintSolver;
  dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);
  dynamicsWorld->setGravity(btVector3(0,-1,0));

}

Graphics::~Graphics()
{

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

  // Init Camera
  m_camera = new Camera();
  if(!m_camera->Initialize(width, height))
  {
    printf("Camera Failed to Initialize\n");
    return false;
  }

  // Create the object
  m_table = new Object("table.obj", " ");
  m_cylinder = new Object("cylinder.obj", " ");
  m_cube = new Object("box.obj", "box");
  m_ball = new Object("ball.obj", "sphere");
  m_cylinder->setCylinder();
  //m_cube->setOrientation(-1);

  //variables for making planes
  btScalar mass(0);
  btVector3 inertia(0,0,0);
  btDefaultMotionState* motionFloor = new btDefaultMotionState(btTransform(btQuaternion(btScalar(0),btScalar(0),btScalar(0),btScalar(1)), btVector3(btScalar(0),btScalar(0),btScalar(0))));
  btDefaultMotionState* motionSouth = new btDefaultMotionState(btTransform(btQuaternion(btScalar(0),btScalar(0),btScalar(0),btScalar(1)), btVector3(btScalar(0),btScalar(2),btScalar(-5))));
  btDefaultMotionState* motionNorth = new btDefaultMotionState(btTransform(btQuaternion(btScalar(0),btScalar(0),btScalar(0),btScalar(1)), btVector3(btScalar(0),btScalar(2),btScalar(5))));
  btDefaultMotionState* motionEast = new btDefaultMotionState(btTransform(btQuaternion(btScalar(0),btScalar(0),btScalar(0),btScalar(1)), btVector3(btScalar(-5),btScalar(2),btScalar(0))));
  btDefaultMotionState* motionWest = new btDefaultMotionState(btTransform(btQuaternion(btScalar(0),btScalar(0),btScalar(0),btScalar(1)), btVector3(btScalar(5),btScalar(2),btScalar(0))));
  btDefaultMotionState* cylinderPos = new btDefaultMotionState(btTransform(btQuaternion(btScalar(0),btScalar(0),btScalar(0),btScalar(1)), btVector3(btScalar(3.0f), btScalar(0.0f), btScalar(-3.0f))));

  //set cylinder
  cylinder = new btCylinderShape(btVector3(btScalar(0.5f), btScalar(0.5f), btScalar(0.5f)));
  cylinder->calculateLocalInertia(mass, inertia);
  btRigidBody::btRigidBodyConstructionInfo cylinderBodyCI(btScalar(0),cylinderPos, cylinder, inertia);
  cylinderBodyCI.m_friction = btScalar(0);
  cylinderBodyCI.m_restitution = (btScalar(1.0));
  cylinderBody = new btRigidBody(cylinderBodyCI);
  cylinderBody->setActivationState(DISABLE_DEACTIVATION);
  dynamicsWorld->addRigidBody(cylinderBody);


  //makes floor 
  ground = new btStaticPlaneShape(btVector3(btScalar(0),btScalar(1),btScalar(0)), btScalar(0));
  ground->calculateLocalInertia(mass, inertia);
  btRigidBody::btRigidBodyConstructionInfo groundBodyCI(btScalar(0), motionFloor, ground, inertia);  
  groundBodyCI.m_friction = btScalar(0.5);
  groundBodyCI.m_restitution = btScalar(1);
  groundBody = new btRigidBody(groundBodyCI);
  groundBody->setActivationState(DISABLE_DEACTIVATION);
  dynamicsWorld->addRigidBody(groundBody);

  //adds south wall
  southWall = new btBoxShape(btVector3(btScalar(4),btScalar(40),btScalar(1)));
  southWall->calculateLocalInertia(mass, inertia);
  btRigidBody::btRigidBodyConstructionInfo southWallBodyCI(btScalar(0), motionSouth, southWall, inertia); 
  southWallBodyCI.m_restitution = btScalar(1); 
  southWallBody = new btRigidBody(southWallBodyCI);
  southWallBody->setActivationState(DISABLE_DEACTIVATION);
  dynamicsWorld->addRigidBody(southWallBody);

  //north wall
  northWall = new btBoxShape(btVector3(btScalar(4),btScalar(40),btScalar(1)));
  northWall->calculateLocalInertia(mass, inertia);
  btRigidBody::btRigidBodyConstructionInfo northWallBodyCI(0, motionNorth, northWall, inertia);
  northWallBodyCI.m_restitution = btScalar(1);  
  northWallBody = new btRigidBody(northWallBodyCI);
  northWallBody->setActivationState(DISABLE_DEACTIVATION);
  dynamicsWorld->addRigidBody(northWallBody);


  //east wall
  eastWall = new btBoxShape(btVector3(btScalar(1),btScalar(40),btScalar(4)));
  eastWall->calculateLocalInertia(mass, inertia);
  btRigidBody::btRigidBodyConstructionInfo eastWallBodyCI(0, motionEast, eastWall, inertia); 
  eastWallBodyCI.m_restitution = btScalar(1); 
  eastWallBody = new btRigidBody(eastWallBodyCI);
  eastWallBody->setActivationState(DISABLE_DEACTIVATION);  
  dynamicsWorld->addRigidBody(eastWallBody);

  //west wall
  westWall = new btBoxShape(btVector3(btScalar(1),btScalar(40),btScalar(4)));
  westWall->calculateLocalInertia(mass, inertia);
  btRigidBody::btRigidBodyConstructionInfo westWallBodyCI(0, motionWest, westWall, inertia);
  westWallBodyCI.m_restitution = btScalar(1);  
  westWallBody = new btRigidBody(westWallBodyCI);
  westWallBody->setActivationState(DISABLE_DEACTIVATION);  
  dynamicsWorld->addRigidBody(westWallBody);


  dynamicsWorld->addRigidBody(m_ball->getRigidBody());
  dynamicsWorld->addRigidBody(m_cube->getRigidBody());    

  // Set up the shaders
  m_shader = new Shader();
  if(!m_shader->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }

  // Add the vertex shader
  if(!m_shader->AddShader(GL_VERTEX_SHADER))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!m_shader->AddShader(GL_FRAGMENT_SHADER))
  {
    printf("Fragment Shader failed to Initialize\n");
    return false;
  }

  // Connect the program
  if(!m_shader->Finalize())
  {
    printf("Program to Finalize\n");
    return false;
  }

  // Locate the projection matrix in the shader
  m_projectionMatrix = m_shader->GetUniformLocation("projectionMatrix");
  if (m_projectionMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_projectionMatrix not found\n");
    return false;
  }

  // Locate the view matrix in the shader
  m_viewMatrix = m_shader->GetUniformLocation("viewMatrix");
  if (m_viewMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_viewMatrix not found\n");
    return false;
  }

  // Locate the model matrix in the shader
  m_modelMatrix = m_shader->GetUniformLocation("modelMatrix");
  if (m_modelMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_modelMatrix not found\n");
    return false;
  }

  m_table->setOrientation(1);
  m_ball->setPos(glm::vec3(2.0f, 0.0f, 2.0f));
  m_cube->setPos(glm::vec3(0.0f,0.0f,0.0f));

  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  return true;
}

void Graphics::Update(unsigned int dt, float mouseX, float mouseY)
{
  dynamicsWorld->stepSimulation(btScalar(dt), btScalar(5));
  // Update the object
  m_ball->Update(dt, dynamicsWorld);
  //std::cout << m_ball->getRigidBody()->getCenterOfMassTransform().getOrigin().getX() << " " << m_cube->getRigidBody()->getCenterOfMassTransform().getOrigin().getX() << std::endl; 
 
  m_cube->UpdateMouse (dt,dynamicsWorld, mouseX, mouseY);
}


void Graphics::Render()
{
  //clear the screen
  glClearColor(0.0, 0.0, 0.2, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Start the correct program
  m_shader->Enable();

  // Send in the projection and view to the shader
  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection())); 
  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView())); 

  // Render the object
  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_table->GetModel()));
  m_table->Render();

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ball->GetModel()));
  m_ball->Render();

    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_cylinder->GetModel()));
  m_cylinder->Render();

    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_cube->GetModel()));
  m_cube->Render();

  // Get any errors from OpenGL
  auto error = glGetError();
  if ( error != GL_NO_ERROR )
  {
    string val = ErrorString( error );
    std::cout<< "Error initializing OpenGL! " << error << ", " << val << std::endl;
  }
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


#include "graphics.h"

Graphics::Graphics()
{
  broadphase = new btDbvtBroadphase();
  collisionConfig = new btDefaultCollisionConfiguration();
  dispatcher = new btCollisionDispatcher(collisionConfig);
  solver = new btSequentialImpulseConstraintSolver;
  dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);
  dynamicsWorld->setGravity(btVector3(-.333,-1,0));
  ballsLeft = 3;
  score = 0;
  resetable = true;
  ballCleared = false;
  gamestate = true;

}

Graphics::~Graphics()
{
    delete dynamicsWorld;
    delete solver;
    delete dispatcher;
    delete collisionConfig;
    delete broadphase;
}

bool Graphics::getGameState()
{
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
  glEnable(GL_DEPTH_TEST);
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
  // index 1 for table
  m_table = new Object("texturethings/textured_pinball_table.obj",btVector3 (0,0,0),0, .5, 0, 0);

  // add collision shape
  dynamicsWorld->addRigidBody (m_table->GetRigidBody());

  m_bump1 = new Object("bumper.obj", btVector3(4, 0.5, 4), 0, .5, 0, 3);
  dynamicsWorld->addRigidBody(m_bump1->GetRigidBody());
  m_bump1->setOrientation();
  m_bump1->GetRigidBody()->setRestitution(1.5);

  m_bump2 = new Object("bumper.obj", btVector3(4, 0.5, -4), 0, .5, 0, 4);
  dynamicsWorld->addRigidBody(m_bump2->GetRigidBody());
  m_bump2->setOrientation();
  m_bump2->GetRigidBody()->setRestitution(1.5);

  m_bump3 = new Object("bumper.obj", btVector3(0, .5, 0), 0, .5, 0, 5);
  dynamicsWorld->addRigidBody(m_bump3->GetRigidBody());
  m_bump3->setOrientation();
  m_bump3->GetRigidBody()->setRestitution(1.5);

  m_leftFlipper = new Object("fancy_leftflipper.obj",500, btVector3 (0,0,0),btVector3 (-10.8,0,-3.3),0, 0, 0, 1);
  dynamicsWorld->addRigidBody (m_leftFlipper->GetRigidBody());
  //m_leftFlipper->GetRigidBody()->setGravity(btVector3(-5,1,0));
  m_leftFlipper->setOrientation();
  m_leftFlipper->GetRigidBody()->setLinearFactor(btVector3(0,0,0));
  m_leftFlipper->GetRigidBody()->setAngularFactor(btVector3(0,1,0));

  m_rightFlipper = new Object("fancy_rightflipper.obj",500, btVector3 (0,0,0),btVector3 (-10.8,0,2.5),0, 0, 0, 2);
  dynamicsWorld->addRigidBody (m_rightFlipper->GetRigidBody());
  //m_rightFlipper->GetRigidBody()->setGravity(btVector3(-5,1,0));
  m_rightFlipper->setOrientation();
  m_rightFlipper->GetRigidBody()->setLinearFactor(btVector3(0,0,0));
  m_rightFlipper->GetRigidBody()->setAngularFactor(btVector3(0,1,0));

  m_ball = new Object(50, btVector3 (0,0,0), btVector3 (-13,.5,7),0,1, 0);
  dynamicsWorld->addRigidBody (m_ball->GetRigidBody());

  btVector3 inertia(0,0,0);
  btDefaultMotionState* motionCeiling = new btDefaultMotionState(btTransform(btQuaternion(btScalar(0),btScalar(0),btScalar(0),btScalar(1)),
                                                                 btVector3(btScalar(0),btScalar(.55),btScalar(0))));

  btCollisionShape* ceiling = new btStaticPlaneShape(btVector3(0.0f, -1.0f, 0.0f), 0.0f);
  ceiling->calculateLocalInertia(0, inertia);
  btRigidBody::btRigidBodyConstructionInfo ceilingBodyCI(btScalar(0), motionCeiling, ceiling, inertia);  
  ceilingBodyCI.m_friction = btScalar(0);
  ceilingBodyCI.m_restitution = btScalar(0);
  ceilingBody = new btRigidBody(ceilingBodyCI);
  ceilingBody->setActivationState(DISABLE_DEACTIVATION);
  dynamicsWorld->addRigidBody(ceilingBody);

  btDefaultMotionState* motionWall = new btDefaultMotionState(btTransform(btQuaternion(btScalar(0), btScalar(0), btScalar(0), btScalar(1)),
                                                              btVector3(btScalar(0), btScalar(-5), btScalar(6))));
  btCollisionShape* wall = new btBoxShape(btVector3(100.0f, 2.0f, 0.2f));
  wall->calculateLocalInertia(0, inertia);
  btRigidBody::btRigidBodyConstructionInfo wallBodyCI(btScalar(0), motionWall, wall, inertia);
  wallBodyCI.m_friction = btScalar(0);
  wallBodyCI.m_restitution = btScalar(.5);
  wallBody = new btRigidBody(wallBodyCI);
  wallBody->setActivationState(DISABLE_DEACTIVATION);
  dynamicsWorld->addRigidBody(wallBody);


  numbers.push_back (new Object ("number0.obj"));
  numbers.push_back (new Object ("number1.obj"));
  numbers.push_back (new Object ("number2.obj"));
  numbers.push_back (new Object ("number3.obj"));
  numbers.push_back (new Object ("number4.obj"));
  numbers.push_back (new Object ("number5.obj"));
  numbers.push_back (new Object ("number6.obj"));
  numbers.push_back (new Object ("number7.obj"));
  numbers.push_back (new Object ("number8.obj"));
  numbers.push_back (new Object ("number9.obj"));

  //intialize score
  for (int i= 0; i < 5; i++){
    ScoreArray.push_back (numbers[0]);
  }

  BallNum.push_back (numbers[0]);
  BallNum.push_back (numbers[1]);
  BallNum.push_back (numbers[2]);
  BallNum.push_back (numbers[3]);
  
    // Set up the shaders
  m_shader = new Shader();
  if(!m_shader->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }

  // Add the vertex shader
  if(!m_shader->AddShader(GL_VERTEX_SHADER, "per_fragment_lighting_vshad.glsl"))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!m_shader->AddShader(GL_FRAGMENT_SHADER,"per_fragment_lighting_fshad.glsl"))
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
  
  ball = m_shader->GetUniformLocation("ballPosition");
  if (ball == INVALID_UNIFORM_LOCATION)
  {
    printf("ball not found\n");
    return false;
  }

  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  ballCleared = false;
  return true;
}


void Graphics::Update(unsigned int dt, std::vector <unsigned int> keyPress, int force)
{ 
  glm::vec4 pos = m_ball->GetModel() * glm::vec4 (1.0,1.0,1.0,1.0);
  dynamicsWorld->stepSimulation(btScalar(dt), btScalar(5));
  int update = collisionDetection(dt);
  m_ball->Update ();
  int leftCall = 0;
  int rightCall = 0;

  btDefaultMotionState* wallPosActive = new btDefaultMotionState (btTransform(btQuaternion(0,0,0,1), btVector3(0.0f,0.0f,6.0f)));
  btDefaultMotionState* wallPosInactive = new btDefaultMotionState (btTransform(btQuaternion(0,0,0,1), btVector3(0.0f,-5.0f,6.0f)));

  for (int i = 0; i < ScoreArray.size(); i ++){
    ScoreArray[i]->ScoreUpdate(i, score);
  }

  BallNum[ballsLeft]->BallNumUpdate();
 
  if (pos.z > 5.8){
      resetable = true;
  }
  if (pos.x < -11.86  && pos.z < 5.5  && ballCleared && resetable){
    resetable = false;
      reinitateBall ();
  }
  if (pos.z < 6.5){
     ballCleared = true;
     wallBody->setMotionState(wallPosActive);
  }
  else {
    ballCleared = false;
    wallBody->setMotionState(wallPosInactive);
  }

  switch (update){
    case 3:
        //1cout = 1;
   // m_bump1->UpdateBumper(2);
    break;
    case 4:
        //2cout = 1;
    break;
    case 5:
        //3cout = 1;
    break;
  }
     
  for (int i = 0; i < keyPress.size(); i ++){
    if (!ballCleared){
        if (keyPress[i] == 13){
          m_ball->applyForce (min (force, 300));
        }
    }
    if (keyPress[i] == 1073742053)
    {
    rightCall = i;
    }

    if (keyPress[i] == 1073742049)
    {
    leftCall = i;
    }
  }

  if (leftCall != 0)
  {
    m_leftFlipper->UpdateFlipper(0, keyPress[leftCall], dt); 
  }
  else
  {
    m_leftFlipper->UpdateFlipper(0, 0, dt); 
  }

  if (rightCall != 0)
  {
    m_rightFlipper->UpdateFlipper(1, keyPress[rightCall], dt);
  }
  else
  {
    m_rightFlipper->UpdateFlipper(1, 0, dt);
  }

}

int Graphics::collisionDetection (unsigned int dt){
  int notBall;
  for (int i = 0; i < dynamicsWorld->getDispatcher()->getNumManifolds(); i++) {
    btPersistentManifold* contactManifold =  dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
    const btCollisionObject* collisionObject = contactManifold->getBody1();
    const btCollisionObject* collisionObject2 = contactManifold->getBody0();
        
    for (int j = 0; j < contactManifold->getNumContacts(); j++) {
      btManifoldPoint& pt = contactManifold->getContactPoint(j);
      if (collisionObject->getUserIndex() != 0 && collisionObject2->getUserIndex() != 0 && collisionObject->getUserIndex() != -1 && collisionObject2->getUserIndex() != -1){
        if (pt.getDistance() < 0.1f ){
          notBall = min (collisionObject ->getUserIndex(), collisionObject2 ->getUserIndex());
          switch (notBall){
            //left flipper
            // case 1:
            //     score += 50;
            // break;
            // //right flipper
            // case 2:
            //     score += 50;
            break;
            case 3:
                score += 100;
            break;
            case 4:
                score += 100;
            break;
            case 5:
                score += 100;
            break;
         }
        }
      }
    }
  }

  return notBall;
}

void Graphics::reinitateBall(){
  ballsLeft --;
  if (ballsLeft > 0)
  {
    m_ball->GetRigidBody()->proceedToTransform(btTransform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(-13,.5,7)));
    m_ball->GetRigidBody()->setAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));
    m_ball->GetRigidBody()->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
    ballCleared = false;
  }
  else if (ballsLeft == 0)
  {
    std::cout << "game over... your score is: "  << score << std::endl;
    m_ball->GetRigidBody()->proceedToTransform(btTransform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(9999,-9999, 9999)));
    gamestate = false;
  }


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

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_bump1->GetModel()));
  m_bump1->Render();
  
 glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_bump2->GetModel()));
  m_bump2->Render();
  
  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_bump3->GetModel()));
  m_bump3->Render();

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_leftFlipper->GetModel()));
  m_leftFlipper->Render();

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_rightFlipper->GetModel()));
  m_rightFlipper->Render();

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ball->GetModel()));
  pos = m_ball->GetModel() * glm::vec4 (1.0,1.0,1.0,1.0);
  glUniform4fv(ball, 1, glm::value_ptr(pos)); 
  m_ball->Render();

  //render score
  for (int i =0; i < ScoreArray.size(); i++){
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(ScoreArray[i]->GetModel()));
    ScoreArray[i]->Render();
  }

    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(BallNum[ballsLeft]->GetModel()));
    BallNum[ballsLeft]->Render();

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



#include "engine.h"

Engine::Engine(string name, int width, int height)
{
  m_WINDOW_NAME = name;
  m_WINDOW_WIDTH = width;
  m_WINDOW_HEIGHT = height;
  m_FULLSCREEN = false;
  SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1", SDL_HINT_OVERRIDE);
  SDL_SetRelativeMouseMode(SDL_TRUE);
}

Engine::Engine(string name)
{
  m_WINDOW_NAME = name;
  m_WINDOW_HEIGHT = 0;
  m_WINDOW_WIDTH = 0;
  m_FULLSCREEN = true;
  SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1", SDL_HINT_OVERRIDE);
  SDL_SetRelativeMouseMode(SDL_TRUE);
}

Engine::~Engine()
{
  delete m_window;
  delete m_graphics;
  m_window = NULL;
  m_graphics = NULL;
}


bool Engine::Initialize()
{
  // Start a window
  m_window = new Window();
  if(!m_window->Initialize(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT))
  {
    printf("The window failed to initialize.\n");
    return false;
  }


  // Start the graphics
  m_graphics = new Graphics();



  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT))
  {
    printf("The graphics failed to initialize.\n");
    return false;
  }

  // Set the time
  m_currentTimeMillis = GetCurrentTimeMillis();

  // No errors
  return true;
}

void Engine::Run()
{

  m_running = true;
  force = 0;
  while(m_running)
  {
    // Update the DT
    m_DT += getDT();

    // Check the keyboard input
    while(SDL_PollEvent(&m_event) != 0)
    {
      key.push_back(Keyboard());
    }

    // Update and render the graphics
    m_graphics->Update(m_DT, key, force);
    // pass the key to render
    m_graphics->Render(key);

    // look for space being released
    for (int i = 0; i < key.size(); i++ ){
      // if released
       if (key[i] == 32){
          // set force to 0
          force = 0;
          // erase key
          key.erase (key.begin() + i);
      }
    }

    if (m_graphics->getGameState() == false)
    {
      m_running = false;
    }
    // Swap to the Window
    m_window->Swap();
  }

  //gets score from graphics
  int score = m_graphics->getScore();

  //creates input stream variable
  ifstream fin;

  fin.open("../highScores.txt");

  //gets old high score
  for (int i=0; i<10; i++)
  {
    fin >> people[i].initials;
    fin >> people[i].score;
  }

  fin.close();

  int index = 9;


  //checks if new score belongs on board
  if (score > people[9].score)
  {
    while (people[index].score <= score && index != 0)
    {
      index--;
    }

    //finds where score belongs
    for (int i=9; i>index; i--)
    {
      people[i].initials = people[i-1].initials;
      people[i].score = people[i-1].score;
    }

    std::string temp;

    //gets score from user
    std::cout << "INPUT YOUR INITIALS: ";
    std::cin >> temp;

    //assigns score in proper spot
    people[index].initials = temp;
    people[index].score = score;

    //prints new scoreboard to file
    ofstream fout;

    fout.open("../highScores.txt");

    for (int i=0; i<10; i++)
    {
      fout << people[i].initials << " " << people[i].score << std::endl;
    }

    fout.close();

  }

  //prints current high score board
  std::cout << "CURRENT SCORE BOARD:" << std::endl;
  for (int i=1; i<11; i++)
  {
    std::cout << i << ".\t" << people[i-1].initials << "\t-\t" << people[i-1].score << std::endl;
  }



}

unsigned int Engine::Keyboard()
{
  //escape button to quit
  if(m_event.type == SDL_QUIT)
  {
    m_running = false;
  }

  // when a key is pressed down
  else if (m_event.type == SDL_KEYDOWN)
  {
    // handle key down events here
    if (m_event.key.keysym.sym == SDLK_ESCAPE)
    {
      m_running = false;
    }
    // if space
    else if (m_event.key.keysym.sym == 32)
    {
      // add force
      force ++;
    }
    else
    {
      // otherwise, return the key
      return m_event.key.keysym.sym;
    }  
  }

  // when the key is released
  else if (m_event.type == SDL_KEYUP)
  {
    // when 0space is released
    if (m_event.key.keysym.sym == 32)
    {
      // return the space key
      return m_event.key.keysym.sym;
    }
    // otherwise
    else{
      // cycle through keys and delete released key
    for (int i = 0; i < key.size(); i++){
        if (key[i] == m_event.key.keysym.sym){
          key.erase (key.begin()+ i);
        }
    }
  }
  }
}

unsigned int Engine::getDT()
{
  long long TimeNowMillis = GetCurrentTimeMillis();
  assert(TimeNowMillis >= m_currentTimeMillis);
  unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
  m_currentTimeMillis = TimeNowMillis;
  return DeltaTimeMillis;
}

long long Engine::GetCurrentTimeMillis()
{
  timeval t;
  gettimeofday(&t, NULL);
  long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
  return ret;
}

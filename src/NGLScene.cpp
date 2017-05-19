#include <QMouseEvent>
#include <QGuiApplication>
#include <QApplication>
#include "Player.h"
#include "Enemy.h"
#include "Sword.h"
#include "Heart.h"
#include "background.h"
#include "NGLScene.h"
#include <ngl/Camera.h>
#include <ngl/Light.h>
#include <ngl/Transformation.h>
#include <ngl/Material.h>
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <QFont>

NGLScene::NGLScene()
{
  setTitle("CRISIS DEFENDER");
  m_updateShipTimer=startTimer(2);
  m_redrawTimer=startTimer(20);
  // now clear the key set
  m_keysPressed.clear();
}

NGLScene::~NGLScene()
{
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
}

void NGLScene::resizeGL( int _w, int _h )
{
  m_cam.setShape( 45.0f, static_cast<float>( _w ) / _h, 0.05f, 350.0f );
  m_win.width  = static_cast<int>( _w * devicePixelRatio() );
  m_win.height = static_cast<int>( _h * devicePixelRatio() );
}

// Function originally written by Jon Macey
void NGLScene::initializeGL()
{
  // we must call this first before any other GL commands to load and link the
  // gl commands from the lib, if this is not done program will crash
  ngl::NGLInit::instance();
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);
  // now to load the shader and set the values
  // grab an instance of shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  shader->loadShader("Texture", "shaders/TextureVertex.glsl", "shaders/TextureFragment.glsl");
  (*shader)["Texture"]->use();
  ngl::Texture t("images/floor.png");
  t.setTextureGL();

  shader->loadShader("Phong", "shaders/PhongVertex.glsl", "shaders/PhongFragment.glsl");
  (*shader)["Phong"]->use();

  // Now we will create a basic Camera from the graphics library
  // This is a static camera so it only needs to be set once
  // First create Values for the camera position
  ngl::Vec3 from(0,0,-80);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,1,0);

  // now load to our new camera
  m_cam.set(from,to,up);
  // set the shape using FOV 45 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10
  m_cam.setShape(45, (float)720.0/576.0, 0.05, 350);
  shader->setShaderParam3f("viewerPos", m_cam.getEye().m_x, m_cam.getEye().m_y, m_cam.getEye().m_z);
  // now create our light this is done after the camera so we can pass the
  // transpose of the projection matrix to the light to do correct eye space
  // transformations
  ngl::Mat4 iv=m_cam.getViewMatrix();
  iv.transpose();
  ngl::Light light(ngl::Vec3(0,0,2), ngl::Colour(1,1,1,1), ngl::Colour(1,1,1,1), ngl::LightModes::DIRECTIONALLIGHT);
  light.setTransform(iv);
  // load these values to the shader as well
  light.loadToShader("light");
  ngl::Material m(ngl::STDMAT::SILVER);
  m.loadToShader("material");

  // instance all the objects needed in the game
  m_enemy.reset( new Enemy(ngl::Vec3(0,0,0), "models/Enemy.obj"));
  m_ship.reset( new Player(ngl::Vec3(0,0,0), "models/player.obj"));
  m_sword.reset( new Sword(ngl::Vec3(0,0,0), "models/sword.obj"));
  m_heart.reset( new Heart(ngl::Vec3(0,0,0), "models/heart.obj"));
  m_background.reset( new background("images/floor.png", "images/floor.png"));
  m_text.reset(new ngl::Text(QFont("Helvetica", 50)));

  m_text->setScreenSize(width(),height());
  m_text->setColour(1,1,1);

  // read from the high scores file
  std::ifstream InputFile ("scores.txt");
  if(InputFile.is_open())
  {
    // load in the high score from the file
    InputFile >> hscore;
    InputFile.close();
  }
}

void NGLScene::paintGL()
{
  // work out the window size for text rendering
  int sH;
  // read from the config file
  std::ifstream InputFile ("config.txt");
  if(InputFile.is_open())
  {
    // load in the screen resolution
    InputFile >> sH;
    InputFile.close();
  }
  else { sH = 1080; }
  // work out the width for a 16:9 ratio
  int sW = (sH/9)*16;

  // check to see if enter has been pressed to start the game
  if (start == true)
  {
    if (dead==false && pause==false)
    {
      moveUpdate();
    }
    // clear the screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // check cooldown of attack
    if(cdown>0)
    {
      if(cdown>10)
      {
        // sword animation that rotates and swings in front of the player
        m_sword->draw("Phong",&m_cam);
        m_sword->m_rotation+=90/10;
      }
      // reduce cooldown timer
      cdown-=1;
    }

    m_enemy->draw("Phong",&m_cam);

    // check invincibility frame value and count it down
    if (invFrame > 0)
    {
      invFrame-=1;
    }
    // don't draw the player if it's in certain frames of invincibility to produce a flashing effect to convey invincibility to player
    if( (invFrame < 50 && invFrame > 40) || (invFrame < 30 && invFrame > 20) || invFrame < 10)
    {
      m_ship->draw("Phong",&m_cam);
    }

    // check if there's a heart spawned
    if (heartspawn == true)
    {
      // if so, draw it
      m_heart->draw("Phong",&m_cam);
      // then check for collisions with the player
      if (m_heart->collide(m_ship->m_pos) == true)
      {
        // if collides, add health to the player and despawn the heart
        m_ship->health+=1;
        heartspawn = false;
      }
    }

    // draw the background
    m_background->draw(&m_cam);

    // set up the HUD text
    QString text=QString("%1")
        .arg(m_ship->score);
    QString text2=QString("MULTIPLIER! %1")
        .arg(m_ship->mult);
    QString text3=QString("HIGHSCORE! %1")
        .arg(hscore);



    // render the text
    m_text->renderText(sW/2-50,0, text);
    m_text->renderText(sW-520,0, text2);
    m_text->renderText(0,0, text3);

    // if the player has died
    if (m_ship->health <= 0)
    {
      // tell them so
      m_text->renderText(sW/2-300, sH/2-200, "DEAD! PRESS R!");
      // set the dead flag
      dead=true;

      // read the scores file
      std::ifstream InputFile ("scores.txt");
      if(InputFile.is_open())
      {
        InputFile >> hscore;
        InputFile.close();
      }
      // if the new score is better than the saved one...
      if (m_ship->score > hscore)
      {
        // save it to the file (or create the file if it doesn't exist)
        std::ofstream OutputFile ("scores.txt");
        OutputFile << m_ship->score;
        OutputFile.close();
      }
    }
    // pause the game if it's requested
    if (pause==true)
    {
      m_text->renderText(sW/2-300,sH/2-200, "PAUSED");
    }
  }
  else
  {
    // text to render at startup
    m_text->renderText(sW/2-300,sH/2-200, "CRISIS DEFENDER!");
    m_text->renderText(sW/2-300,sH/2-140, "WASD OR ARROW KEYS TO MOVE");
    m_text->renderText(sW/2-300,sH/2-80, "SPACE TO ATTACK");
    m_text->renderText(sW/2-300,sH/2-20, "ENTER TO START");
  }
}

void NGLScene::moveUpdate()
{
  // move the player
  movePlayer();
  // make sure the sword stays with the player
  m_sword->m_pos=m_ship->m_pos;
  // move the enemy based on where the player is
  m_enemy->moveEnemy(m_ship->m_pos);
  // make sure the enemies don't collide
  m_enemy->enemyCollisions();
  // if the player isn't in an invincibility frame
  if (invFrame == 0)
  {
    // check for any collisions of enemy against player
    int knockback = m_enemy->knockbackPlayer(m_ship->m_pos);
    // if there was, move the player according to the direction received, reduce their health, and make them invincible for a bit so they can recover
    switch (knockback)
    {
      case 1:
      {
        m_ship->move(4,4);
        m_ship->health-=1;
        m_ship->mult=1;
        invFrame = 60;
        break;
      }
      case 2:
      {
        m_ship->move(-4,4);
        m_ship->health-=1;
        m_ship->mult=1;
        invFrame = 60;
        break;
      }
      case 3:
      {
        m_ship->move(4,-4);
        m_ship->health-=1;
        m_ship->mult=1;
        invFrame = 60;
        break;
      }
      case 4:
      {
        m_ship->move(-4,-4);
        m_ship->health-=1;
        m_ship->mult=1;
        invFrame = 60;
        break;
      }
    }
  }
}


void NGLScene::timerEvent( QTimerEvent *_event)
{
  // the usual process is to check the event timerID and compare it to
  // any timers we have started with startTimer

  if (_event->timerId() == m_redrawTimer)
  {
    update();
  }

}

void NGLScene::movePlayer()
{
  // first we reset the movement values
  float xDirection=0.0;
  float yDirection=0.0;
  // how far the player moves each frame
  float moveSpeed = 0.3;
  // now we loop for each of the pressed keys in the the set
  // and see which ones have been pressed. If they have been pressed
  // we set the movement value to be an incremental value
  foreach(Qt::Key key, m_keysPressed)
  {
    switch (key)
    {
    case Qt::Key_Left :
    case Qt::Key_A :
    { xDirection=moveSpeed; break;}
    case Qt::Key_Right :
    case Qt::Key_D :
    { xDirection=-moveSpeed; break;}
    case Qt::Key_Up :
    case Qt::Key_W :
    { yDirection=moveSpeed; break;}
    case Qt::Key_Down :
    case Qt::Key_S :
    { yDirection=-moveSpeed; break;}
    default : break;
    }
  }
  // if the set is non zero size we can update the ship movement
  // then tell openGL to re-draw
  if(m_keysPressed.size() !=0)
  {
    m_ship->move(xDirection,yDirection);

  }

}

// function originally written by Jon Macey
//----------------------------------------------------------------------------------------------------------------------
// in this case this method is called from the MainWindow class and passed
// the key event from that, if we didn't need to process the events in main
// window we could just overload this as the keyPressEvent and only process here.
//----------------------------------------------------------------------------------------------------------------------
void NGLScene::keyPressEvent( QKeyEvent *_event)
{
  // add to our keypress set the values of any keys pressed
  m_keysPressed += (Qt::Key)_event->key();

  switch (_event->key())
  {

    case Qt::Key_Escape:
    {
      QApplication::exit(EXIT_SUCCESS); break;
    }
    case Qt::Key_Space:
    {
    // attack button, check if attack cooldown is 0, if so, attack
      if (cdown == 0)
      {
        // method that checks how many enemies have been killed by the attack (and damages them accordingly)
        int hitSuccess =(m_enemy->attack(m_ship->m_pos, m_ship->m_rotation));
        // for every hit
        for(int i=0; i < hitSuccess; i++)
        {
          // increase the score and the multiplier
          m_ship->score+=10*m_ship->mult;
          m_ship->mult+=1;
          // if there's no heart already spawned, try and spawn one
          if (heartspawn == false && m_ship->health < 3)
          {
            if (m_heart->spawn() == true)
            {
              heartspawn = true;
            }
          }
        }
        // start off the rotation of the sword, based on the direction of the player when the attack starts
        m_sword->m_rotation=m_ship->m_rotation-45;
        // set the attack cooldown
        cdown=20;
      }
      break;
    }
    case Qt::Key_P:
    {
      // pause the game
      pause= !pause; break;
    }
    case Qt::Key_R:
    {
      // resets the game after death and loads the enemies back to starting position
      m_enemy.reset( new Enemy(ngl::Vec3(0,0,0),"models/Enemy.obj"));
      // makes the player not dead
      dead=false;
      // resets the score, multiplier and health
      m_ship->score=0;
      m_ship->mult=1;
      m_ship->health=3;
      // makes sure no hearts are spawned immediately
      heartspawn=false;
      break;
    }
    case Qt::Key_Return:
    {
      //starts the game
      start = true;
    }
  }

}

//----------------------------------------------------------------------------------------------------------------------
// in this case this method is called from the MainWindow class and passed
// the key event from that, if we didn't need to process the events in main
// window we could just overload this as the keyPressEvent and only process here.
//----------------------------------------------------------------------------------------------------------------------
void NGLScene::keyReleaseEvent( QKeyEvent *_event	)
{
  // remove from our key set any keys that have been released
  m_keysPressed -= (Qt::Key)_event->key();
}



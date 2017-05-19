/// \file Enemy.h
/// \brief this class inherits from the Qt OpenGLWindow and allows us to use NGL to draw OpenGL
/// \author Jonathan Macey

#ifndef NGLSCENE_H_
#define NGLSCENE_H_
#include "NGLScene.h"
#include "Player.h"
#include "Enemy.h"
#include "Sword.h"
#include "Heart.h"
#include "background.h"
#include <ngl/Camera.h>
#include <ngl/Colour.h>
#include <ngl/Light.h>
#include <ngl/Text.h>
#include "WindowParams.h"
#include <QOpenGLWindow>
#include <QSet>
#include <memory>
#include <ngl/Text.h>

class NGLScene : public QOpenGLWindow
{
public:
  /// @brief ctor for our NGL drawing class
  NGLScene();
  /// @brief dtor must close down ngl and release OpenGL resources
  ~NGLScene();
  /// @brief the initialize class is called once when the window is created and we have a valid GL context
  /// use this to setup any default GL stuff
  void initializeGL();
  /// @brief this is called everytime we want to draw the scene
  void paintGL();
  /// @brief this is called everytime we resize
  void resizeGL(int _w, int _h);

private:
  /// @brief the windows params such as mouse and rotations etc
  WinParams m_win;
  /// @brief Our Camera
  ngl::Camera m_cam;
  /// @brief the model position for mouse movement
  ngl::Vec3 m_modelPos;
  /// @brief a timer triggered by the startTimer call in the ctor this is called
  /// to update the position based on the key presses
  int m_updateShipTimer;
  /// @brief we will draw with a different timer to the update
  int m_redrawTimer;
  /// @brief a pointer to our Player
  std::unique_ptr<Player> m_ship;
  /// @brief a pointer to our enemy
  std::unique_ptr<Enemy> m_enemy;
  /// @brief a pointer to the sword
  std::unique_ptr<Sword> m_sword;
  /// @brief a pointer to the heart pickup
  std::unique_ptr<Heart> m_heart;
  /// @brief a pointer to the background
  std::unique_ptr<background> m_background;
  /// @brief bool for whether the heart has spawned
  bool heartspawn = false;
  /// @brief invincibility frame value for after being hit
  int invFrame = 0;
  /// @brief the keys being pressed
  QSet<Qt::Key> m_keysPressed;
//  /// @brief this method is called every time a mouse is moved
//  /// @param _event the Qt Event structure
//  void mouseMoveEvent (QMouseEvent * _event );
//  /// @brief this method is called everytime the mouse button is pressed
//  /// inherited from QObject and overridden here.
//  /// @param _event the Qt Event structure
//  void mousePressEvent ( QMouseEvent *_event);
//  /// @brief this method is called everytime the mouse button is released
//  /// inherited from QObject and overridden here.
//  /// @param _event the Qt Event structure
//  void mouseReleaseEvent ( QMouseEvent *_event );
//  /// @brief this method is called everytime the mouse wheel is moved
//  /// inherited from QObject and overridden here.
//  /// @param _event the Qt Event structure
//  void wheelEvent( QWheelEvent *_event);
  /// @brief processKeyDown passed from the main window class
  /// @param *_event a pointer to the QKeyEvent passed from main window
  /// class
  void keyPressEvent(QKeyEvent *_event );
  /// @brief processKeyUp passed from the main window class
  /// @param *_event a pointer to the QKeyEvent passed from main window
  /// class
  void keyReleaseEvent(QKeyEvent *_event );
  /// @brief called when the timer triggers used to update and draw
  /// @param *_event a pointer to the timer event id
  void timerEvent( QTimerEvent *_event);
  /// @brief update player based on stored key movements
  void movePlayer();
  /// @brief calculate all the movements for all the objects
  void moveUpdate();
  /// @brief method to calculate knockback of the player from enemies
  void knockbackPlayer();
  /// @brief pointer for text rendering
  std::unique_ptr<ngl::Text> m_text;
  /// @brief bool for whether the game is paused or not
  bool pause = false;
  /// @brief bool for whether the player has died or not
  bool dead = false;
  /// @brief bool for whether the game has been started or not
  bool start = false;
  /// @brief value for attack cooldown
  int cdown = 0;
  /// @brief value for the high score
  int hscore;

};


#endif

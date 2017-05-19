/// \file Enemy.h
/// \brief for the enemies
/// \author Alex Cowell

#ifndef ENEMY_H__
#define ENEMY_H__

#include <ngl/Camera.h>
#include <ngl/Vec3.h>
#include <ngl/Obj.h>
#include <ngl/Transformation.h>

/// @brief struct for enemy data
struct enemies
{
  /// @brief position vector
  ngl::Vec3 m_pos;
  /// @brief a value for enemy health
  int health = 1;
  /// @brief a transform stack
  ngl::Transformation m_transform;
  /// @brief the enemy's rotation
  float m_rotation;
  /// @brief the enemy mesh
  std::unique_ptr<ngl::Obj>m_mesh;
};

class Enemy
{
public :
  /// @brief ctor
  /// @param _pos the initial position
  /// @param _fname the name of mesh to load
  Enemy(ngl::Vec3 _pos, std::string _fname);
  /// @brief draw method
  /// @param _shader the shader to use
  /// @param _cam camera data passed on
  void draw(const std::string &_shader, ngl::Camera *_cam );
  /// @brief movement method
  /// @param spos player's position passed on for chasing
  void moveEnemy(ngl::Vec3 spos);
  /// @brief vector of enemy structs
  std::vector<enemies> m_enemies;
  /// @brief enemy being attacked method
  /// @param spos player position
  /// @param rot player facing direction
  int attack(ngl::Vec3 spos, int rot);
  /// @brief method to work out player/enemy collision and knockback
  /// @param spos player position
  int knockbackPlayer(ngl::Vec3 spos);
  /// @brief enemy collision method
  void enemyCollisions();
  /// @brief movement speed variable
  float speed = 0.1;
private :
  /// @brief variable to represent difficulty increase as progression is made
  int difficulty = 0;
};

#endif

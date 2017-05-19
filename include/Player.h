/// \file Player.h
/// \brief this is for the player character
/// \author Alex Cowell

#ifndef PLAYER_H__
#define PLAYER_H__

#include <ngl/Camera.h>
#include <ngl/Vec3.h>
#include <ngl/Obj.h>
#include <ngl/Transformation.h>

class Player
{
public :
  /// @brief ctor
  /// @param _pos the initial position
  /// @param _fname the name of mesh to load
  Player(ngl::Vec3 _pos, std::string _fname);
  ///@brief draw method
  /// @param _shader the shader to use
  /// @param _cam camera data passed on
  void draw(const std::string &_shader, ngl::Camera *_cam );
  /// @brief the position of the player
  ngl::Vec3 m_pos;
  /// @brief move method
  /// @param _x, _y values to transform with
  void move(float _x,	float _y	);
  /// @brief player health value
  int health = 3;
  /// @brief rotation value
  float m_rotation;
  /// @brief score value
  int score = 0;
  /// @brief multiplier value
  int mult = 1;
private :
  /// @brief the player object's mesh
  std::unique_ptr<ngl::Obj>m_mesh;
  /// @brief a transform stack
  ngl::Transformation m_transform;

};

#endif

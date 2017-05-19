/// \file Sword.h
/// \brief the sword object
/// \author Alex Cowell

#ifndef SWORD_H__
#define SWORD_H__

#include <ngl/Camera.h>
#include <ngl/Vec3.h>
#include <ngl/Obj.h>
#include <ngl/Transformation.h>

class Sword
{
public :
  /// @brief ctor
  /// @param _pos the initial position
  /// @param _fname the name of mesh to load
  Sword(ngl::Vec3 _pos, std::string _fname);
  /// @brief draw method
  /// @param _shader the shader to use
  /// @param _cam camera data passed on
  void draw(const std::string &_shader, ngl::Camera *_cam );
  /// @brief the position of the ship
  ngl::Vec3 m_pos;
  /// @brief move method
  /// @param spos the position of the player
  void move(ngl::Vec3 spos);
  /// @brief the sword's rotation
  float m_rotation;

private :
  /// @brief the ship's mesh
  std::unique_ptr<ngl::Obj>m_mesh;
  /// @brief a transform stack
  ngl::Transformation m_transform;
};

#endif

/// \file Heart.h
/// \brief for the heart health pickup
/// \author Alex Cowell

#ifndef HEART_H__
#define HEART_H__

#include <ngl/Camera.h>
#include <ngl/Vec3.h>
#include <ngl/Obj.h>
#include <ngl/Transformation.h>

class Heart
{
public :
  /// @brief ctor
  /// @param _pos the initial position
  /// @param _fname the name of mesh to load
  Heart(ngl::Vec3 _pos, std::string _fname);
  /// @brief draw method
  /// @param _shader the shader to use
  /// @param _cam camera data passed on
  void draw(const std::string &_shader, ngl::Camera *_cam );
  /// @brief the position of the heart
  ngl::Vec3 m_pos;
  /// @brief collision method
  /// @param spos player position
  bool collide(ngl::Vec3 spos);
  /// @brief method to determine whether it will spawn
  bool spawn();
private :
  /// @brief the heart's mesh
  std::unique_ptr<ngl::Obj>m_mesh;
  /// @brief a transform stack
  ngl::Transformation m_transform;
};

#endif

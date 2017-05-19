/// \file background.h
/// \brief used for the background sphere
/// \author Alex Cowell

#ifndef BACKGROUND_H__
#define BACKGROUND_H__

#include <ngl/Camera.h>
#include <string>

class background
{
public :
  /// @brief ctor
  /// @param _texture/_texture2 the texture to load in
  background(std::string _texture, std::string _texture2);
  /// @brief dtor
  ~background();
  /// @brief draw method
  /// @param _cam camera data
  void draw(ngl::Camera *_cam);
private :
  /// @brief texture ID
  GLuint m_texID;
  /// @brief texture ID
  GLuint m_texID2;
  /// @brief camera pointer
  ngl::Camera *m_camera;
};

#endif

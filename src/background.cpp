#include "background.h"
#include <ngl/VAOPrimitives.h>
#include <ngl/Texture.h>
#include <ngl/ShaderLib.h>
#include <ngl/Transformation.h>

background::background(std::string _texture, std::string _texture2)
{
  // set texture
  ngl::Texture t(_texture);
  m_texID=t.setTextureGL();
  // initialise and create the sphere
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  prim->createSphere("background",300,100);

}

background::~background()
{
  std::cout<<"deleting background";
}

void background::draw(ngl::Camera *_cam)
{
  // initialise shader and primitives
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();

  // set up shader
  glEnable (GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  m_camera=_cam;
  (*shader)["Texture"]->use();
  ngl::Transformation t;
  ngl::Mat4 MVP;
  MVP=t.getMatrix()*m_camera->getVPMatrix();
  shader->setShaderParamFromMat4("MVP",MVP);
  glBindTexture(GL_TEXTURE_2D,m_texID);

  // draw background
  prim->draw("background");
  t.setScale(0.91,0.91,0.91);
  glBindTexture(GL_TEXTURE_2D,m_texID2);
  MVP=t.getMatrix()*m_camera->getVPMatrix();
  shader->setShaderParamFromMat4("MVP",MVP);
  prim->draw("background");
  glDisable(GL_BLEND);

}

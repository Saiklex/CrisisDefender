#include "Heart.h"
#include <ngl/ShaderLib.h>
#include <ngl/Material.h>

Heart::Heart( ngl::Vec3 _pos, std::string _fname )
{
  m_pos=_pos;
  m_mesh.reset( new ngl::Obj(_fname));
  m_mesh->createVAO();
}

void Heart::draw(const std::string &_shader,  ngl::Camera *_cam )
{
  // initialise shader and draw
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)[_shader]->use();
  m_transform.setPosition(m_pos);
  ngl::Mat4 MVP= m_transform.getMatrix() * _cam->getVPMatrix() ;
  shader->setShaderParamFromMat4("MVP",MVP);
  ngl::Material m(ngl::STDMAT::BRONZE);
  m.loadToShader("material");
  m_mesh->draw();
}

bool Heart::spawn()
{
  // generate a random number to use to see whether a heart spawns
  int r = std::rand() % 100;
  // 10% chance of a heart spawning
  if (r > 89)
  {
    return true;
  }
  return false;
}

bool Heart::collide(ngl::Vec3 spos)
{
  // collision detection for the heart and the player
  if (abs(spos.m_x - m_pos.m_x) < 2 && abs(spos.m_y - m_pos.m_y) < 2)
  {
    return true;
  }
  return false;
}

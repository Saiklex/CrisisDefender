#include "Sword.h"
#include <ngl/ShaderLib.h>
#include <ngl/Material.h>

Sword::Sword( ngl::Vec3 _pos, std::string _fname )
 {
   m_pos=_pos;
   m_mesh.reset( new ngl::Obj(_fname));
   m_mesh->createVAO();
   m_rotation=0;
 }

void Sword::draw(const std::string &_shader,  ngl::Camera *_cam )
{
  // initialise shader and draw with it
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)[_shader]->use();
  m_transform.setPosition(m_pos);
  m_transform.setRotation(0,0,m_rotation);
  ngl::Mat4 MVP= m_transform.getMatrix() * _cam->getVPMatrix() ;
  shader->setShaderParamFromMat4("MVP",MVP);
  ngl::Material m(ngl::STDMAT::SILVER);
  m.loadToShader("material");
  m_mesh->draw();
}


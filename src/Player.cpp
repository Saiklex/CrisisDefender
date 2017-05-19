#include "Player.h"
#include <ngl/ShaderLib.h>
#include <ngl/Material.h>

Player::Player( ngl::Vec3 _pos, std::string _fname )
{
  m_pos=_pos;
  m_mesh.reset( new ngl::Obj(_fname));
  m_mesh->createVAO();
  m_rotation=0;
}

void Player::draw(const std::string &_shader,  ngl::Camera *_cam )
{
  // initialise shader to be used
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)[_shader]->use();
  m_transform.setPosition(m_pos);
  m_transform.setRotation(0,0,m_rotation);
  ngl::Mat4 MVP= m_transform.getMatrix() * _cam->getVPMatrix() ;
  shader->setShaderParamFromMat4("MVP",MVP);

  // change colour based on how much health the player has
  switch (health)
  {
  case 3 : {ngl::Material m(ngl::STDMAT::SILVER); m.loadToShader("material");} break;
  case 2 : {ngl::Material m(ngl::STDMAT::GOLD); m.loadToShader("material");} break;
  case 1 : {ngl::Material m(ngl::STDMAT::COPPER); m.loadToShader("material");} break;
  }

  m_mesh->draw();

  // load in silver (default material)
  ngl::Material m(ngl::STDMAT::SILVER);
  m.loadToShader("material");
}

// boundaries for the screen
const static float s_xExtents=60;
const static float s_yExtents=35;

void Player::move( float _x,float _y )
{
  // set the rotation to match the player direction
  if(_x>0){m_rotation=270;}
  if(_x<0){m_rotation=90;}
  if(_y>0){m_rotation=0;}
  if(_y<0){m_rotation=180;}

  // store the current position
  float currentX=m_pos.m_x;
  float currentY=m_pos.m_y;
  // add the requested movement by the player
  m_pos.m_x+=_x;
  m_pos.m_y+=_y;

  // check whether the player has tried to move too far off screen, if so, negate their movement in that direction
  if(m_pos.m_x<=-s_xExtents || m_pos.m_x>=s_xExtents)
  {
    m_pos.m_x=currentX;
  }
  if(m_pos.m_y<=-s_yExtents || m_pos.m_y>=s_yExtents)
  {
    m_pos.m_y=currentY;
  }
}

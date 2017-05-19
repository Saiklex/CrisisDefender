#include "Enemy.h"
#include <ngl/ShaderLib.h>
#include <ngl/Material.h>

Enemy::Enemy( ngl::Vec3 _pos, std::string _fname )
{
  // the amount of enemies to be present at any time
  m_enemies.resize(10);

  // set up every enemy
  for(auto &e : m_enemies)
  {
    e.m_mesh.reset( new ngl::Obj(_fname));
    e.m_mesh->createVAO();
    // random starting positions
    e.m_pos.m_x=(std::rand() % 200)-100;
    e.m_pos.m_y=(std::rand() % 200)-100;
  }

}

void Enemy::draw(const std::string &_shader,  ngl::Camera *_cam )
{
  // draw every enemy
  for(auto &e : m_enemies)
  {
    // setup and assign shader
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    (*shader)[_shader]->use();

    // set position and rotation
    e.m_transform.setPosition(e.m_pos);
    e.m_transform.setRotation(0,0,e.m_rotation);

    // setup shader with MVP
    ngl::Mat4 MVP= e.m_transform.getMatrix() * _cam->getVPMatrix() ;
    shader->setShaderParamFromMat4("MVP",MVP);

    // depending on how much health the enemy has, change its colour
    switch (e.health)
    {
    case 3 : {ngl::Material m(ngl::STDMAT::SILVER); m.loadToShader("material"); break;}
    case 2 : {ngl::Material m(ngl::STDMAT::GOLD); m.loadToShader("material"); break;}
    case 1 : {ngl::Material m(ngl::STDMAT::COPPER); m.loadToShader("material"); break;}
    }

    e.m_mesh->draw();

    // reload silver shader (default shader
    ngl::Material m(ngl::STDMAT::SILVER);
    m.loadToShader("material");
  }
}

void Enemy::moveEnemy(ngl::Vec3 spos)
{
  for(auto &e : m_enemies)
  {
    // reset movement values
    float xDirection=0.0;
    float yDirection=0.0;
    ngl::Vec3 epos = e.m_pos;

    // determine where the player is in relation to enemy
    // then move towards it
    if(spos.m_x - epos.m_x > 2)
    {
      xDirection+=speed;
    }

    if(spos.m_x - epos.m_x < -2)
    {
      xDirection-=speed;
    }

    if(spos.m_y - epos.m_y > 2)
    {
      yDirection+=speed;
    }

    if(spos.m_y - epos.m_y < -2)
    {
      yDirection-=speed;
    }

    // rotate to face the direction that they're heading
    if(yDirection>0 && xDirection<0){e.m_rotation=45;}
    if(yDirection<0 && xDirection>0){e.m_rotation=205;}
    if(yDirection<0 && xDirection<0){e.m_rotation=135;}
    if(yDirection>0 && xDirection>0){e.m_rotation=315;}

    // apply movement
    e.m_pos.m_x+=xDirection;
    e.m_pos.m_y+=yDirection;
  }
}

int Enemy::attack(ngl::Vec3 spos, int rot)
{
  // counter to see how many enemies got killed
  int hitVal=0;

  // how close an enemy has to be for it to get hit
  float attackRange = 12.0;

  // check every enemy for collisions
  for(auto &e : m_enemies)
  {
    // this determines where to check for depending on which direction the player was facing
    // the attack only applies to that one direction in a cone
    switch(rot)
    {
    case 0:
    {
      if(spos.m_x - e.m_pos.m_x < attackRange && spos.m_x - e.m_pos.m_x > -attackRange)
      {
        if(spos.m_y - e.m_pos.m_y > -attackRange && spos.m_y - e.m_pos.m_y <= 0)
        {
          e.m_pos.m_y+=10;
          e.health-=1;
        }
      }
      break;
    }
    case 270:
    {
      if(spos.m_x - e.m_pos.m_x > -attackRange && spos.m_x - e.m_pos.m_x <= 0)
      {
        if(spos.m_y - e.m_pos.m_y < attackRange && spos.m_y - e.m_pos.m_y > -attackRange)
        {
          e.m_pos.m_x+=10;
          e.health-=1;
        }
      }

      break;
    }
    case 180:
    {
      if(spos.m_x - e.m_pos.m_x < attackRange && spos.m_x - e.m_pos.m_x > -attackRange)
      {
        if(spos.m_y - e.m_pos.m_y >= 0 && spos.m_y - e.m_pos.m_y < attackRange)
        {
          e.m_pos.m_y-=10;
          e.health-=1;
        }
      }

      break;
    }
    case 90:
    {
      if(spos.m_x - e.m_pos.m_x >= 0 && spos.m_x - e.m_pos.m_x < attackRange)
      {
        if(spos.m_y - e.m_pos.m_y < attackRange && spos.m_y - e.m_pos.m_y > -attackRange)
        {
          e.m_pos.m_x-=10;
          e.health-=1;
        }
      }

      break;

    }
    }

    // if the enemy has been killed
    if (e.health == 0)
    {
      // value to determine how much health the new enemy will have
      difficulty +=1;

      // the more enemies killed, the more likely it is for higher health enemies to spawn, up to 3 health
      if (difficulty < 15)
      {
        e.health=1;
      }
      else if (difficulty < 50)
      {
        e.health=(std::rand() % 2 + 1);
      }else
      {
        e.health=(std::rand() % 3 + 1);
      }

      // respawn in a random position
      e.m_pos.m_x=(std::rand() % 200)-100;
      e.m_pos.m_y=(std::rand() % 200)-100;

      // every time an enemy has been killed, all enemies speed up very slightly to further increase challenge
      speed+=0.001;

      // count that an enemy has been killed
      hitVal+=1;
    }
  }
  return hitVal;
}



int Enemy::knockbackPlayer(ngl::Vec3 spos)
{

  int hit = 0;
  // collision detection for all enemies
  for(auto &e : m_enemies)
  {
    // compare the location of the enemy to the player to see if they've been hit and from which direction
    if(spos.m_x - e.m_pos.m_x < 4 && spos.m_x - e.m_pos.m_x >= 0)
    {
      if(spos.m_y - e.m_pos.m_y < 4 && spos.m_y - e.m_pos.m_y >= 0)
      {
        hit = 1;
      }
    }


    if(spos.m_x - e.m_pos.m_x > -4 && spos.m_x - e.m_pos.m_x <= 0)
    {
      if(spos.m_y - e.m_pos.m_y < 4 && spos.m_y - e.m_pos.m_y >= 0)
      {
        hit = 2;
      }
    }

    if(spos.m_x - e.m_pos.m_x < 4 && spos.m_x - e.m_pos.m_x >= 0)
    {
      if(spos.m_y - e.m_pos.m_y > -4 && spos.m_y - e.m_pos.m_y <= 0)
      {
        hit = 3;
      }
    }


    if(spos.m_x - e.m_pos.m_x > -4 && spos.m_x - e.m_pos.m_x <= 0)
    {
      if(spos.m_y - e.m_pos.m_y > -4 && spos.m_y - e.m_pos.m_y <= 0)
      {
        hit = 4;
      }
    }
  }
  return hit;
}

void Enemy::enemyCollisions()
{
  // how much the enemy is shifted away if it's overlapping
  float mVal = 0.1;
  // how close the enemies have to be to trigger the movement
  float dVal = 5.0;

  // for every enemy...
  for(auto &e : m_enemies)
  {
    // check all other enemies
    for(auto &c : m_enemies)
    {
      if(e.m_pos.m_x - c.m_pos.m_x < dVal && e.m_pos.m_x - c.m_pos.m_x >=0)
      {
        if(e.m_pos.m_y - c.m_pos.m_y < dVal && e.m_pos.m_y - c.m_pos.m_y > -dVal)
        {
          c.m_pos.m_x -= mVal;
        }

      }
      if(e.m_pos.m_x - c.m_pos.m_x > -dVal && e.m_pos.m_x - c.m_pos.m_x <=0)
      {
        if(e.m_pos.m_y - c.m_pos.m_y < dVal && e.m_pos.m_y - c.m_pos.m_y > -dVal)
        {
          c.m_pos.m_x += mVal;
        }
      }

      if(e.m_pos.m_y - c.m_pos.m_y < dVal && e.m_pos.m_y - c.m_pos.m_y >=0)
      {
        if(e.m_pos.m_x - c.m_pos.m_x < dVal && e.m_pos.m_x - c.m_pos.m_x > -dVal)
        {
          c.m_pos.m_y -= mVal;
        }
      }
      if(e.m_pos.m_y - c.m_pos.m_y > -dVal && e.m_pos.m_y - c.m_pos.m_y <=0)
      {
        if(e.m_pos.m_x - c.m_pos.m_x < dVal && e.m_pos.m_x - c.m_pos.m_x > -dVal)
        {
          c.m_pos.m_y += mVal;
        }
      }


    }
  }
}

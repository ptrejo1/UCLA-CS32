#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"

Actor::Actor(StudentWorld* w, double startX, double startY, int imageID, int imageDir, double size, int depth) : GraphObject(imageID, startX, startY, imageDir, size, depth), m_world(w), m_dead(false)
{
}

bool Actor::isDead() const
{
    return m_dead;
}

bool Actor::collidableWithPlayerFiredProjectile() const
{
    return false;
}

StudentWorld* Actor::world() const
{
    return m_world;
}

void Actor::setDead()
{
    m_dead = true;
}

void Actor::moveTo(double x, double y)
{
    if (getX() < VIEW_WIDTH && getY() < VIEW_HEIGHT)
        GraphObject::moveTo(x, y);
    else
        setDead();
}

Star::Star(StudentWorld* w, double startX, double startY) : Actor(w, startX, startY, IID_STAR, 0, randInt(5, 50)*.001, 3)
{
}

void Star::doSomething()
{
    moveTo(getX()-1, getY());
}

/*
 Put Explosion here
*/

DamageableObject::DamageableObject(StudentWorld* w, double startX, double startY, int imageID, int startDir, double size, int depth, double hitPoints) : Actor(w, startX, startY, imageID, startDir, size, depth), m_hitPoints(hitPoints)
{
}

double DamageableObject::hitPoints() const
{
    return m_hitPoints;
}

void DamageableObject::increaseHitPoints(double amt)
{
    m_hitPoints += amt;
}

void DamageableObject::sufferDamage(double amt, int cause)
{
    m_hitPoints -= amt;
}

Player::Player(StudentWorld* w) : DamageableObject(w, 0, 128, IID_NACHENBLASTER, 0, 1.0, 0, 50), m_cabbagePoints(30)
{
}

void Player::doSomething()
{
    
    if (!isDead())
    {
        int ch;
        if (world()->GameWorld::getKey(ch))
        {
            switch (ch)
            {
                case KEY_PRESS_UP : moveTo(<#double x#>, <#double y#>) break;
                    
            }
            
        }
        
    }
    else
        return;

}

void Player::increaseHitPoints(double amt)
{
    DamageableObject::increaseHitPoints(amt);
}

void Player::sufferDamage(double amt, int cause)
{
    DamageableObject::sufferDamage(amt, cause);
}

void Player::increaseTorpedoes(int amt)
{
    m_torpedos += amt;
}

int Player::healthPct() const
{
    return static_cast<int>((hitPoints()/50)*100);
}

int Player::cabbagePct() const
{
    return static_cast<int>((m_cabbagePoints/30)*100);
}

/*
 
 Notes:
 Actors always initialized to alive
 use setDead() to change this
 
 Actor moveTo() method will check if its
 on the the screen before calling GraphObject moveTo
 
 rememeber to override sufferDamage
 (this is kinda weird)
 
*/

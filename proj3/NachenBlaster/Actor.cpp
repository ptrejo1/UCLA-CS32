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
    if (x < VIEW_WIDTH && x >= 0 && y < VIEW_HEIGHT && y >= 0)
        GraphObject::moveTo(x, y);
    else
        setDead();
}

Star::Star(StudentWorld* w, double startX, double startY) : Actor(w, startX, startY, IID_STAR, START_DIRECTION, randInt(5, 50)*.01, DEPTH+3)
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

Player::Player(StudentWorld* w) : DamageableObject(w, 0, PLAYER_START_Y, IID_NACHENBLASTER, START_DIRECTION, PLAYER_SIZE, DEPTH, PLAYER_HIT_POINTS), m_cabbagePoints(PLAYER_FULL_CABBAGE)
{
}

// TODO
// Implement Collisions
// Probably Done in StudentWorld
void Player::doSomething()
{
    
    if (!isDead())
    {
        int ch;
        if (world()->GameWorld::getKey(ch))
        {
            switch (ch)
            {
                case KEY_PRESS_UP:
                {
                    moveTo(getX(), getY()+PLAYER_SPEED);
                    // collided();
                    break;
                }
                case KEY_PRESS_DOWN:
                {
                    moveTo(getX(), getY()-PLAYER_SPEED);
                    // collided();
                    break;
                }
                case KEY_PRESS_LEFT:
                {
                    moveTo(getX()-PLAYER_SPEED, getY());
                    // collided();
                    break;
                }
                case KEY_PRESS_RIGHT:
                {
                    moveTo(getX()+PLAYER_SPEED, getY());
                    // collided();
                    break;
                }
                case KEY_PRESS_SPACE:
                {
                    world()->addActor(new Cabbage(world(), getX()+12, getY()));
                    world()->playSound(SOUND_PLAYER_SHOOT);
                    break;
                }
                case KEY_PRESS_TAB:
                {
                    // TODO shoot torpedo
                    break;
                }
            }
        }
        if (m_cabbagePoints < PLAYER_FULL_CABBAGE) {
            m_cabbagePoints++;
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

void Player::moveTo(double x, double y)
{
    if (x < VIEW_WIDTH && x >= 0 && y < VIEW_HEIGHT && y >= 0)
        GraphObject::moveTo(x, y);
}

void Player::increaseTorpedoes(int amt)
{
    m_torpedos += amt;
}

int Player::healthPct() const
{
    return static_cast<int>((hitPoints()/PLAYER_HIT_POINTS)*100);
}

int Player::cabbagePct() const
{
    return static_cast<int>((m_cabbagePoints/PLAYER_FULL_CABBAGE)*100);
}

Alien::Alien(StudentWorld* w, double startX, double startY, int imageID, double hitPoints, double damageAmt, double deltaX, double deltaY, double speed, unsigned int scoreValue) : DamageableObject(w, startX, startY, imageID, START_DIRECTION, ALIEN_SIZE, DEPTH+1, hitPoints), m_deltaX(deltaX), m_deltaY(deltaY), m_speed(speed), m_scoreValue(scoreValue), m_flight_plan(0)
{
}

bool Alien::collidableWithPlayerFiredProjectile() const
{
    return true;
}

void Alien::sufferDamage(double amt, int cause)
{
    DamageableObject::sufferDamage(amt, cause);
}

void Alien::move()
{
    if (m_flight_plan == 0)
    {
        m_flight_plan = randInt(1, 32);
    }
    if (getY() >= VIEW_HEIGHT-1)
    {
        setDeltaY(-1*m_speed);
    }
    if(getY() <= 0)
    {
        setDeltaY(m_speed);
    }
    m_flight_plan--;
}

void Alien::setDeltaY(double dy)
{
    m_deltaY = dy;
}

void Alien::setSpeed(double speed)
{
    m_speed = speed;
}

double Alien::getDeltaX()
{
    return m_deltaX;
}

double Alien::getDeltaY()
{
    return m_deltaY;
}

// not real imp
bool Alien::damageCollidingPlayer(double amt)
{
    return true;
}

// not real imp
void Alien::possiblyDropGoodie()
{
    
}

Smallgon::Smallgon(StudentWorld* w, double startX, double startY) : Alien(w, startX, startY, IID_SMALLGON, 5*(1+(w->getLevel()-1)*0.1), SMALLGON_DAMAGE, SMALLGON_SPEED, SMALLGON_SPEED, SMALLGON_SPEED, SMALLGON_SCORE_VALUE)
{
}

void Smallgon::doSomething()
{
    move();
    moveTo(getX()-getDeltaX(), getY()+getDeltaY());
}

Projectile::Projectile(StudentWorld* w, double startX, double startY, int imageID, double damageAmt, double deltaX, bool rotates, int imageDir) : Actor(w, startX, startY, imageID, imageDir, PROJECTILE_SIZE, DEPTH+1)
{
}

void Projectile::moveTo(double x, double y)
{
    if (x < VIEW_WIDTH && x >= 0)
        GraphObject::moveTo(x, y);
    else
        setDead();
}

Cabbage::Cabbage(StudentWorld* w, double startX, double startY) : Projectile(w, startX, startY, IID_CABBAGE, CABBAGE_DAMAGE, CABBAGE_DELTA_X, true, START_DIRECTION)
{
}

void Cabbage::doSomething()
{
    if (!isDead())
    {
        // TODO
        moveTo(getX()+CABBAGE_DELTA_X, getY());
        setDirection(getDirection()+ROTATE);
        // TODO
    }
    else
        return;
}

/*
 
 Notes:
 Actors always initialized to alive
 use setDead() to change this
 
 Actor moveTo() method will check if its
 on the the screen before calling GraphObject moveTo
 
 TODO:
 
 ** Actor::collidableWithPlayerFiredProjectile
    needs to be implmented for Alien
 
 ** Alien::damageCollidingPlayer
 needs to be implmented for Alien
 
 ** Alien::possiblyDropGoodie
 
 
 ** rememeber to override sufferDamage
 (this is kinda weird)
 
 ** Alien::damageCollidingPlayer
 
 ** Alien::possiblyDropGoodie
 
*/

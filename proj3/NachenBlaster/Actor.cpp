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
    // if within bounds move otherwise set actor to dead
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

Explosion::Explosion(StudentWorld* w, double startX, double startY) : Actor(w, startX, startY, IID_EXPLOSION, START_DIRECTION, EXPLOSION_SIZE, DEPTH), m_duration(4)
{
}

void Explosion::doSomething()
{
    // once explosion has been alive for 4 ticks
    // play sound and set to dead
    if(m_duration == 0)
    {
        world()->playSound(SOUND_DEATH);
        setDead();
        return;
    }
    setSize(getSize()*1.5);
    m_duration--;
}

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

Player::Player(StudentWorld* w) : DamageableObject(w, 0, PLAYER_START_Y, IID_NACHENBLASTER, START_DIRECTION, PLAYER_SIZE, DEPTH, PLAYER_HIT_POINTS), m_cabbagePoints(PLAYER_FULL_CABBAGE), m_torpedos(0)
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
                case KEY_PRESS_UP:
                {
                    moveTo(getX(), getY()+PLAYER_SPEED);
                    break;
                }
                case KEY_PRESS_DOWN:
                {
                    moveTo(getX(), getY()-PLAYER_SPEED);
                    break;
                }
                case KEY_PRESS_LEFT:
                {
                    moveTo(getX()-PLAYER_SPEED, getY());
                    break;
                }
                case KEY_PRESS_RIGHT:
                {
                    moveTo(getX()+PLAYER_SPEED, getY());
                    break;
                }
                case KEY_PRESS_SPACE:
                {
                    // if enough cabbage points fire
                    if (m_cabbagePoints >= 5)
                    {
                        world()->addActor(new Cabbage(world(), getX()+12, getY()));
                        world()->playSound(SOUND_PLAYER_SHOOT);
                        m_cabbagePoints -= 5;
                    }
                    break;
                }
                case KEY_PRESS_TAB:
                {
                    // if enough torpedos fire
                    if (m_torpedos > 0)
                    {
                        world()->addActor(new PlayerLaunchedTorpedo(world(), getX()+12, getY()));
                        world()->playSound(SOUND_TORPEDO);
                        m_torpedos--;
                        break;
                    }
                }
            }
        }
        // add to cabbage points each tick
        if (m_cabbagePoints < PLAYER_FULL_CABBAGE)
        {
            m_cabbagePoints++;
        }
    }
}

void Player::increaseHitPoints(double amt)
{
    // dont allow more than allowed hit points
    if (hitPoints()+amt > PLAYER_HIT_POINTS) {
        DamageableObject::increaseHitPoints(PLAYER_HIT_POINTS-hitPoints());
        return;
    }
    DamageableObject::increaseHitPoints(amt);
}

void Player::sufferDamage(double amt, int cause)
{
    // hit by projectile
    if (cause)
    {
        // projectile killed player
        if (hitPoints()-amt <= 0)
        {
            world()->addActor(new Explosion(world(), getX(), getY()));
            setDead();
            return;
        }
        world()->playSound(SOUND_BLAST);
        DamageableObject::sufferDamage(amt, cause);
    }
    // hit by alien
    else
    {
        if (hitPoints()-amt <= 0)
        {
            world()->addActor(new Explosion(world(), getX(), getY()));
            setDead();
            return;
        }
        DamageableObject::sufferDamage(amt, cause);
    }
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
    return static_cast<int>((m_cabbagePoints/PLAYER_FULL_CABBAGE)*100.0);
}

int Player::numTorpedoes() const
{
    return m_torpedos;
}

Alien::Alien(StudentWorld* w, double startX, double startY, int imageID, double hitPoints, double damageAmt, double deltaX, double deltaY, double speed, unsigned int scoreValue) : DamageableObject(w, startX, startY, imageID, START_DIRECTION, ALIEN_SIZE, DEPTH+1, hitPoints), m_deltaX(deltaX), m_deltaY(deltaY), m_speed(speed), m_scoreValue(scoreValue), m_flight_plan(0)
{
}

bool Alien::collidableWithPlayerFiredProjectile() const
{
    return true;
}

void Alien::sufferDamage(double amt, int cause, int score)
{
    // hit by projectile
    if (cause) {
        // projectile caused death
        if (hitPoints() - amt <= 0)
        {
            world()->addActor(new Explosion(world(), getX(), getY()));
            setDead();
            world()->recordAlienDestroyed();
            possiblyDropGoodie();
            world()->increaseScore(score);
            return;
        }
        world()->playSound(SOUND_BLAST);
        DamageableObject::sufferDamage(amt, cause);
    }
    // hit by player
    else
    {
        world()->addActor(new Explosion(world(), getX(), getY()));
        setDead();
        world()->recordAlienDestroyed();
        possiblyDropGoodie();
    }
}

void Alien::move()
{
    // decide if new flight plan is needed
    if (m_flight_plan == 0)
    {
        m_flight_plan = randInt(1, 32);
        
        // randomly select new flight direction
        int choice = randInt(1, 3);
        switch (choice)
        {
            case 1: setDeltaY(0); break;
            case 2: setDeltaY(m_speed); break;
            case 3: setDeltaY(-1*m_speed); break;
        }
    }
    
    // dont fly out of bounds
    if (getY()+m_deltaY >= VIEW_HEIGHT-1)
    {
        setDeltaY(-1*getDeltaY());
    }
    if(getY()+m_deltaY <= 0)
    {
        setDeltaY(-1*getDeltaY());
    }
    
    // reduce flight plan
    m_flight_plan--;
}

void Alien::setDeltaY(double dy)
{
    m_deltaY = dy;
}

void Alien::setDeltaX(double dx)
{
    m_deltaX = dx;
}

double Alien::getDeltaX()
{
    return m_deltaX;
}

double Alien::getDeltaY()
{
    return m_deltaY;
}

void Alien::setFlightPlan(double length)
{
    m_flight_plan = length;
}

bool Alien::damageCollidingPlayer(double amt, int scoreValue)
{
    Player* p = world()->getCollidingPlayer(this);
    if (p != nullptr)
    {
        p->sufferDamage(amt, HIT_BY_SHIP);
        world()->increaseScore(scoreValue);
        sufferDamage(0, HIT_BY_SHIP, 0);
        return true;
    }
    
    return false;
}

void Alien::possiblyDropGoodie()
{
}

bool Alien::shoot(int choice)
{
    if (world()->playerInLineOfFire(this) && choice == 2)
    {
        world()->addActor(new Turnip(world(), getX()-14, getY()));
        world()->playSound(SOUND_ALIEN_SHOOT);
        return true;
    }
    return false;
}

unsigned int Alien::getScoreValue()
{
    return m_scoreValue;
}

Smallgon::Smallgon(StudentWorld* w, double startX, double startY) : Alien(w, startX, startY, IID_SMALLGON, 5*(1+(w->getLevel()-1)*0.1), ALIEN_DAMAGE, ALIEN_SPEED, ALIEN_SPEED, ALIEN_SPEED, ALIEN_SCORE_VALUE)
{
}

void Smallgon::doSomething()
{
    if (!isDead()) {
        
        // check if collided with player
        if (damageCollidingPlayer(ALIEN_DAMAGE, ALIEN_SCORE_VALUE))
        {
            return;
        }
        
        // flight plan
        move();
        
        // shoot
        int choice = randInt(1, ((20/world()->getLevel())+5));
        if(shoot(choice))
        {
            return;
        }
        
        // move alien
        moveTo(getX()-getDeltaX(), getY()+getDeltaY());
    }
}

Smoregon::Smoregon(StudentWorld* w, double startX, double startY) : Alien(w, startX, startY, IID_SMOREGON, 5*(1+(w->getLevel()-1)*0.1), ALIEN_DAMAGE, ALIEN_SPEED, ALIEN_SPEED, ALIEN_SPEED, ALIEN_SCORE_VALUE)
{
}

void Smoregon::doSomething()
{
    if (!isDead())
    {
        int choice = randInt(1, ((20/world()->getLevel())+5));
        
        // check if collided with player
        if (damageCollidingPlayer(ALIEN_DAMAGE, ALIEN_SCORE_VALUE))
        {
            return;
        }
        
        // update flight plan
        move();
        
        // apply ramming speed
        if (choice == 1)
        {
            setDeltaY(0);
            setFlightPlan(VIEW_WIDTH);
            setDeltaX(5);
        }
        
        // shoot
        if(shoot(choice))
        {
            return;
        }
        
        // move alien
        moveTo(getX()-getDeltaX(), getY()+getDeltaY());
    }
}

void Smoregon::possiblyDropGoodie()
{
    // drop with 1/3 chance
    int goodie = randInt(1, 3);
    if (goodie == 1)
    {
        int drop = randInt(1, 2);
        // repair goodie w/ 1/2 chance
        if (drop == 1)
        {
            world()->addActor(new RepairGoodie(world(), getX(), getY()));
            return;
        }
        world()->addActor(new TorpedoGoodie(world(), getX(), getY()));
    }
}

Snagglegon::Snagglegon(StudentWorld* w, double startX, double startY) : Alien(w, startX, startY, IID_SNAGGLEGON, 10*(1+(w->getLevel()-1)*0.1), SNAGGLEGON_DAMAGE, SNAGGLEGON_SPEED, -1*SNAGGLEGON_SPEED, SNAGGLEGON_SPEED, SNAGGLEGON_SCORE_VALUE)
{
}

void Snagglegon::doSomething()
{
    if (!isDead())
    {
        int choice = randInt(1, ((15/world()->getLevel())+10));
        
        // collided w/ player
        if (damageCollidingPlayer(ALIEN_DAMAGE*3, SNAGGLEGON_SCORE_VALUE))
        {
            return;
        }
        
        // shoot
        if(shoot(choice))
        {
            return;
        }
        
        // adjust flight plan if going out of bounds
        if (getY()+getDeltaY() >= VIEW_HEIGHT-1)
        {
            setDeltaY(-1*getDeltaY());
        }
        if(getY()+getDeltaY() <= 0)
        {
            setDeltaY(-1*getDeltaY());
        }
        
        // move alien
        moveTo(getX()-getDeltaX(), getY()+getDeltaY());
    }
}

void Snagglegon::possiblyDropGoodie()
{
    // drop life w/ 1/6 chance
    int goodie = randInt(1, 6);
    if (goodie == 1)
    {
        world()->addActor(new ExtraLifeGoodie(world(), getX(), getY()));
    }
}

bool Snagglegon::shoot(int choice)
{
    if (world()->playerInLineOfFire(this) && choice == 2)
    {
        world()->addActor(new AlienLaunchedTorpedo(world(), getX()-14, getY()));
        world()->playSound(SOUND_TORPEDO);
        return true;
    }
    return false;
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

bool Projectile::alienCollidedWithProj(int damage)
{
    Alien* a = dynamic_cast<Alien*>(world()->getOneCollidingAlien(this));
    if (a != nullptr)
    {
        a->sufferDamage(damage, HIT_BY_PROJECTILE, a->getScoreValue());
        setDead();
        return true;
    }
    return false;
}

bool Projectile::playerCollidedWithProj(int damage)
{
    Player* p = world()->getCollidingPlayer(this);
    if (p != nullptr)
    {
        p->sufferDamage(damage, HIT_BY_PROJECTILE);
        setDead();
        return true;
    }
    return false;
}

Cabbage::Cabbage(StudentWorld* w, double startX, double startY) : Projectile(w, startX, startY, IID_CABBAGE, CABBAGE_DAMAGE, CABBAGE_DELTA_X, true, START_DIRECTION)
{
}

void Cabbage::doSomething()
{
    if (!isDead())
    {
        // check if collided
        if (alienCollidedWithProj(CABBAGE_DAMAGE))
        {
            return;
        }
        
        // move cabbage
        moveTo(getX()+CABBAGE_DELTA_X, getY());
        setDirection(getDirection()+ROTATE);
        
        // check if collided
        if (alienCollidedWithProj(CABBAGE_DAMAGE))
        {
            return;
        }
    }
}

Turnip::Turnip(StudentWorld* w, double startX, double startY) : Projectile(w, startX, startY, IID_TURNIP, TURNIP_DAMAGE, TURNIP_DELTA_X, true, START_DIRECTION)
{
}

void Turnip::doSomething()
{
    if (!isDead())
    {
        // check if collided
        if (playerCollidedWithProj(TURNIP_DAMAGE))
        {
            return;
        }
        
        // move turnip
        moveTo(getX()-CABBAGE_DELTA_X, getY());
        setDirection(getDirection()+ROTATE);
        
        // check if collided
        if (playerCollidedWithProj(TURNIP_DAMAGE))
        {
            return;
        }
    }
}

Torpedo::Torpedo(StudentWorld* w, double startX, double startY, double deltaX, int imageDir) : Projectile(w, startX, startY, IID_TORPEDO, TORPEDO_DAMAGE, deltaX, false, imageDir)
{
}

PlayerLaunchedTorpedo::PlayerLaunchedTorpedo(StudentWorld* w, double startX, double startY) : Torpedo(w, startX, startY, TORPEDO_DELTA_X, START_DIRECTION)
{
}

void PlayerLaunchedTorpedo::doSomething()
{
    if (!isDead())
    {
        // check if collided
        if (alienCollidedWithProj(TORPEDO_DAMAGE))
        {
            return;
        }
        
        // move torpedo
        moveTo(getX()+TORPEDO_DELTA_X, getY());
        
        // check if collided
        if (alienCollidedWithProj(TORPEDO_DAMAGE))
        {
            return;
        }
    }
}

AlienLaunchedTorpedo::AlienLaunchedTorpedo(StudentWorld* w, double startX, double startY) : Torpedo(w, startX, startY, TORPEDO_DELTA_X, ALIEN_TORPEDO_DIRECTION)
{
}

void AlienLaunchedTorpedo::doSomething()
{
    if (!isDead())
    {
        // check if collided
        if (playerCollidedWithProj(TORPEDO_DAMAGE))
        {
            return;
        }
        
        // move torpedo
        moveTo(getX()-CABBAGE_DELTA_X, getY());
        
        // check if collided
        if (playerCollidedWithProj(TORPEDO_DAMAGE))
        {
            return;
        }
    }
}

Goodie::Goodie(StudentWorld* w, double startX, double startY, int imageID) : Actor(w, startX, startY, imageID, START_DIRECTION, GOODIE_SIZE, DEPTH+1)
{
}

bool Goodie::helpCollidingPlayer()
{
    Player* p = world()->getCollidingPlayer(this);
    if (p != nullptr)
    {
        world()->increaseScore(GOODIE_POINTS);
        world()->playSound(SOUND_GOODIE);
        setDead();
        return true;
    }
    return false;
}

ExtraLifeGoodie::ExtraLifeGoodie(StudentWorld* w, double startX, double startY) : Goodie(w, startX, startY, IID_LIFE_GOODIE)
{
}

void ExtraLifeGoodie::doSomething()
{
    if (!isDead())
    {
        // check if collided
        if (helpCollidingPlayer())
        {
            return;
        }
        
        // move goodie
        moveTo(getX()-GOODIE_SPEED, getY()-GOODIE_SPEED);
        
        // check if collided
        if (helpCollidingPlayer())
        {
            return;
        }
    }
}

bool ExtraLifeGoodie::helpCollidingPlayer()
{
    if (Goodie::helpCollidingPlayer())
    {
        world()->incLives();
        return true;
    }
    return false;
}

RepairGoodie::RepairGoodie(StudentWorld* w, double startX, double startY) : Goodie(w, startX, startY, IID_REPAIR_GOODIE)
{
}

void RepairGoodie::doSomething()
{
    if (!isDead())
    {
        // check if collided
        if (helpCollidingPlayer())
        {
            return;
        }
        
        // move goodie
        moveTo(getX()-GOODIE_SPEED, getY()-GOODIE_SPEED);
        
        // check if collided
        if (helpCollidingPlayer())
        {
            return;
        }
    }
}

bool RepairGoodie::helpCollidingPlayer()
{
    if (Goodie::helpCollidingPlayer())
    {
        world()->getPlayer()->increaseHitPoints(10);
        return true;
    }
    return false;
}

TorpedoGoodie::TorpedoGoodie(StudentWorld* w, double startX, double startY) : Goodie(w, startX, startY, IID_TORPEDO_GOODIE)
{
}

void TorpedoGoodie::doSomething()
{
    if (!isDead())
    {
        // check if collided
        if (helpCollidingPlayer())
        {
            return;
        }
        
        // move goodie
        moveTo(getX()-GOODIE_SPEED, getY()-GOODIE_SPEED);
        
        // check if collided
        if (helpCollidingPlayer())
        {
            return;
        }
    }
}

bool TorpedoGoodie::helpCollidingPlayer()
{
    if (Goodie::helpCollidingPlayer())
    {
        world()->getPlayer()->increaseTorpedoes(5);
        return true;
    }
    return false;
}

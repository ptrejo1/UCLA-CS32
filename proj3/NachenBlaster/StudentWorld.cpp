#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(string assetDir)
: GameWorld(assetDir)
{
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
{
    m_aliens_destroyed = 0;
    
    // add Player
    m_player = new Player(this);
    
    // add stars
    for (int i = 0; i < 30; i++)
    {
        Star* star = new Star(this, randInt(0, VIEW_WIDTH)-1, randInt(0, VIEW_HEIGHT-1));
        addActor(star);
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    if (!m_player->isDead())
    {
        m_player->doSomething();
    }
    
    // give actors a chance to do something
    vector<Actor*>:: iterator it;
    for (it = actors.begin(); it != actors.end(); it++)
    {
        if (!(*it)->isDead())
        {
            (*it)->doSomething();
        }
        
        // if player died end level
        if (m_player->isDead())
        {
            decLives();
            delete m_player;
            return GWSTATUS_PLAYER_DIED;
        }
        
        // if level completed return
        if(m_aliens_destroyed == 6+(4*this->getLevel()))
        {
            m_aliens_destroyed = 0;
            delete m_player;
            return GWSTATUS_FINISHED_LEVEL;
        }
    }
    addStar();
    
    addAlien();
    
    removeDead();
    
    // update display
    ostringstream oss;
    oss.setf(ios::fixed);
    oss << "Lives:" << setw(2) << getLives() << setw(10) << "Health: " << setw(3) << m_player->healthPct() << "%" << setw(8) << "Score:" << setw(6) << getScore() << setw(8) << "Level:" << setw(2) << getLevel() << setw(11) << "Cabbages:" << setw(4) << m_player->cabbagePct() << "%" << setw(11) << "Torpedos:" << setw(2) << m_player->numTorpedoes();
    setGameStatText(oss.str());
    
    //decLives();
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    vector<Actor*>::iterator it;
    for (it = actors.begin(); it != actors.end(); it++)
    {
        delete (*it);
    }
    actors.clear();
}

Actor* StudentWorld::getOneCollidingAlien(const Actor *a) const
{
    vector<Actor*>::const_iterator it;
    for (it = actors.begin(); it != actors.end(); it++)
    {
        if ((*it)->collidableWithPlayerFiredProjectile() && collided(a, *it))
        {
            return (*it);
        }
    }
    
    return nullptr;
}

Player* StudentWorld::getCollidingPlayer(const Actor* a) const
{
    if (collided(a, m_player))
    {
        return m_player;
    }
    
    return nullptr;
}

bool StudentWorld::collided(const Actor *a, const Actor *b) const
{
    // calculate distance and radii
    double distance = sqrt(pow((b->getX())-(a->getX()), 2.0)+pow((b->getY())-(a->getY()), 2.0));
    double radii = 0.75*(a->getRadius()+a->getRadius());
    
    if (distance < radii) {
        return true;
    }
    return false;
}

bool StudentWorld::playerInLineOfFire(const Actor *a) const
{
    if (m_player->getX() < a->getX() && abs(m_player->getY()-a->getY()) <= 4)
    {
        return true;
    }
    return false;
}

void StudentWorld::addActor(Actor *a)
{
    actors.push_back(a);
}

void StudentWorld::recordAlienDestroyed()
{
    m_aliens_destroyed++;
}

void StudentWorld::addStar()
{
    if (randInt(1, 15) == 1)
    {
        addActor(new Star(this, VIEW_WIDTH-1, randInt(0, VIEW_HEIGHT-1)));
    }
}

void StudentWorld::addAlien()
{
    int T = 6+(4*this->getLevel());     // total ships per level
    int M = 4+(0.5*this->getLevel());   // max aliens
    
    // count aliens on screen
    int aliens = 0;
    vector<Actor*>::iterator it;
    for (it = actors.begin(); it != actors.end(); it++)
    {
        if ((*it)->collidableWithPlayerFiredProjectile())
        {
            aliens++;
        }
    }
    
    // add alien according to proability
    if (aliens < min(M, T-m_aliens_destroyed))
    {
        int s1 = 60;
        int s2 = 20+getLevel()*5;
        int s3 = 5+getLevel()*5;
        int choice  = randInt(1, s1+s2+s3);
        
        if (choice >= 1 && choice <= s1)
        {
            addActor(new Smallgon(this, VIEW_WIDTH-1, randInt(0, VIEW_HEIGHT-1)));
        }
        else if (choice >= s1+1 && choice <= s1+s2)
        {
            addActor(new Smoregon(this, VIEW_WIDTH-1, randInt(0, VIEW_HEIGHT-1)));
        }
        else if (choice >= s1+s2+1 && choice <= s1+s2+s3)
        {
            addActor(new Snagglegon(this, VIEW_WIDTH-1, randInt(0, VIEW_HEIGHT-1)));
        }
    }
}

void StudentWorld::removeDead()
{
    vector<Actor*>::iterator it;
    for (it = actors.begin(); it != actors.end();)
    {
        if ((*it)->isDead())
        {
            delete (*it);
            it = actors.erase(it);
        }
        else
        {
            it++;
        }
    }
}

Player* StudentWorld::getPlayer() const
{
    return m_player;
}

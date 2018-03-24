#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(string assetDir)
: GameWorld(assetDir), m_aliens_destroyed(0)
{
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
{
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
    vector<Actor*>:: iterator it;
    for (it = actors.begin(); it != actors.end(); it++)
    {
        if (!m_player->isDead())
        {
            m_player->doSomething();
        }
        if (!(*it)->isDead())
        {
            (*it)->doSomething();
        }
        
        //if player died return
        
        // if level completed return
        
    }
    addStar();
    
    addAlien();
    
    removeDead();
    
    // update display
    
    //decLives();
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete m_player;
    
    vector<Actor*>::iterator it;
    for (it = actors.begin(); it != actors.end(); it++)
    {
        delete (*it);
    }
    actors.clear();
}
// only called by player fired projectile
// need to implement player collision
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

Player* getCollidingPlayer(const Actor* a)
{
    
    
    
    return nullptr;
}

bool StudentWorld::collided(const Actor *a, const Actor *b) const
{
    double distance = sqrt(pow((b->getX())-(a->getX()), 2.0)+pow((b->getY())-(a->getY()), 2.0));
    double radii = 0.75*(a->getRadius()+a->getRadius());
    
    if (distance < radii) {
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
    
    int aliens = 0;
    vector<Actor*>::iterator it;
    for (it = actors.begin(); it != actors.end(); it++)
    {
        if ((*it)->collidableWithPlayerFiredProjectile()) {
            aliens++;
        }
    }
    
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

/*
 
 ** Player is not in actors list **
 
 i. Add any private data members to this class required to keep track of Stars as well as the NachenBlaster object. You may ignore all other items in the game such as Smallgons, projectiles, goodies, etc. for Part #1.
 
 ii. Implement a constructor for this class that initializes your data
 members.
 
 iii. Implement a destructor for this class that frees any remaining
 dynamically allocated data, if any, that has not yet been freed at the
 time the StudentWorld object is about to destroyed.
 
 iv. Implement the init() method in this class. It must create the
 NachenBlaster and insert it into the space field at the proper starting location, as detailed in the spec above. It must also create all of the Stars and add them to the space field as specified in the spec above.
 
 v. Implement the move() method in your StudentWorld class. During
 each tick, it must ask your NachenBlaster and Stars to do something.
 Your move() method need not check to see if the NachenBlaster has
 died or not; you may assume for Part #1 that the NachenBlaster cannot
 die. Your move() method does not have to deal with any actors other
 than the NachenBlaster and the Stars. During each tick, the move()
 method may also need to introduce new stars onto the far right side of
 the screen, as detailed in the spec above.
 
 vi. Implement a cleanup() method that frees any dynamically allocated
 data that was allocated during calls to the init() method or the move() method (i.e., it should delete all your allocated Stars and the NachenBlaster). Note: Your StudentWorld class must have both a destructor and the cleanUp() method even though they likely do the same thing (in which case the destructor could just call cleanup()).
 
*/

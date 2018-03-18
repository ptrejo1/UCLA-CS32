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
: GameWorld(assetDir)
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

void StudentWorld::addActor(Actor *a)
{
    actors.push_back(a);
}

void StudentWorld::addStar()
{
    if (randInt(1, 15) == 1)
    {
        addActor(new Star(this, VIEW_WIDTH-1, randInt(0, VIEW_HEIGHT-1)));
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

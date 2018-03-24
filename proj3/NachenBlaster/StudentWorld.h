#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>

class Actor;
class Player;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();     // Can call cleanup
    
    // If there's at least one alien that's collided with a, return
    // a pointer to one of them; otherwise, return a null pointer.
    Actor* getOneCollidingAlien(const Actor* a) const;
    
    // If the player has collided with a, return a pointer to the player;
    // otherwise, return a null pointer.
    Player* getCollidingPlayer(const Actor* a) const;
    
    bool collided(const Actor* a, const Actor* b) const;
    
    // Is the player in the line of fire of a, which might cause a to attack?
    bool playerInLineOfFire(const Actor* a) const;
    
    // Add an actor to the world.
    void addActor(Actor* a);
    
    // Record that one more alien on the current level has been destroyed.
    void recordAlienDestroyed();
    
    // add stars as necessary
    void addStar();
    
    // add aliens as necessary
    void addAlien();
    
    // delete dead actors
    void removeDead();
    
private:
    std::vector<Actor*> actors;
    Player* m_player;
    int m_aliens_destroyed;
};

#endif // STUDENTWORLD_H_

#ifndef CITY_HH
#define CITY_HH
#include "interfaces/icity.hh"
#include "actors/stop.hh"
#include "actors/nysse.hh"
#include "errors/gameerror.hh"
#include "errors/initerror.hh"
#include "core/location.hh"
#include "statistics.hh"
#include "player.hh"
#include "murderer.hh"
#include <QTime>
#include <QTimer>
#include <memory>

/**
  * @file
  * @brief Defines a class that defines the details of the game city area.
  */
namespace StudentSide
{
using Stops=std::vector<std::shared_ptr<Interface::IStop>>;
using Actors=std::vector<std::shared_ptr<Interface::IActor>>;

const int INIT_STATE=0;
const int GAME_STATE=1;
const int WIN_RADIUS=3;
const int STOP_RADIUS=5;
const int WALK_RADIUS=100;
const int UPDATE_INTERVAL_MS=200;
// Locations of stops where the players will be initialized
const Interface::Location P1_START_LOC=
        Interface::Location(6.82544e+06, 3.32823e+06);
const Interface::Location P2_START_LOC=
        Interface::Location(6.82537e+06, 3.32758e+06);
        
class City: public Interface::ICity
{
public:
    /**
      * @brief Constructor
      * @post City is at initialization state.
      */    
    City();
    
    /**
      * @brief Destructor
      */
    ~City();

    //Initializing methods, requiring initstate:
    /**
     * @brief setBackground sets the bitmap picture of the game area.
     * @param basicbackground  Normal sized picture used as the game area.
     * @param bigbackground  Background of the game that is bigger than normal.
     * Used only if doing Scrolling map-expansion.
     * @pre City is in init state.
     * @post Picture for the game area is saved.
     * Initerror if a valid background is not given
     */
    void setBackground(QImage& bigbackground, QImage& basicbackground);

    /**
     * @brief setClock sets the time of the game clock.
     * @param clock Game clock time at the function call.
     * @pre `kello.isValid() == true`.
     * @post Time is set. Exception guarantee: nothrow.
     */
    void setClock(QTime clock);

    /**
     * @brief addStop adds a stop to the city.
     * @param stop pointer to a stop object.
     * @pre City is in init state.
     * @post if stop is in game-area it is added to the city.
     * Exception guarantee: basic
     */
    void addStop(std::shared_ptr<Interface::IStop> stop);

    /**
     * @brief startGame shifts city from init state to the gamestate.
     * @pre City is in init state.
     * setBackground(), setClock() and givePlayers() have been called.
     * @post City is in gamestate. Exception guarantee: nothrow.
     */
    void startGame();

    //Non-interface initialization methods:
    /**
     * @brief giveBackground
     * @pre city is in init state, background image has been initialized
     * @return background image
     */
    QImage giveBackground() const;

    /**
     * @brief givePlayers gives player1 and player2
     * @param playernum number of players either 1 or 2
     * @pre game is in initstate
     * @post if playernum is 1, player 2 is an ai player.
     */
    std::pair< std::shared_ptr<Player>, std::shared_ptr<Murderer> >
    givePlayers(int playernum);

    // Methods needed in both initalization and throughout the game:
    /**
     * @brief giveStops
     * @pre stops have been initialized
     * @return all stops in city
     */
    Stops giveStops() const;

    /**
     * @brief send_buses
     * @return all buses in city
     */
    std::vector<std::shared_ptr<CourseSide::Nysse>> send_buses() const;

    /**
     * @brief addActor adds a new actor to the city.
     * @param newactor actor to be added to the city that fulfills ActorIF.
     * @pre -
     * @post Actor is added to the city. Exception guarantee: basic.
     * @exception GameError Actor is already in the city.
     */
    void addActor(std::shared_ptr<Interface::IActor> newactor);

    //Methods needed in gamestate, some of them can also be used in initstate:
    /**
     * @brief removeActor removes the actor from the city.
     * @param actor Actor to be removed.
     * @pre City is in gamestate.
     * @post Actor is removed from the city. Exception guarantee: strong.
     * @exception GameError Actor not found in the city
     */
    void removeActor(std::shared_ptr<Interface::IActor> actor);

    /**
     * @brief actorDestroyed tells the city that actor is destroyed ingame.
     * @param actor Actor that is set destroyed ingame.
     * @pre City is in gamestate. Given actor is found in the city.
     *  Actor has `actor.isDestroyed() == true`.
     * @post Exception guarantee: strong.
     */
    void actorRemoved(std::shared_ptr<Interface::IActor> actor);

    /**
     * @brief findActor checks if the given actor is in the city.
     * @param actor Actor that that is looked for in the city.
     * @pre -
     * @return Boolean that tells whether the actor is in the city.
     * @post Exception guarantee: nothrow.
     */
    bool findActor(std::shared_ptr<Interface::IActor> actor) const;

    /**
     * @brief actorMoved is an used to tell whether certain actor has moved.
     * @param actor Actor that has moved.
     * @pre City is in gamestate. Given actor is found in the city.
     * @post Exception guarantee: basic.
     */
    void actorMoved(std::shared_ptr<Interface::IActor> actor);

    /**
     * @brief getNearbyActors returns actors that are close to given position.
     * @param loc Location for getting the actors close to it.
     * @pre City is in gamestate.
     * @return Vector containing actors close to the location,
     * that pass `getLocation().isClose(loc) == true`.
     * @post Exception guarantee: strong.
     */
    Actors getNearbyActors(Interface::Location loc) const;

    /**
     * @brief getNearbyStops finds nearby stops
     * @param loc location
     * @param dist maximum distance from loc, must be positive
     * @return stops that are distance dist or less from loc
     * @post Exception guarantee: strong
     **/
    Stops getNearbyStops(Interface::Location loc, int dist) const;

    /**
     * @brief isGameOver tells whether the game is overor not.
     * @pre City is in gamestate.
     * @return `true`, if game is over, else ´false´
     * @post Exception guarantee: nothrow.
     */
    bool isGameOver() const;
       
    /**
     * @brief tryExitBus will attempt to take p out of the bus
     * @param p player that will try to exit bus
     * @param pn setting variable; if p is player2 and number of players is 1,
     * then set pn=2 otherwise pn=1 by default
     * @pre game is in gamestate, player p is in a bus
     * @post if the bus is at a stop the player will exit In the case of pn=2,
     * the algoritm chooses whether the player will enter
     * Exception guarantee: basic
     * @return true if player entered stop, false if not
     */
    bool tryExitBus(std::shared_ptr<Player> p, int pn=1);
    
    /**
     * @brief tryEnterBus
     * @param p player that will try to enter bus
     * @param pn setting variable; if p is player2 and number of players is 1,
     * then set pn=2 otherwise pn=1 by default
     * @pre game is in gamestate, player p is at a stop
     * @post if there was a bus at the stop, the player will enter it.
     * In the case of pn=2 the algoritm chooses whether the player will enter.
     * In this case player2 may also walk to another stop.
     * Exception guarantee: basic
     * @return true if player entered bus, false if not
     */
    bool tryEnterBus(std::shared_ptr<Player> p, int pn=1);

    /**
     * @brief givePoints gets points from statistics and gives them forward
     * @return current points
     * @post Exception guarantee: strong
     */
    int givePoints();

    /**
     * @brief giveTime
     * @pre time has been initialized
     * @return current time
     * @post Exception guarantee: strong
     */
    QTime giveTime();

private:
    /**
      * @brief handles automated functions of players and updates their status
      */
    void advance_players();
    /**
      * @brief checks whether a location is outside game area
      * @param Interface:: Location the location to check
      */
    bool isOutsideGamearea(Interface::Location) const;

    /**
      * @brief checks whether players are in the game area and if they have
      * escaped via a bus, return them to last known stop
      */
    void playersInGamearea();

    /**
      * @brief Updates the players on whether they have entered a stop
      * or exited a stop
      * @param std::shared_ptr<Player> the player to check
      */
    void changeStopStatus(std::shared_ptr<Player> p);

    /**
      * @brief player2 will try to walk to a nearby stop
      */
    void tryWalktoStop();

    int state_;
    Actors actors_;
    Stops stops_;
    QTime gameclock_;
    QTimer playertimer_;
    std::shared_ptr<StudentSide::Player> player1_;
    std::shared_ptr<StudentSide::Murderer> player2_;

    int playernum_;
    std::unique_ptr<statistics> statistics_;
    QImage background_;

};

}
#endif // CITY_HH

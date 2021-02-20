#ifndef PLAYER1_HH
#define PLAYER1_HH
#include "interfaces/iactor.hh"
#include "core/location.hh"
#include "actors/passenger.hh"
#include "actors/nysse.hh"
#include <memory>

/**
  * @file
  * @brief Defines a class that defines a player and its attributes in the
  * game.
  */

namespace StudentSide {

class Player : public Interface::IPassenger
{
public:
    /**
     * @brief constructor
     * @param start_loc_ starting location
     * @post a player at start_loc_
     */
    Player(Interface::Location start_loc_);
    /**
     * @brief destructor
     */
    ~Player();

    /**
     * @brief giveLocation returns the location of the player.
     * @pre -
     * @return Actors location.
     * @post Exception guarantee: strong.
     */
    Interface::Location giveLocation() const;

    /**
     * @brief move-method moves the actor to given location.
     * @param loc Actors new location.
     * @pre -
     * @post Actors location is loc. Exception guarantee: strong.
     */
    void move(Interface::Location loc);

    /**
     * @brief isRemoved
     * @return true if player is removed else false
     * @post Exception guarantee: no throw
     */
    bool isRemoved() const;

    /**
     * @brief remove sets removed_ to true
     * @post Exception guarantee: no throw
     */
    void remove();

    /**
     * @brief isInVehicle
     * @return true if player is in bus, else false
     * @post Exception guarantee: no throw
     */
    bool isInVehicle() const;

    /**
     * @brief getStop
     * @return current stop or if player is not on stop nullptr
     * @post Exception guarantee: no throw
     */
    std::shared_ptr<Interface::IStop> getStop() const;

    /**
     * @brief getlastknownStop
     * @return The last stop the player was on
     * @post Exception guarantee: no throw
     */
    std::shared_ptr<Interface::IStop> getlastknownStop() const;

    /**
     * @brief getVehicle
     * @pre player is in bus
     * @return the bus the player is in
     * @post Exception guarantee: strong
     */
    std::shared_ptr<Interface::IVehicle> getVehicle() const;

    /**
     * @brief enterNysse
     * @param bus to enter
     * @post player enters the bus, Exception guarantee: strong
     */
    void enterNysse(std::weak_ptr< CourseSide::Nysse > bus);

    /**
     * @brief enterStop
     * @param stop stop to enter
     * @post player enters the stop, Exception guarantee: strong
     */
    void enterStop(std::shared_ptr< Interface::IStop > stop);

    /**
     * @brief leftStop
     * @post player leaves the stop its currently at,
     * Exception guarantee: strong
     */
    void leftStop();

protected:
    Interface::Location loc_;
    std::weak_ptr<CourseSide::Nysse> nyssep_;
    std::weak_ptr< Interface::IStop > stop_;
private:
    bool removed_;
    std::weak_ptr< Interface::IStop > lastknownstop_;


};
}
#endif // PLAYER1_HH

#ifndef MURDERER_HH
#define MURDERER_HH
#include "player.hh"
#include <vector>

/**
  * @file
  * @brief Defines a class that extends a player to murderer and
  * its attributes in the game.
  */
namespace StudentSide{

class Murderer : public Player
{
    int const ALERT_LIMIT = 150;

public:
    /**
     * @brief Constructor
     * @param start_loc initial location
     * @post murderer is in loction start_loc
     */
    Murderer(Interface::Location start_loc);

    /**
      * @brief Destructor
      */
    ~Murderer();

    /**
     * @brief murdered if possible murders a passenger
     * @return passenger that was murdered, if not possible return nullptr
     * @post Exception guarantee: strong
     * */
    std::shared_ptr<Interface::IPassenger> murdered() const;

    /**
     * @brief getPlayer1 gives the player
     * @param player1
     * @post Exception guarantee: strong
     */
    void getPlayer1(std::shared_ptr<StudentSide::Player> player1);

    /**
     * @brief wanttoboardbus
     * @param bus to enter
     * @pre murderer is in on a stop
     * @return true if murderer will enter bus
     * @post exception guarantee: strong
     */
    bool wanttoboardbus(std::weak_ptr<CourseSide::Nysse> bus) const;

    /**
     * @brief wanttoexitbus
     * @pre murderer is on a bus
     * @return true if murderer will exit bus
     * @post Exception guarantee: strong
     */
    bool wanttoexitbus() const;

    /**
     * @brief wanttowalktostop
     * @pre murderer is on a stop
     * @param stop to walk to
     * @return true if murderer will walk to stop
     * @post Exception guarantee: strong
     */
    bool wanttowalktostop(std::weak_ptr<Interface::IStop> stop) const;

    /**
     * @brief walktostop set targetstop as stop
     * @pre murderer must be on a stop
     * @param stop to walk to
     * @post Exception guarantee: strong
     */
    void walktostop(std::weak_ptr<Interface::IStop> stop);

    /**
     * @brief walktowardsstop
     * @pre walktostop has been called and targetstop is not expired
     * @return true if walking succeeded else false
     * @post Exception guarantee: basic
     */
    bool walktowardsstop();

private:
    /**
      * @brief determines approximately if the location loc is in the direction
      * of p1
      * @param Interface::Location loc location for checking
      * @return true or false whether direction is good or not
      */
    bool isGoodDirection(Interface::Location loc) const;

    std::shared_ptr<Player> player1_;
    std::weak_ptr<Interface::IStop> targetstop_;
    double speed_ = 5;
};
}
#endif // MURDERER_HH

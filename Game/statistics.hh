#ifndef STATISTICS_HH
#define STATISTICS_HH
#include "../../Course/CourseLib/interfaces/istatistics.hh"
#include <QtGlobal>

/**
  * @file
  * @brief Defines a class that updates points of the player in the game.
  */
namespace StudentSide
{
int const DEFAULT_POINTS = 5000;
class statistics : public Interface::IStatistics
{

public:
    /**
      * @brief constructor for statistics class which keeps track of statistics
      * in the game
      * @param int points initial points of the player in game
      */
    statistics(int points = DEFAULT_POINTS);

    /**
      * @brief Destructor
      */
    ~statistics();

    /**
      * @brief provides current points of the player
      * @return current points of first player
      */
    int givePoints() const;

    /**
      * @brief reduces points for passengers killed by murderer
      */
    void passengerDied();

    /**
      * @brief reduces points for time of gameplay increasing
      */
    void minutePassed();

    /**
     * @brief morePassengers notifies, that more passangers are added
     * to the game.
     * @param num how many new passangers are added.
     * @pre num > 0
     * @post Exception guarantee: strong
     */
    void morePassengers(int num);

    /**
     * @brief newNysse notifies, that a new nysse is added to the game.
     * @pre -
     * @post Exception guarantee: strong
     */
    void newNysse();

    /**
     * @brief nysseLeft notifies, that a nysse has left the game.
     * @pre -
     * @post Exception guarantee: strong
     */
    void nysseLeft();

    /**
     * @brief nysseRemoved notifies, that the nysse is removed ingame.
     * @pre -
     * @post Exception guarantee: strong
     */
    void nysseRemoved();
private:
    int points_;
    int passengers_;
    int nysse_;  
};
}


#endif // STATISTICS_HH

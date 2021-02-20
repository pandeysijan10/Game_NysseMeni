#ifndef ACTORGUI_HH
#define ACTORGUI_HH

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>

/**
  * @file
  * @brief Defines a class that creates graphicsitem for actors in game
  */

namespace StudentSide{
//width and height for the bounding rectangle for graphics item
const int WIDTH = 30;
const int HEIGHT = 30;

enum {STOP, BUS, PLAYER1, PLAYER2};

class ActorGUI : public QGraphicsItem
{
public:
    /**
     * @brief creates a graphicsitem for actors to draw on screen
     * @param int x x position of actorgui
     * @param int y y position of actorgui
     * @param int type type of the actor to be created based on enum
     * @param int passengers number of passengers for bus and stops actors
     * @pre ActorGUI is in init state.
     * @post ActorGUI is created for placing on screen.
     * @exception InitError x, y positions were invalid or out of the screen
     * scope, invald type of player
     */
    ActorGUI(int x, int y, int type, int passengers = 0);

    /**
      * @brief Destructor
      */
    ~ActorGUI();

    /**
      * @brief Creates the bounding rectangle for graphicsitem
      * @pre -
      * @return QRectF rectangle which defines the boundary for graphicsitem
      * @post QRectF bounding rectangle is created
      */
    QRectF boundingRect() const;

    /**
      * @brief Paints the images for each actorGUI
      * @pre ActorGUI is successfully created and has valid type
      * @post background is painted for actorGUI graphicsitem
      */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    /**
      * @brief Sets coordinates for the graphicsitem
      * @pre x and y are within scope of grahicsview scene boundaries
      * @post coordinates are set for the actorgui graphicsitem
      * Exception guarantee: basic
      */
    void setCoord(int x, int y);

    /**
      * @brief Returns current coordinates of actorgui graphicsitem
      * @pre ActorGUI is in well defined state
      * @return vector of x and y coordinates of actorgui graphicsitem
      * @post coordinates are returned, Exception guarantee: nothrow.
      */
    std::vector<int> getCoord();

    /**
      * @brief Returns type of actor graphicsitem is associated
      * @pre ActorGUI is in well defined state
      * @return type of the actor represented by the graphicsitem
      * @post type of the actor is returned, Exception guarantee: nothrow.
      */
    int getType();

private:
    int x_;
    int y_;
    int type_;
    int passengers_;
};

}
#endif // ACTORGUI_HH

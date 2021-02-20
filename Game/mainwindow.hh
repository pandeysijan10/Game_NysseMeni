#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include "city.hh"
#include "actorgui.hh"

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>

/**
  * @file
  * @brief Defines a class that defines and  creates the main gamewindow and
  * its elements
  */
namespace Ui {
class MainWindow;
}
namespace StudentSide {
enum {SINGLEPLAYER, MULTIPLAYER};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    /**
      * @brief Constructor
      */
    explicit MainWindow(std::shared_ptr<StudentSide::City> city,
                        QWidget *parent = nullptr);
    /**
      * @brief Destructor
      */
    ~MainWindow();

    /**
      * @brief inital setup of the game
      * @pre city and players are initialized
      * @post game timer is started and game is initialized for drawing actors
      * Exception guarantee: no guarantee.
      */
    void setup();

    /**
      * @brief starts timer for game updates
      * @pre -
      * @post timer is started. Exception guarantee: nothrow.
      */
    void startTimer();

private slots:
    /**
      * @brief updates the game scene with signal from timer
      */
    void update();

public slots:
    /**
      * @brief Sets the game type: single or multi player
      * @param QString type game type
      */
    void setGameType(QString type);

    /**
      * @brief Sets the player name
      * @param QString name
      */
    void setPlayername(QString name);

private:
    Ui::MainWindow *ui;
    QGraphicsScene *map;
    QTimer *timer;
    std::shared_ptr<StudentSide::City> city_;
    QVector<ActorGUI*> actors_;
    int width_ = 500; //pxls
    int height_ = 500;
    int tick_ = 10; //ms
    std::pair<std::shared_ptr<StudentSide::Player>,
    std::shared_ptr<StudentSide::Murderer>> players_;
    int game_type_ = SINGLEPLAYER;
    QString player_name_;

    /**
      * @brief listener for key presses
      * @param QKeyEvent* event key press event
      */
    void keyPressEvent(QKeyEvent* event);

    /**
      * @brief adds actors graphics items to scene
      * @param int locX x coordinate of actor
      * @param int locY y coordinate of actor
      * @param int type type of actor
      * @param int passenger passenegr number associated with actor if
      * actors are buses or stops
      */
    virtual void addActor(int locX, int locY, int type,
                          int passenger = 0);

    /**
      * @brief sets background of city in the scene
      * @param QImage img image of the city game area
      */
    void setBackground(QImage img);

    /**
      * @brief updates all the actors graphicsitem to the scene
      */
    void actorsUpdate();

    /**
      * @brief writes scores to a file
      */
    void write_scores(int points);
};
}
#endif // MAINWINDOW_HH

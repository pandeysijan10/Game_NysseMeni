#include "mainwindow.hh"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QKeyEvent>
#include <string>
#include <fstream>

namespace StudentSide {
const int MAPWIDTH = 1095;
const int MAPHEIGHT = 592;
const int STEP = 1; //step a player moves on press of key
const std::string FILE = "scores_file.txt";

MainWindow::MainWindow(std::shared_ptr<StudentSide::City> city,
                       QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    city_(city)
{
    ui->setupUi(this);
    ui->gameView->setFixedSize(width_,height_);
    map = new QGraphicsScene(this);
    ui->gameView->setScene(map);
    map->setSceneRect(0, 0, MAPWIDTH, MAPHEIGHT);
    ui->endButton->setEnabled(false);
    ui->infoText->clear();
    ui->infoTextMurderer->clear();
    this->setBackground(city_->giveBackground());
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &StudentSide::MainWindow::update);
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::blue);
    ui->scoreLCD->setAutoFillBackground(true);
    ui->scoreLCD->setPalette(pal);
    ui->timeLCD->setAutoFillBackground(true);
    ui->timeLCD->setPalette(pal);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    StudentSide::ActorGUI* player1;
    StudentSide::ActorGUI* player2;
    if(!city_->isGameOver())
    {
        ui->infoText->clear();
        ui->infoTextMurderer->clear();
        for(auto it:actors_)
        {
            if(it->getType()==PLAYER1)
            {
                player1 = it;
            }
            if(game_type_==MULTIPLAYER)
            {
                if(it->getType()==PLAYER2)
                {
                    player2 = it;
                }
            }
        }

        //Player 1 movement
        if(event->key() == Qt::Key_D)
        {
            if(player1->x()<MAPWIDTH-WIDTH)
            {
                player1->setCoord(player1->x()+STEP, player1->y());
                Interface::Location n_location;
                n_location.setXY(player1->x(), player1->y());
                players_.first->move(n_location);
                if(game_type_==SINGLEPLAYER){
                   ui->gameView->centerOn(player1->pos());
                }
            }
        }
        if(event->key() == Qt::Key_S)
        {
            if(player1->y()<MAPHEIGHT-HEIGHT)
            {
                player1->setCoord(player1->x(), player1->y()+STEP);
                Interface::Location n_location;
                n_location.setXY(player1->x(), player1->y());
                players_.first->move(n_location);
                if(game_type_==SINGLEPLAYER){
                   ui->gameView->centerOn(player1->pos());
                }
            }
        }
        if(event->key() == Qt::Key_A)
        {
            if(player1->x()>0)
            {
                player1->setCoord(player1->x()-STEP, player1->y());
                Interface::Location n_location;
                n_location.setXY(player1->x(), player1->y());
                players_.first->move(n_location);
                if(game_type_==SINGLEPLAYER){
                   ui->gameView->centerOn(player1->pos());
                }
            }
        }
        if(event->key() == Qt::Key_W)
        {
            if(player1->y()>0)
            {
                player1->setCoord(player1->x(), player1->y()-STEP);
                Interface::Location n_location;
                n_location.setXY(player1->x(), player1->y());
                players_.first->move(n_location);
                if(game_type_==SINGLEPLAYER)
                {
                   ui->gameView->centerOn(player1->pos());
                }
            }
        }
        if(event->key() == Qt::Key_E)
        {
            if(!players_.first->isInVehicle())
            {
                bool signal = false;
                if(players_.first->getStop() != nullptr)
                {
                    signal = city_->tryEnterBus(players_.first);
                }
                else
                {
                    ui->infoText->setText("Hero is not at the stop");
                    signal = true;
                }
                if(!signal)
                {
                    ui->infoText->setText("Hero: No buses at the moment to "
                                          "enter");
                }
            }
            else
            {
                bool signal = false;
                if(players_.first->isInVehicle())
                {
                    signal = city_->tryExitBus(players_.first);
                }
                if(!signal)
                {
                    ui->infoText->setText("Hero couldn't exit the bus");
                }
            }
        }

        // Player 2 movement
        if(game_type_==MULTIPLAYER)
        {
            if(event->key() == Qt::Key_L)
            {
                if(player2->x()<MAPWIDTH-WIDTH)
                {
                    player2->setCoord(player2->x()+STEP, player2->y());
                    Interface::Location n_location;
                    n_location.setXY(player2->x(), player2->y());
                    players_.second->move(n_location);
                }
            }
            if(event->key() == Qt::Key_K)
            {
                if(player2->y()<MAPHEIGHT-HEIGHT)
                {
                    player2->setCoord(player2->x(), player2->y()+STEP);
                    Interface::Location n_location;
                    n_location.setXY(player2->x(), player2->y());
                    players_.second->move(n_location);
                }
             }
            if(event->key() == Qt::Key_J)
            {
                if(player2->x()>0)
                {
                    player2->setCoord(player2->x()-STEP, player2->y());
                    Interface::Location n_location;
                    n_location.setXY(player2->x(), player2->y());
                    players_.second->move(n_location);
                }
            }
            if(event->key() == Qt::Key_I)
            {
                if(player2->y()>0)
                {
                    player2->setCoord(player2->x(), player2->y()-STEP);
                    Interface::Location n_location;
                    n_location.setXY(player2->x(), player2->y());
                    players_.second->move(n_location);
                }
              }
             if(event->key() == Qt::Key_O)
             {
                if(!players_.second->isInVehicle())
                {
                    bool signal = false;
                    if(players_.second->getStop() != nullptr)
                    {
                        signal = city_->tryEnterBus(players_.second);
                    }
                    else
                    {
                        ui->infoTextMurderer->setText("Murderer is not "
                                                      "at the stop");
                        signal = true;
                    }
                    if(!signal)
                    {
                        ui->infoTextMurderer->setText("Murderer: "
                                                      "No buses at "
                                                      "the moment to enter");
                    }
                }
                else
                {
                    bool signal = false;
                    if(players_.second->isInVehicle())
                    {
                        signal = city_->tryExitBus(players_.second);
                    }
                    if(!signal)
                    {
                        ui->infoTextMurderer->setText("Murderer couldn't "
                                                      "exit the bus");
                    }
                }
            }
        }

    }
}

void MainWindow::addActor(int locX, int locY, int type, int passenger)
{
    ActorGUI* nActor = new ActorGUI(locX, locY, type, passenger);
    actors_.push_back(nActor);
    map->addItem(nActor);
    if(game_type_==SINGLEPLAYER && type==PLAYER1)
    {
        ui->gameView->centerOn(nActor->pos());
    }
}

void MainWindow::setup()
{
    int player_num;
    if(game_type_ == SINGLEPLAYER)
    {
        player_num = 1;
    }
    else
    {
        player_num = 2;
    }

    players_ = city_->givePlayers(player_num);
    this->startTimer();
}

void MainWindow::startTimer()
{
    timer->start(tick_);
}

void MainWindow::setBackground(QImage img)
{
    QPixmap image = QPixmap::fromImage(img);
    map->setBackgroundBrush(image);
}

void MainWindow::update()
{
    for(auto it:actors_)
    {
        delete it;
    }
    actors_.clear();
    int points = city_->givePoints();
    QTime gametime = city_->giveTime();
    ui->timeLCD->display(gametime.toString("h:mm:ss"));
    ui->scoreLCD->display(points);
    if(city_->isGameOver())
    {
        ui->endButton->setEnabled(true);
        timer->stop();
        if(game_type_ == SINGLEPLAYER)
        {
            this->write_scores(points);
        }
        if (points > 0)
        {
            ui->infoText->setText("Game ends, Hero wins");
        }
        else
        {
            ui->infoText->setText("Game ends, Murderer wins");
        }
        ui->infoText->show();
    }
    else
    {
        actorsUpdate();
        if(game_type_ == SINGLEPLAYER)
        {
            if(players_.first->isInVehicle())
            {
            for(auto it:actors_)
            {
                if(it->getType()==PLAYER1)
                {
                    it->hide();
                }
            }
        }
    }
}
}

void MainWindow::setGameType(QString type)
{
    if(type == "Multi Player")
    {
        game_type_ = MULTIPLAYER;
    }
}

void MainWindow::setPlayername(QString name)
{
    if (name == ""){
        player_name_ = "No name";
    }
    else
    {
        player_name_ = name;
    }
}

void MainWindow::actorsUpdate()
{
    Stops stops;
    stops = city_->giveStops();
    for(auto it: stops)
    {
        std::vector<std::shared_ptr<Interface::IPassenger>> s_passenger =
                                                    it->getPassengers();
        int passenger = s_passenger.size();
        this->addActor(it->getLocation().giveX(),
                        it->getLocation().giveY(),
                        STOP, passenger);

    }

    std::vector<std::shared_ptr<CourseSide::Nysse>> buses =
            city_->send_buses();
    for(auto it: buses)
    {
        std::vector<std::shared_ptr<Interface::IPassenger>> b_passenger
                                                        = it->getPassengers();
        int passenger = b_passenger.size();
        this->addActor(it->giveLocation().giveX(),
                       it->giveLocation().giveY(),
                       BUS, passenger);
    }

    this->addActor(players_.first->giveLocation().giveX(),
                   players_.first->giveLocation().giveY(),
                   PLAYER1);
    this->addActor(players_.second->giveLocation().giveX(),
                   players_.second->giveLocation().giveY(),
                   PLAYER2);
}

void MainWindow::write_scores(int points)
{
    std::ofstream file_object(FILE, std::ios::app);
    file_object << player_name_.toStdString() << ";" << std::to_string(points)
                << std::endl;
    file_object.close();
}

}

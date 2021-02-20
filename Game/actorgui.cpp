#include "actorgui.hh"

namespace StudentSide {

ActorGUI::ActorGUI(int x, int y, int type, int passengers):x_(x), 
    y_(y), 
    type_(type),
    passengers_(passengers)
{
    setPos(mapToParent(x_, y_ ));
}

ActorGUI::~ActorGUI()
{

}

QRectF ActorGUI::boundingRect() const
{
    return QRectF(0, 0, WIDTH, HEIGHT);
}


void ActorGUI::paint(QPainter *painter,const QStyleOptionGraphicsItem *option,
                     QWidget *widget)
{
    QRectF bounds = boundingRect();
    if(type_ == STOP){
        QImage img(":/offlinedata/stop.png");
        QString passenger_num = QString::number(passengers_);
        painter->drawImage(bounds, img);
        painter->drawText(bounds.center(), passenger_num);
    }
    else if(type_ == BUS){
        QImage img(":/offlinedata/bus.png");
        QString passenger_num = QString::number(passengers_);
        painter->drawImage(bounds, img);
        painter->drawText(bounds.center(), passenger_num);
    }
    else if(type_ == PLAYER1){
        QImage img(":/offlinedata/hero.png");
        painter->drawImage(bounds, img);
    }
    else if(type_ == PLAYER2){
        QImage img(":/offlinedata/murderer.png");
        painter->drawImage(bounds, img);
    }

}


void ActorGUI::setCoord(int x, int y)
{
    setX( x );
    setY( y );
}

std::vector<int> ActorGUI::getCoord()
{
    std::vector<int> coords;
    coords.push_back(x_);
    coords.push_back(y_);
    return coords;
}

int ActorGUI::getType()
{
    return type_;
}


}


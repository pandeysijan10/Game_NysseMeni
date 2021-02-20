#include "creategame.hh"
#include "city.hh"

std::shared_ptr<Interface::ICity> Interface::createGame()
{
    QImage big_background = QImage(":/offlinedata/offlinedata/kartta_iso_1095x592.png");
    QImage small_background = QImage(":/offlinedata/offlinedata/kartta_pieni_500x500.png");
    auto City=std::shared_ptr<StudentSide::City>(new StudentSide::City);
    City->setBackground(big_background, small_background);
    return City;
}

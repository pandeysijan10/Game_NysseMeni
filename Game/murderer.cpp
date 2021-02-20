#include "murderer.hh"
#include <QDebug>

namespace StudentSide {

Murderer::Murderer(Interface::Location start_loc):
    Player(start_loc)
{
}

Murderer::~Murderer()
{

}

std::shared_ptr<Interface::IPassenger> Murderer::murdered() const
{
    if( !nyssep_.expired() )
    {
        std::vector< std::shared_ptr< Interface::IPassenger >> passengers
                = nyssep_.lock()->getPassengers();
        if( !passengers.empty() )
        {
            return passengers.at(0);
        }
    }
    return nullptr;
}

void Murderer::getPlayer1(std::shared_ptr<Player> player1)
{
    player1_ = player1;
}

bool Murderer::wanttoboardbus(
        std::weak_ptr<CourseSide::Nysse> bus) const
{
    Q_ASSERT(!stop_.expired());
    if( player1_->giveLocation().isClose(giveLocation(), ALERT_LIMIT) )
    {

        //calculate whether player1 is in direction bus is heading.
        return isGoodDirection(bus.lock()->getFinalStop().lock()->getLocation());
    }
    else
    {
        return true;
    }
}

bool Murderer::wanttoexitbus() const
{
    Q_ASSERT(isInVehicle());

    if( player1_->giveLocation().isClose(giveLocation(), ALERT_LIMIT) )
    {
        //calculate whether player1 is in direction bus is heading.
        return isGoodDirection(nyssep_.lock()->getFinalStop().lock()->getLocation());
    }
    else
    {
        if( rand() % 4 == 0 )
        {
            return false;
        }
        return true;
    }
}

bool Murderer::wanttowalktostop(
        std::weak_ptr<Interface::IStop> stop) const
{
    Q_ASSERT(stop.lock()!=nullptr);
    if(stop.lock()==stop_.lock())
    {
        return false;
    }
    Interface::Location p1_loc = player1_->giveLocation();
    // To make movements sufficiently frequent some random numbers are used.
    // The frequencies have been chosen somewhat arbitarily
    if( p1_loc.isClose(giveLocation(), ALERT_LIMIT) && rand() % 10 == 0)
    {
        return isGoodDirection(stop.lock()->getLocation());
    }
    else
    {
        if (rand() % 500 == 0)
        {
            return true;
        }
        return false;
    }
}

void Murderer::walktostop(std::weak_ptr<Interface::IStop> stop)
{
    Q_ASSERT(stop.lock()!=nullptr);
    targetstop_ = stop;
}

bool Murderer::walktowardsstop()
{
    Q_ASSERT(!targetstop_.expired());
    double dtotravel = Interface::Location::calcDistance(
                giveLocation(),targetstop_.lock()->getLocation());
    // if the murderer is on the stop it was travelling to update this
    if( dtotravel==0 )
    {
        stop_ = targetstop_;
        targetstop_.reset();
        return false;
    }
    else
    {
        // calculate the ratio of the distance to travel and speed
        double ratio;
        // ratio must be less than 1
        if(dtotravel < speed_)
        {
            ratio = 1;
        }
        else
        {
            ratio = speed_ / dtotravel;
        }
        // new location set depending on this ratio
        loc_ = Interface::Location::calcBetween(
                    giveLocation(),targetstop_.lock()->getLocation(),ratio);
        return true;
    }

}

bool Murderer::isGoodDirection(Interface::Location loc) const
{
    // new location if travelling a short distance toward loc:
    Interface::Location loc0 = Interface::Location::calcBetween(loc_,loc, 0.01);
    // checking whether this new location is closer to player1
    if(Interface::Location::calcDistance(player1_->giveLocation(), loc0) <=
            Interface::Location::calcDistance(player1_->giveLocation(),loc_))
    {
        return false;
    }
    else
    {
        return true;
    }
}

}





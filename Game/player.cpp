#include "player.hh"

namespace StudentSide {

Player::Player(Interface::Location start_loc_):
    loc_(start_loc_),
    removed_(false),
    lastknownstop_()
{
}

Player::~Player()
{
}

Interface::Location Player::giveLocation() const
{    
    if ( nyssep_.expired() )
    {
        return loc_;
    }
    // if player is in bus return bus' location
    return nyssep_.lock()->giveLocation();
}

void Player::move(Interface::Location loc)
{
    if( !isInVehicle() )
    {
        loc_=loc;
    }
    else
    {
        // if player is in a bus and its removed, move to the new location
        // otherwise player wont be moved.
        if(nyssep_.lock()->isRemoved())
        {
            loc_=loc;
            nyssep_.reset();
        }
    }
}

bool Player::isRemoved() const
{
    return removed_;
}

void Player::remove()
{
    removed_ = true;
}

bool Player::isInVehicle() const
{
    if( nyssep_.expired() )
    {
        return false;
    }
    return true;
}

std::shared_ptr<Interface::IStop> Player::getStop() const
{
    return stop_.lock();
}

std::shared_ptr<Interface::IStop> Player::getlastknownStop() const
{
    return lastknownstop_.lock();
}

std::shared_ptr<Interface::IVehicle> Player::getVehicle() const
{
    Q_ASSERT(!nyssep_.expired());
    return nyssep_.lock();
}

void Player::enterNysse(std::weak_ptr< CourseSide::Nysse > bus)
{
    nyssep_ = bus;
    stop_.reset();
}
void Player::enterStop(std::shared_ptr< Interface::IStop > stop)
{
    nyssep_.reset();
    loc_ = stop->getLocation();
    stop_ = stop;
    lastknownstop_ = stop_;
}

void Player::leftStop()
{
    stop_.reset();
}

}

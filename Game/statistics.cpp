#include "statistics.hh"

namespace StudentSide {

statistics::statistics(int points):
        points_(points),
        passengers_(0),
        nysse_(0)
{

}

statistics::~statistics()
{
}

int statistics::givePoints() const
{
    return points_;
}

void statistics::passengerDied()
{
    points_-=100;
}

void statistics::minutePassed()
{
    points_-=10;
}

void statistics::morePassengers(int num)
{
    Q_ASSERT(num>0);
    passengers_+=num;
}

void statistics::newNysse()
{
    nysse_+=1;
}

void statistics::nysseLeft()
{
    nysse_-=1;
}

void statistics::nysseRemoved()
{
    nysse_-=1;
}

}







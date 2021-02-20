#include "city.hh"
#include <memory>
#include <QDebug>

namespace StudentSide {

City::City():
    state_( INIT_STATE ),
    actors_(),
    stops_(),
    gameclock_(),
    player1_(std::shared_ptr<Player>( new Player(P1_START_LOC) )),
    player2_(std::shared_ptr<Murderer>( new Murderer(P2_START_LOC) )),
    statistics_(std::unique_ptr<statistics>( new statistics ))

{
    
}

City::~City()
{
    
}

void City::setBackground( QImage &bigbackground,
                                      QImage &smallbackground )
{
    Q_ASSERT(state_==INIT_STATE);

    //If the co-ordinate 1,1 is in the background, then it is valid,
    //Choose big background if possible, else smallbackground
    if( bigbackground.valid(1,1) )
    {
        background_=bigbackground;

    }
    else if( smallbackground.valid(1,1) )
    {
        background_=smallbackground;

    }
    else
    {
        throw Interface::InitError("Background not valid");
    }
}

void City::setClock( QTime clock )
{
    Q_ASSERT(clock.isValid());
    gameclock_=clock; 

    if( state_==GAME_STATE )
    {
        statistics_->minutePassed();
    }
}

void City::addStop( std::shared_ptr<Interface::IStop> stop )
{
    Q_ASSERT(state_==INIT_STATE);
    if( !isOutsideGamearea(stop->getLocation()) )
    {
        stops_.push_back(stop);
    }
}

void City::startGame()
{
    Q_ASSERT(state_==INIT_STATE);
    Q_ASSERT(gameclock_.isValid());
    Q_ASSERT(player1_!=nullptr);
    Q_ASSERT(player2_!=nullptr);

    state_=GAME_STATE;
    changeStopStatus(player2_);
    QObject::connect(&playertimer_,&QTimer::timeout,[=](){advance_players();});
    playertimer_.start(UPDATE_INTERVAL_MS);

}

QImage City::giveBackground() const
{
    Q_ASSERT(state_==INIT_STATE);
    return background_;
}

std::pair< std::shared_ptr< StudentSide::Player >,
std::shared_ptr< StudentSide::Murderer > >
City::givePlayers(int playernum)
{
    Q_ASSERT(state_==INIT_STATE);

    // for ai decisions player2_ needs to have player1_
    if(playernum==1)
    {
        player2_->getPlayer1(player1_);
    }
    playernum_=playernum;
    return std::make_pair(player1_,player2_);
}


StudentSide::Stops City::giveStops() const
{
    Q_ASSERT(stops_.at(0) != nullptr);
    return stops_;
}


std::vector<std::shared_ptr<CourseSide::Nysse>>
City::send_buses() const
{
    std::vector<std::shared_ptr<CourseSide::Nysse>> buses;
    for(auto actor : actors_)
    {
        if( std::shared_ptr<CourseSide::Nysse> bus =
                std::dynamic_pointer_cast<CourseSide::Nysse>(actor) )
        {
            buses.push_back(bus);
        }
    }
    return buses;
}


void City::addActor( std::shared_ptr<Interface::IActor> newactor )
{
    if( !findActor(newactor) )
    {
        actors_.push_back(newactor);
    }
    else
    {
        throw Interface::GameError("Actor is already in the city.");
    }
}

void City::removeActor( std::shared_ptr<Interface::IActor> actor )
{
    Q_ASSERT(state_==GAME_STATE);
    if( !findActor(actor) )
    {
        throw Interface::GameError("actor not in city");
    }
    actor->remove();
    actorRemoved(actor);
}

void City::actorRemoved(std::shared_ptr<Interface::IActor> actor)
{
    Q_ASSERT(state_==GAME_STATE);
    Q_ASSERT(actor->isRemoved());
    Q_ASSERT(findActor(actor));
    actors_.erase( std::find(actors_.begin(), actors_.end(), actor) );
}

bool City::findActor(std::shared_ptr<Interface::IActor> actor) const
{
    if (std::find(actors_.begin(), actors_.end(), actor)!=actors_.end())
    {
        return true;
    }
    return false;

}

void City::actorMoved(std::shared_ptr<Interface::IActor> actor)
{
    //This methods information is not needed, but is included from the interface
    Q_ASSERT(state_==GAME_STATE);
    Q_ASSERT(findActor(actor));
}

std::vector<std::shared_ptr<Interface::IActor> >
City::getNearbyActors(Interface::Location loc) const
{
    Q_ASSERT(state_==GAME_STATE);
    Actors nearby_actors = {};

    for(std::shared_ptr<Interface::IActor> actor : actors_)
    {
        if( actor->giveLocation().isClose(loc) )
        {
            nearby_actors.push_back(actor);
        }
    }
    return nearby_actors;
}

std::vector<std::shared_ptr<Interface::IStop>>
City::getNearbyStops(Interface::Location loc, int dist) const
{
    Q_ASSERT(dist >= 0);
    Stops nearby_stops = {};
    for(std::shared_ptr<Interface::IStop> stop : stops_)
    {
        if(stop->getLocation().isClose(loc, dist))
        {
            nearby_stops.push_back(stop);
        }
    }
    return nearby_stops;
}


bool City::isGameOver() const
{
    Q_ASSERT(state_==GAME_STATE);
    // Game ends if the hero catches the murderer or the points run out
    if(player1_->giveLocation().isClose(player2_->giveLocation(), WIN_RADIUS))
    {
        return true;   
    }
    else if(statistics_->givePoints() < 0)
    {
        return true;
    }
    return false;
}

bool City::tryExitBus(std::shared_ptr<StudentSide::Player> p, int pn)
{
    Q_ASSERT(state_==GAME_STATE);
    Q_ASSERT(p->isInVehicle());

    std::shared_ptr <CourseSide::Nysse> bus =
            std::dynamic_pointer_cast<CourseSide::Nysse>(p->getVehicle());
    std::shared_ptr <Interface::IStop> stop = bus->getStop().lock();

    // player can not exit to the stop they just left
    if (stop != nullptr && stop != p->getlastknownStop())
    {

        //p is player 2 and game is singleplayer ask the ai
        if( pn == 2 )
        {
            if(player2_->wanttoexitbus())
            {
                p->enterStop(stop);
                return true;
            }
        //otherwise just enter the stop
        }
        else
        {
            p->enterStop(stop);
            return true;
        }
    }
    return false;
}

bool City::tryEnterBus(std::shared_ptr<Player> p, int pn)
{
    Q_ASSERT( state_ == GAME_STATE );
    Q_ASSERT(!p->isInVehicle());
    Q_ASSERT( p->getStop()!=nullptr );

    // Going through all actors and taking from them the buses to check if they
    // are on the same stop as the player
    for( auto actor : actors_ )
    {
        if( std::shared_ptr<CourseSide::Nysse> bus =
                std::dynamic_pointer_cast<CourseSide::Nysse>(actor) )
        {
            std::shared_ptr<CourseSide::Stop> stop = bus->getStop().lock();

            if( stop!=nullptr )
            {
                if( stop->getLocation()==p->getStop()->getLocation() )
                {

                    // p is player 2 and mode is singleplayer ask ai
                    if( pn == 2 )
                    {
                        if( player2_->wanttoboardbus(bus) )
                        {
                            p->enterNysse(bus);
                            return true;
                        }
                    // otherwise just enter the bus
                    }
                    else
                    {
                        p->enterNysse(bus);
                        return true;
                    }
                }
            }
        }
    }
    // if player2_ did not board a have them attempt walking to a stop
    if( pn==2 )
    {
        tryWalktoStop();
    }
    return false;
}


int City::givePoints()
{
    return statistics_->givePoints();
}

QTime City::giveTime()
{
    Q_ASSERT(gameclock_.isValid());
    return gameclock_;
}


void City::advance_players()
{
    Q_ASSERT(state_==GAME_STATE);
    if( isGameOver() )
    {
        playertimer_.stop();
        return;
    }
    if( !player1_->isInVehicle() )
    {
        changeStopStatus(player1_);
    }
    if( playernum_==2 && !player2_->isInVehicle() )
    {
        changeStopStatus(player2_);
    }

    // automated murdering:
    if( player2_->isInVehicle() )
    {

        std::shared_ptr<Interface::IPassenger> dead = player2_->murdered();
        if( dead!=nullptr && findActor(dead) )
        {
            removeActor(dead);
            statistics_->passengerDied();
        }
    }
    // Ai player actions if not in multiplayer mode
    if( playernum_==1 )
    {
        if( player2_->isInVehicle() )
        {
            tryExitBus(player2_,2);
        }
        else if( player2_->getStop()!=nullptr )
        {
            tryEnterBus(player2_,2);
        }
        else
        {
            player2_->walktowardsstop();
        }
    }
    // Ensure that players stay in gamearea
    playersInGamearea();
}

bool City::isOutsideGamearea(Interface::Location loc) const
{
    if((( loc.giveX() >= background_.width() ) | ( loc.giveX() < 0 )) |
            (( loc.giveY() >= background_.height() ) | ( loc.giveY() < 0 )))
    {
        return true;
    }
    return false;
}


void City::playersInGamearea()
{
    if( isOutsideGamearea(player1_->giveLocation()) )
    {
        // handle keeping players in gamearea only if they are in a vehicle
        // walking checks are handled by gamewindow
        if( player1_->isInVehicle() )
        {
            player1_->enterStop( player1_->getlastknownStop() );
        }
    }
    if( isOutsideGamearea(player2_->giveLocation()) )
    {
        // handle only escapes for multiplayer if they happened via a bus
        if( player2_->isInVehicle() && playernum_==2 )
        {
            player2_->enterStop(player2_->getlastknownStop());
        }else if( playernum_==1 ){
            player2_->enterStop(player2_->getlastknownStop());
        }
    }
}

void City::changeStopStatus(std::shared_ptr<StudentSide::Player> p)
{
    Q_ASSERT(state_==GAME_STATE);

    std::shared_ptr<Interface::IStop> stop = p->getStop();

    if( stop != nullptr )
    {
        if( !p->giveLocation().isClose(stop->getLocation(), STOP_RADIUS) )
        {
            p->leftStop();
        }
    }
    else
    {
        // enter the first stop given by getnearbystops
        for( auto stop:getNearbyStops(p->giveLocation(), STOP_RADIUS) )
        {
            p->enterStop(stop);
            break;
        }
    }
}

void City::tryWalktoStop()
{
    for(auto stops:getNearbyStops(player2_->giveLocation(), WALK_RADIUS))
    {
        if(stops!=nullptr)
        {
            if(player2_->wanttowalktostop(stops))
            {
                // tell player2 where it is walking
                player2_->walktostop(stops);
                player2_->leftStop();
                // take the first step in walking
                player2_->walktowardsstop();
                break;
            }
        }
    }
}

}







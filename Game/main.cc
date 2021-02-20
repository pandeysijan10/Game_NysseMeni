#include <QApplication>
#include <QDir>
#include <QObject>
#include <QDebug>

#include "mainwindow.hh"
#include "startdialog.hh"
#include "core/logic.hh"
#include "creategame.hh"
#include "city.hh"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Q_INIT_RESOURCE(offlinedata);
    Q_INIT_RESOURCE(graphics_resources);

    StudentSide::StartDialog dialog;
    auto icity=Interface::createGame();

    std::shared_ptr<StudentSide::City> city =
            std::dynamic_pointer_cast<StudentSide::City>(icity);
    StudentSide::MainWindow window(city);
    QObject::connect(&dialog,
                     SIGNAL(game_type(QString)),
                     &window,
                     SLOT(setGameType(QString)));
    QObject::connect(&dialog,
                     SIGNAL(player_name(QString)),
                     &window,
                     SLOT(setPlayername(QString)));
    if(dialog.exec()==QDialog::Accepted)
    {
        auto logic = new CourseSide::Logic(&window);
        logic->takeCity(icity);
        logic->fileConfig();
        logic->setTime(10,0);  
        window.setup();
        logic->finalizeGameStart();
        window.show();
    }
    return a.exec();
}

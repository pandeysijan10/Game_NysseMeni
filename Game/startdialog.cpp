#include "startdialog.hh"
#include "ui_startdialog.h"
#include <fstream>

namespace StudentSide{
const std::string FILE = "scores_file.txt";

StartDialog::StartDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartDialog)
{
    ui->setupUi(this);
    ui->gameTypeSelect->addItem("Single Player");
    ui->gameTypeSelect->addItem("Multi Player");
    ui->backButton->hide();
    connect(ui->gameTypeSelect,
            SIGNAL(currentIndexChanged(int)),
            this, SLOT(onSelectMultiplayer(int)));
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint|
                         Qt::WindowTitleHint);
}

StartDialog::~StartDialog()
{
    delete ui;
}

void StartDialog::onSelectMultiplayer(int ind)
{
    if(ind == 1)
    {
        ui->playerName->hide();
        ui->playerName->clear();
    }
    else
    {
        ui->playerName->show();
    }
}

void StartDialog::on_ScoresButton_clicked()
{
  ui->summary->setText(read_scores());
  ui->backButton->show();
  ui->playerName->hide();
  ui->StartButton->hide();
  ui->gameTypeSelect->hide();
  ui->ScoresButton->hide();
}

void StartDialog::on_StartButton_clicked()
{
    QString choice = ui->gameTypeSelect->currentText();
    emit this->player_name(ui->playerName->text());
    emit this->game_type(choice);
}

void StartDialog::on_backButton_clicked()
{
    ui->summary->setText(QString::
                         fromStdString("Welcome To NysseMeni! "
                                       "To start the game, "
                                       "select the type of game you "
                                       "want to play and press the start "
                                       "button. Thank You!"));
    ui->backButton->hide();
    ui->playerName->show();
    ui->StartButton->show();
    ui->gameTypeSelect->show();
    ui->ScoresButton->show();
    ui->gameTypeSelect->setCurrentIndex(0);
}

QString StartDialog::read_scores()
{
    std::ifstream file_object(FILE, std::ios::in);
    std::vector<std::pair<int, std::string>> scores;
    if(not file_object)
    {
        return "Oops! Couldn't find the Top-10 list";
    }
    else
    {
        std::string line;
        while(getline(file_object, line))
        {
            std::vector<std::string> line_vec = txt_split(line, ';');
            scores.push_back({std::stoi(line_vec.at(1)), line_vec.at(0)});
        }
        file_object.close();
    }
    int num = 1;
    std::sort(scores.begin(), scores.end());
    std::string list = "Top-10 list:\n\n";
    for(int i = scores.size()-1; i > -1 && num < 11; i-- )
    {
        list = list + std::to_string(num)
               +". "+scores.at(i).second
               +": "+std::to_string(scores.at(i).first)
               +"\n";
        num += 1;
    }
    return QString::fromStdString(list);
}

std::vector<std::string> StartDialog::txt_split(std::string line,
                                                char delimiter)
{
    std::vector<std::string> result = {""};
    bool cont = false;
    for ( char cha : line )
    {
            if ( cha == '"' )
            {
                cont = !cont;
            }
            else if ( cha == delimiter && !cont)
            {
                result.push_back("");
            }
            else
            {
                result.back().push_back(cha);
            }
        }
        if ( result.back() == "" )
        {
            result.erase(--result.end());
        }
        return result;
}

}

#ifndef STARTDIALOG_HH
#define STARTDIALOG_HH

#include <QDialog>

/**
  * @file
  * @brief Defines a class that contains ui for start dialog that gives intro
  * to game.
  */

namespace Ui {
class StartDialog;
}

namespace StudentSide {

class StartDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StartDialog(QWidget *parent = nullptr);
    ~StartDialog();

private slots:
    void onSelectMultiplayer(int ind);
    void on_ScoresButton_clicked();
    void on_StartButton_clicked();
    void on_backButton_clicked();

signals:
    /**
      * @brief signal for the game type(single or multi player)
      */
    void game_type(const QString &type);

    /**
      * @brief signal for the game player name
      */
    void player_name(const QString &name);

private:
    Ui::StartDialog *ui;

    /**
      * @brief reads score list from a file
      * @return top 10 list
      */
    QString read_scores();

    /**
      * @brief splits the text separated by delimiter
      * @param std::string line line to be split
      * @param char delimiter
      * @return top 10 list
      */
    std::vector<std::string> txt_split(std::string line, char delimiter);
};
}
#endif // STARTDIALOG_HH

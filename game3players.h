#ifndef GAME3PLAYERS_H
#define GAME3PLAYERS_H

#include <QDialog>

namespace Ui {
class game3players;
}

class game3players : public QDialog
{
    Q_OBJECT

public:
    explicit game3players(QWidget *parent = nullptr);
    ~game3players();

private slots:
    void on_pushButton_player1_clicked();

    void on_pushButton_player2_clicked();

    void on_pushButton_player3_clicked();

    void on_pushButton_gameName_clicked();

    void on_pushButton_add10_clicked();

    void on_pushButton_take10_clicked();

    void on_pushButton_test_clicked();

    void on_pushButton_musikCard1_clicked();

    void on_pushButton_musikCard2_clicked();

    void on_pushButton_musikCard3_clicked();

    void on_pushButton_playerCard1_clicked();

    void on_pushButton_playerCard2_clicked();

    void on_pushButton_playerCard3_clicked();

    void on_pushButton_playerCard4_clicked();

    void on_pushButton_playerCard5_clicked();

    void on_pushButton_playerCard6_clicked();

    void on_pushButton_playerCard7_clicked();

    void on_pushButton_playerCard8_clicked();



    void on_pushButton_pick_clicked();

    void on_pushButton_pass_clicked();

private:
    Ui::game3players *ui;
};

#endif // GAME3PLAYERS_H

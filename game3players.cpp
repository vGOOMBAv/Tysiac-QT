#include "game3players.h"
#include "ui_game3players.h"
#include "carddeck.h"
#include "gameiterationscore.h"
#include "player.h"
#include <QDebug>
#include <QPixmap>
#include <algorithm>
#include <QHBoxLayout>
#include <QLabel>
#include <QtSql>
#include <QtDebug>
#include <QSqlDatabase>
#include <unistd.h>

bool probabilityGen();

bool flag_p1name;
bool flag_p2name;
bool flag_p3name;
bool flag_gameName;
bool flag_cardForFirstPlayer=true;
bool flag_cardForSecondPlayer=true;
bool flag_startSuit=true;
bool flag_firstMove=true;
bool flag_fpContract=false;
bool flag_spContract=false;
bool flag_tpContract=false;


void incrimentCurrentPlayer(size_t& currentPlayer);
void addCardToPlayer(std::vector<Player>& players, Card card, size_t& currentPlayer);
void generateGameDecks();
//void gameIteration();
void showPlayersCard(Player& player);
void showMusikCards();
void setIcon(Card& card, QLabel* label);
void setValues(Card& card, QLabel* label1, QLabel* label2);
void gameFunction();
void musikCardBehaviour(QGroupBox* gb_ptr);
void playerCardBehaviour(QGroupBox* gb_ptr);
bool haveSuit(std::vector<Card>& array);
size_t endRound();
void showChosenCard(Card& card);
void countPoints();
void scoreToAchieveButtonBehaviour();
void scoreFlagChange();
size_t returnScoreBonus(Player& player);
size_t absValue(size_t& value);

std::vector<Card> musik;
std::vector<Player> players;
std::vector<Card> cardsOnTheTable;
std::vector<size_t> choosenMusikCards;
std::vector<bool> playersWithMaxScore;
//std::vector<GameIterationScore> gameScore;
CardDeck gameDeck;
Ui::game3players* ui_ptr;
Player* currentPlayer_ptr;

size_t ID;
size_t currentPlayer=0;
size_t currentPlayerForMusik=4;
size_t currentPlayerForPlayerCards=4;
size_t currentPlayerForContract;
size_t currentStarter;
size_t playerIterationCount;
size_t contract;
size_t maxValue;
int8_t playerWithContractID;
int winScore=100;

cardSuit startSuit;

size_t minScoreToAchieve=100;
size_t maxScoreToAchieve=120;
int8_t playersWithMaxScoreA=0;

game3players::game3players(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::game3players)
{
    ui->setupUi(this);
    ui_ptr=ui;
    srand(time(NULL));
    players.resize(3);
    playersWithMaxScore.resize(3);
    playersWithMaxScore[0]=false;
    playersWithMaxScore[1]=false;
    playersWithMaxScore[2]=false;
    ui_ptr->groupBox_card1->hide();
    ui_ptr->groupBox_card2->hide();
    ui_ptr->groupBox_card3->hide();
    ui_ptr->groupBox_card4->hide();
    ui_ptr->groupBox_card5->hide();
    ui_ptr->groupBox_card6->hide();
    ui_ptr->groupBox_card7->hide();
    ui_ptr->groupBox_card8->hide();
    ui_ptr->label_gameName->hide();
    ui_ptr->label_gameName->hide();
    ui_ptr->groupBox_scoreToAchieveControl->hide();
    ui_ptr->groupBox_musikCard1->hide();
    ui_ptr->groupBox_musikCard2->hide();
    ui_ptr->groupBox_musikCard3->hide();
    ui_ptr->label_Info->hide();
    QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("D:/C++ korepetycja/Tysiac/tysiac.db");
    if(!mydb.open()){
        qDebug()<<"-db";
    }else{
        qDebug()<<"kunecta";
    }

}

game3players::~game3players()
{
    delete ui;
}

bool probabilityGen() {
    bool buff = (rand()/(RAND_MAX/100)) < 40;
    return buff;
}

void game3players::on_pushButton_player1_clicked()
{
    flag_p1name=true;
    ui->groupBox_player1->setTitle(ui->textEdit_player1->toPlainText());
    ui->label_player1DB->setText(ui->textEdit_player1->toPlainText());
    players[0]=(*(new Player(ui->groupBox_player1->title())));
    ui->textEdit_player1->hide();
    ui->pushButton_player1->hide();
}

void game3players::on_pushButton_player2_clicked()
{
    flag_p2name=true;
    ui->groupBox_player2->setTitle(ui->textEdit_player2->toPlainText());
    ui->label_player2DB->setText(ui->textEdit_player2->toPlainText());
    players[1]=(*(new Player(ui->groupBox_player2->title())));
    ui->textEdit_player2->hide();
    ui->pushButton_player2->hide();
}

void game3players::on_pushButton_player3_clicked()
{
    flag_p3name=true;
    ui->groupBox_player3->setTitle(ui->textEdit_player3->toPlainText());
    ui->label_player3DB->setText(ui->textEdit_player3->toPlainText());
    players[2]=(*(new Player(ui->groupBox_player3->title())));
    ui->textEdit_player3->hide();
    ui->pushButton_player3->hide();
}

void incrimentCurrentPlayer(size_t& currentPlayerF){
    if(currentPlayerF<3){
        currentPlayerF++;
    }else{
        currentPlayerF=1;
    }
}

void generateGameDecks(){
    players[0].playerCards.clear();
    players[1].playerCards.clear();
    players[2].playerCards.clear();
    musik.clear();
    {
        int8_t i=0;
        while(i<3){
            gameDeck.shuffle();
            i++;
        }
    }
    currentPlayer=1;
    for(size_t i=0;i<gameDeck.deck.size();i++){
        if(probabilityGen()&&musik.size()!=3){
            musik.push_back(gameDeck.deck[i]);
            continue;
        }else{
            size_t iterations=0;
            while(players[currentPlayer-1].playerCards.size()==7){
                incrimentCurrentPlayer(currentPlayer);
                iterations++;
                if(iterations==3){
                    if(musik.size()!=3){
                        musik.push_back(gameDeck.deck[i]);
                    }else{
                        throw std::runtime_error("player hands fill error");
                    }
                    iterations=0;
                }
            }
            players[currentPlayer-1].playerCards.push_back(gameDeck.deck[i]);
            incrimentCurrentPlayer(currentPlayer);
            continue;
        }
    }
}

void showPlayersCard(Player& player){
    ui_ptr->label_currentPlayer->setText(player.name);
    std::sort(player.playerCards.begin(), player.playerCards.end(), [](const Card & a, const Card & b) -> bool{
        return static_cast<int>(a.suit) > static_cast<int>(b.suit);
    });
    ui_ptr->groupBox_card1->hide();
    ui_ptr->groupBox_card2->hide();
    ui_ptr->groupBox_card3->hide();
    ui_ptr->groupBox_card4->hide();
    ui_ptr->groupBox_card5->hide();
    ui_ptr->groupBox_card6->hide();
    ui_ptr->groupBox_card7->hide();
    ui_ptr->groupBox_card8->hide();
    for(size_t i=0;i<player.playerCards.size();i++){
        switch(i){
        case 0:
            setIcon(player.playerCards[i], ui_ptr->label_card1suit);
            setValues(player.playerCards[i], ui_ptr->label_card1value_1, ui_ptr->label_card1value_2);
            ui_ptr->groupBox_card1->show();
            break;
        case 1:
            setIcon(player.playerCards[i], ui_ptr->label_card2suit);
            setValues(player.playerCards[i], ui_ptr->label_card2value_1, ui_ptr->label_card2value_2);
            ui_ptr->groupBox_card2->show();
            break;
        case 2:
            setIcon(player.playerCards[i], ui_ptr->label_card3suit);
            setValues(player.playerCards[i], ui_ptr->label_card3value_1, ui_ptr->label_card3value_2);
            ui_ptr->groupBox_card3->show();
            break;
        case 3:
            setIcon(player.playerCards[i], ui_ptr->label_card4suit);
            setValues(player.playerCards[i], ui_ptr->label_card4value_1, ui_ptr->label_card4value_2);
            ui_ptr->groupBox_card4->show();
            break;
        case 4:
            setIcon(player.playerCards[i], ui_ptr->label_card5suit);
            setValues(player.playerCards[i], ui_ptr->label_card5value_1, ui_ptr->label_card5value_2);
            ui_ptr->groupBox_card5->show();
            break;
        case 5:
            setIcon(player.playerCards[i], ui_ptr->label_card6suit);
            setValues(player.playerCards[i], ui_ptr->label_card6value_1, ui_ptr->label_card6value_2);
            ui_ptr->groupBox_card6->show();
            break;
        case 6:
            setIcon(player.playerCards[i], ui_ptr->label_card7suit);
            setValues(player.playerCards[i], ui_ptr->label_card7value_1, ui_ptr->label_card7value_2);
            ui_ptr->groupBox_card7->show();
            break;
        case 7:
            setIcon(player.playerCards[i], ui_ptr->label_card8suit);
            setValues(player.playerCards[i], ui_ptr->label_card8value_1, ui_ptr->label_card8value_2);
            ui_ptr->groupBox_card8->show();
            break;
        default:
            throw std::out_of_range("player card show");
            break;
        }
    }
}

void setIcon(Card& card, QLabel* label){
    size_t w,h;
    w=label->width();
    h=label->height();
    if(card.suit==cardSuit::Clovers){
        //QPixmap pix(":/card icons/card icons/clovers.png");
        //label->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        label->setText("Clovers");
        return;
    }
    if(card.suit==cardSuit::Hearts){
        //QPixmap pix(":/card icons/card icons/hearts.png");
        //label->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        label->setText("Hearts");
        return;
    }
    if(card.suit==cardSuit::Pikes){
        //QPixmap pix(":/card icons/card icons/pikes.png");
        //label->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        label->setText("Pikes");
        return;
    }
    if(card.suit==cardSuit::Tiles){
        //QPixmap pix(":/card icons/card icons/tiles.png");
        //label->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        label->setText("Tiles");
        return;
    }
}

void setValues(Card& card, QLabel* label1, QLabel* label2){
    if(card.value==cardValue::Nine){
        label1->setText("9");
        label2->setText("9");
        return;
    }
    if(card.value==cardValue::Jack){
        label1->setText("J");
        label2->setText("J");
        return;
    }
    if(card.value==cardValue::Queen){
        label1->setText("Q");
        label2->setText("Q");
        return;
    }
    if(card.value==cardValue::King){
        label1->setText("K");
        label2->setText("K");
        return;
    }
    if(card.value==cardValue::Ten){
        label1->setText("10");
        label2->setText("10");
        return;
    }
    if(card.value==cardValue::Ace){
        label1->setText("A");
        label2->setText("A");
        return;
    }
}

void game3players::on_pushButton_gameName_clicked()
{
    flag_gameName=true;
    ui_ptr->label_gameName->setText(ui_ptr->textEdit_gameName->toPlainText());
    ui->textEdit_gameName->hide();
    ui->pushButton_gameName->hide();
    ui->label_gameName->show();
}


void gameFunction(){
    {
        QSqlQuery query("INSERT INTO games (GAMENAME) VALUES (?)");
        query.addBindValue(ui_ptr->label_gameName->text());
        if(query.exec()){
            qDebug()<<"well";
        }else{
            qDebug()<<"not well"<< query.lastError();
        }
    }
    {
        QSqlQuery query("select * from games where GAMENAME = ?");
        query.addBindValue(ui_ptr->label_gameName->text());
        if(query.exec()){
            qDebug()<<"well";
            while(query.next()){
                ID = static_cast<size_t>(query.value(0).toInt());
            }
        }else{
            qDebug()<<"not well"<< query.lastError();
        }
    }
    qDebug()<<"ID: "<<ID;
    generateGameDecks();
    currentPlayer = static_cast<size_t>(rand()%3+1);
}

void game3players::on_pushButton_add10_clicked()
{
    size_t value;
    value = static_cast<size_t>(ui_ptr->label_scoreToAchieveValue->text().toInt());
    if(value==maxScoreToAchieve){
        QPalette palette = ui_ptr->label_scoreToAchieveValue->palette();
        palette.setColor(ui_ptr->label_scoreToAchieveValue->foregroundRole(), Qt::red);
        ui_ptr->label_scoreToAchieveValue->setPalette(palette);
    }else if(value>maxScoreToAchieve){
        //throw std::out_of_range("value is bigger than maxScore");
    }else{
        value+=10;
        ui->label_scoreToAchieveValue->setText(QString::number(value));
        QPalette palette = ui_ptr->label_scoreToAchieveValue->palette();
        palette.setColor(ui_ptr->label_scoreToAchieveValue->foregroundRole(), Qt::black);
        ui_ptr->label_scoreToAchieveValue->setPalette(palette);
    }
}

void game3players::on_pushButton_take10_clicked()
{
    QPalette palette = ui_ptr->label_scoreToAchieveValue->palette();
    palette.setColor(ui_ptr->label_scoreToAchieveValue->foregroundRole(), Qt::red);
    ui_ptr->label_scoreToAchieveValue->setPalette(palette);
    size_t value;
    value = static_cast<size_t>(ui_ptr->label_scoreToAchieveValue->text().toInt());
    if(value==minScoreToAchieve){
        QPalette palette = ui_ptr->label_scoreToAchieveValue->palette();
        palette.setColor(ui_ptr->label_scoreToAchieveValue->foregroundRole(), Qt::red);
        ui_ptr->label_scoreToAchieveValue->setPalette(palette);
    }else if(value<minScoreToAchieve){
        throw std::out_of_range("value is smaller than minScore");
    }else{
        value-=10;
        ui->label_scoreToAchieveValue->setText(QString::number(value));
        QPalette palette = ui_ptr->label_scoreToAchieveValue->palette();
        palette.setColor(ui_ptr->label_scoreToAchieveValue->foregroundRole(), Qt::black);
        ui_ptr->label_scoreToAchieveValue->setPalette(palette);
    }
}

void showMusikCards(){
    showPlayersCard(players[currentPlayer-1]);
    ui_ptr->groupBox_musikCard1->hide();
    ui_ptr->groupBox_musikCard2->hide();
    ui_ptr->groupBox_musikCard3->hide();
    ui_ptr->pushButton_playerCard1->hide();
    ui_ptr->pushButton_playerCard2->hide();
    ui_ptr->pushButton_playerCard3->hide();
    ui_ptr->pushButton_playerCard4->hide();
    ui_ptr->pushButton_playerCard5->hide();
    ui_ptr->pushButton_playerCard6->hide();
    ui_ptr->pushButton_playerCard7->hide();
    ui_ptr->pushButton_playerCard8->hide();
    for(size_t i=0;i<musik.size();i++){
        if(i==0){
            setIcon(musik[i], ui_ptr->label_musikCard1Suit);
            setValues(musik[i], ui_ptr->label_musikCard1value_1, ui_ptr->label_musikCard1value_2);
            ui_ptr->groupBox_musikCard1->show();
            ui_ptr->pushButton_musikCard1->show();
            continue;
        }
        if(i==1){
            setIcon(musik[i], ui_ptr->label_musikCard2Suit);
            setValues(musik[i], ui_ptr->label_musikCard2value_1, ui_ptr->label_musikCard2value_2);
            ui_ptr->groupBox_musikCard2->show();
            ui_ptr->pushButton_musikCard2->show();
            continue;
        }
        if(i==2){
            setIcon(musik[i], ui_ptr->label_musikCard3Suit);
            setValues(musik[i], ui_ptr->label_musikCard3value_1, ui_ptr->label_musikCard3value_2);
            ui_ptr->groupBox_musikCard3->show();
            ui_ptr->pushButton_musikCard3->show();
            continue;
        }
    }
}


void game3players::on_pushButton_test_clicked()
{
    ui_ptr->pushButton_test->hide();
    players[0].name = ui->groupBox_player1->title();
    players[1].name = ui->groupBox_player2->title();
    players[2].name = ui->groupBox_player3->title();
    ui->textEdit_player1->hide();
    ui->pushButton_player1->hide();
    ui->textEdit_player2->hide();
    ui->pushButton_player2->hide();
    ui->textEdit_player3->hide();
    ui->pushButton_player3->hide();
    gameFunction();
    ui_ptr->groupBox_scoreToAchieveControl->show();
    incrimentCurrentPlayer(currentPlayer);
    players[currentPlayer-1].scoreToAchieve=100;
    currentPlayerForContract=currentPlayer;
    incrimentCurrentPlayer(currentPlayerForContract);
    showPlayersCard(players[currentPlayerForContract-1]);
    ui_ptr->pushButton_playerCard1->hide();
    ui_ptr->pushButton_playerCard2->hide();
    ui_ptr->pushButton_playerCard3->hide();
    ui_ptr->pushButton_playerCard4->hide();
    ui_ptr->pushButton_playerCard5->hide();
    ui_ptr->pushButton_playerCard6->hide();
    ui_ptr->pushButton_playerCard7->hide();
    ui_ptr->pushButton_playerCard8->hide();
    ui_ptr->label_scoreToAchieveValue->setText(QString::number(minScoreToAchieve));
    maxScoreToAchieve = 120+returnScoreBonus(players[currentPlayerForContract-1]);
    ui_ptr->label_scoreToAchieveMaxValue->setText(QString::number(maxScoreToAchieve));
    //if(flag_p1name&&flag_p2name&&flag_p3name&&flag_gameName){
    //
    //}else{
    //    qDebug()<<"fck";
    //}
}

void musikCardBehaviour(QGroupBox* gb_ptr){
    //sleep(5);
    if(currentPlayerForMusik==4){
        currentPlayerForMusik=currentPlayer;
    }
    gb_ptr->hide();
    if(flag_cardForFirstPlayer){
        flag_cardForFirstPlayer=false;
        incrimentCurrentPlayer(currentPlayerForMusik);
        players[currentPlayerForMusik-1].playerCards.push_back(musik[gb_ptr->title().toInt()-1]);
        choosenMusikCards.push_back(gb_ptr->title().toInt()-1);
        return;
    }
    if(flag_cardForSecondPlayer){
        ui_ptr->groupBox_musikCard1->hide();
        ui_ptr->groupBox_musikCard2->hide();
        ui_ptr->groupBox_musikCard3->hide();
        flag_cardForSecondPlayer=false;
        incrimentCurrentPlayer(currentPlayerForMusik);
        players[currentPlayerForMusik-1].playerCards.push_back(musik[gb_ptr->title().toInt()-1]);
        choosenMusikCards.push_back(gb_ptr->title().toInt()-1);
        bool flag_0=true;
        bool flag_1=true;
        bool flag_2=true;
        for(auto& i:choosenMusikCards){
            if(i==0){
                flag_0=false;
                continue;
            }
            if(i==1){
                flag_1=false;
                continue;
            }
            if(i==2){
                flag_2=false;
                continue;
            }
        }
        if(flag_0){
            players[currentPlayer-1].playerCards.push_back(musik[0]);
        }
        if(flag_1){
            players[currentPlayer-1].playerCards.push_back(musik[1]);
        }
        if(flag_2){
            players[currentPlayer-1].playerCards.push_back(musik[2]);
        }
        ui_ptr->pushButton_playerCard1->show();
        ui_ptr->pushButton_playerCard2->show();
        ui_ptr->pushButton_playerCard3->show();
        ui_ptr->pushButton_playerCard4->show();
        ui_ptr->pushButton_playerCard5->show();
        ui_ptr->pushButton_playerCard6->show();
        ui_ptr->pushButton_playerCard7->show();
        ui_ptr->pushButton_playerCard8->show();
        if(!(flag_0||flag_1||flag_2))throw  std::out_of_range("wrong musik card return");
        showPlayersCard(players[currentPlayer-1]);
        return;
    }
}

void game3players::on_pushButton_musikCard1_clicked()
{
    qDebug()<<"gbm 1";
    musikCardBehaviour(ui_ptr->groupBox_musikCard1);
}

void game3players::on_pushButton_musikCard2_clicked()
{
    qDebug()<<"gbm 2";
    musikCardBehaviour(ui_ptr->groupBox_musikCard2);
}

void game3players::on_pushButton_musikCard3_clicked()
{
    qDebug()<<"gbm 3";
    musikCardBehaviour(ui_ptr->groupBox_musikCard3);
}

size_t returnScoreBonus(Player&player){
    size_t sum=0;
    for(const auto& card1:player.playerCards){
        if(card1.value==cardValue::Queen){
            for(const auto& card2:player.playerCards){
                if(card2.value==cardValue::King&&card2.suit==card1.suit){
                    switch (card2.suit) {
                        case cardSuit::Clovers:
                            sum+=60;
                            break;
                        case cardSuit::Hearts:
                            sum+=100;
                            break;
                        case cardSuit::Pikes:
                            sum+=40;
                            break;
                        case cardSuit::Tiles:
                            sum+=80;
                            break;
                        default:
                            throw std::out_of_range("returnScoreBonus cardSuit");
                            break;
                    }
                }
            }
        }
    }
    return sum;
}

void playerCardBehaviour(QGroupBox* gb_ptr){
    //sleep(5);
    size_t pos = static_cast<size_t>(gb_ptr->title().toInt()-1);
    bool flag_rightCard=true;
    if(flag_firstMove){
        currentPlayerForPlayerCards=currentPlayer;
        startSuit = players[currentPlayerForPlayerCards-1].playerCards[pos].suit;
        playerIterationCount=0;
        flag_firstMove=false;
    }else{
        if(haveSuit(players[currentPlayerForPlayerCards-1].playerCards)){
            if(players[currentPlayerForPlayerCards-1].playerCards[pos].suit==startSuit){
                flag_rightCard=true;
            }else{
                flag_rightCard=false;
                return;
            }
        }else{
            flag_rightCard=true;
        }
    }
    if(flag_rightCard){
        gb_ptr->hide();
        playerIterationCount++;
        cardsOnTheTable.push_back(*new Card(players[currentPlayerForPlayerCards-1].playerCards[pos], static_cast<int>(currentPlayerForPlayerCards)));
        showChosenCard(players[currentPlayerForPlayerCards-1].playerCards[pos]);
        players[currentPlayerForPlayerCards-1].playerCards.erase(players[currentPlayerForPlayerCards-1].playerCards.begin()+pos);
        players[currentPlayerForPlayerCards-1].playerCards.shrink_to_fit();
        incrimentCurrentPlayer(currentPlayerForPlayerCards);
        if(playerIterationCount==3){
            size_t winner=endRound();
            currentPlayer=winner;
            flag_firstMove=true;
            //gameScore.push_back(*(new GameIterationScore(1,0,0)));

            ui_ptr->groupBox_musikCard1->hide();
            ui_ptr->groupBox_musikCard2->hide();
            ui_ptr->groupBox_musikCard3->hide();
            if(players[winner-1].playerCards.size()==0){
                countPoints();

                QHBoxLayout *HLay= new QHBoxLayout();
                QLabel *player1 = new QLabel("");
                player1->setText(ui_ptr->label_player1score->text());
                player1->setAlignment(Qt::AlignCenter);
                player1->setFixedSize(125,70);
                QLabel *player2 = new QLabel("");
                player2->setAlignment(Qt::AlignCenter);
                player2->setText(ui_ptr->label_player2score->text());
                player2->setFixedSize(125,70);
                QLabel *player3 = new QLabel("");
                player3->setAlignment(Qt::AlignCenter);
                player3->setText(ui_ptr->label_player3score->text());
                player3->setFixedSize(125,70);

                HLay->addWidget(player1);
                HLay->addWidget(player2);
                HLay->addWidget(player3);

                QWidget * Widget = new QWidget();
                Widget->setMinimumSize(390,70);
                Widget->setLayout(HLay);

                QListWidgetItem *ListItem=new QListWidgetItem();
                ListItem->setSizeHint(QSize(390,70));
                ui_ptr->listWidget->addItem(ListItem);
                ui_ptr->listWidget->setItemWidget(ListItem, Widget);

                ui_ptr->groupBox_card1->hide();
                ui_ptr->groupBox_card2->hide();
                ui_ptr->groupBox_card3->hide();
                ui_ptr->groupBox_card4->hide();
                ui_ptr->groupBox_card5->hide();
                ui_ptr->groupBox_card6->hide();
                ui_ptr->groupBox_card7->hide();
                ui_ptr->groupBox_card8->hide();
                QSqlQuery query("INSERT INTO gameshistory(ID, player1score, player2score, player3score) VALUES (?,?,?,?)");
                query.addBindValue(ID);
                query.addBindValue(ui_ptr->label_player1score->text());
                qDebug()<<"1:"<<ui_ptr->label_player1score->text().toInt();
                query.addBindValue(ui_ptr->label_player2score->text());
                qDebug()<<"2:"<<ui_ptr->label_player1score->text().toInt();
                query.addBindValue(ui_ptr->label_player3score->text());
                qDebug()<<"3:"<<ui_ptr->label_player1score->text().toInt();
                ui_ptr->label_Info->show();
                if(ui_ptr->label_player1score->text().toInt()>=winScore){
                    qDebug()<<"Player 1 won";
                    ui_ptr->label_Info->setText("Player 1 won");
                    return;
                }
                if(ui_ptr->label_player2score->text().toInt()>=winScore){
                    qDebug()<<"Player 2 won";
                    ui_ptr->label_Info->setText("Player 2 won");
                    return;
                }
                if(ui_ptr->label_player3score->text().toInt()>=winScore){
                    qDebug()<<"Player 3 won";
                    ui_ptr->label_Info->setText("Player 3 won");
                    return;
                }
                if(query.exec()){
                    qDebug()<<"DB 2 ok";
                }else{
                    qDebug()<<"DB 2 not ok "+query.lastError().text();
                }
                ui_ptr->label_Info->hide();
                cardsOnTheTable.clear();
                flag_cardForFirstPlayer = true;
                flag_cardForSecondPlayer = true;
                currentPlayerForMusik=4;
                //!
                choosenMusikCards.clear();
                for(auto& p:players){
                   p.scoreToAchieve=0;
                }
                generateGameDecks();
                ui_ptr->groupBox_scoreToAchieveControl->show();
                incrimentCurrentPlayer(currentPlayer);
                for(auto& p: players){
                    p.scoreToAchieve=0;
                }
                players[currentPlayer-1].scoreToAchieve=100;
                minScoreToAchieve=100;
                ui_ptr->label_player1ScoreToAchieve->setText("0");
                ui_ptr->label_player2ScoreToAchieve->setText("0");
                ui_ptr->label_player3ScoreToAchieve->setText("0");
                currentPlayerForContract=currentPlayer;
                incrimentCurrentPlayer(currentPlayerForContract);
                showPlayersCard(players[currentPlayerForContract-1]);
                ui_ptr->pushButton_playerCard1->hide();
                ui_ptr->pushButton_playerCard2->hide();
                ui_ptr->pushButton_playerCard3->hide();
                ui_ptr->pushButton_playerCard4->hide();
                ui_ptr->pushButton_playerCard5->hide();
                ui_ptr->pushButton_playerCard6->hide();
                ui_ptr->pushButton_playerCard7->hide();
                ui_ptr->pushButton_playerCard8->hide();
                ui_ptr->label_scoreToAchieveValue->setText(QString::number(minScoreToAchieve));
                maxScoreToAchieve = 120+returnScoreBonus(players[currentPlayerForContract-1]);
                ui_ptr->label_scoreToAchieveMaxValue->setText(QString::number(maxScoreToAchieve));
                //choosenMusikCards.clear();
                //generateGameDecks();
                //showMusikCards();
                return;
            }else{
                showPlayersCard(players[winner-1]);
            }
        }else{
            showPlayersCard(players[currentPlayerForPlayerCards-1]);
        }
    }


}

bool haveSuit(std::vector<Card>& array){
    for(const auto& card:array){
        if(card.suit==startSuit)return true;
    }
    return false;
}

void countPoints(){
    int value1 = ui_ptr->label_player1score->text().toInt();
    int value2 = ui_ptr->label_player2score->text().toInt();
    int value3 = ui_ptr->label_player3score->text().toInt();
    size_t sum1=0;
    size_t sum2=0;
    size_t sum3=0;
    for(auto card:cardsOnTheTable){
        if(card.owner-1==0){
            sum1+=cardValueToPoints(card.value);
            continue;
        }
        if(card.owner-1==1){
            sum2+=cardValueToPoints(card.value);
            continue;
        }
        if(card.owner-1==2){
            sum3+=cardValueToPoints(card.value);
            continue;
        }
    }
    value1+=absValue(sum1);
    value2+=absValue(sum2);
    value3+=absValue(sum3);
    if(players[0].scoreToAchieve>sum1){
        value1=-players[0].scoreToAchieve;
    }
    if(players[1].scoreToAchieve>sum2){
        value2=-players[1].scoreToAchieve;
    }
    if(players[2].scoreToAchieve>sum3){
        value3=-players[2].scoreToAchieve;
    }
    ui_ptr->label_player1score->setText(QString::number(value1));
    ui_ptr->label_player2score->setText(QString::number(value2));
    ui_ptr->label_player3score->setText(QString::number(value3));
}

//returns winner pos
size_t endRound(){
    Card* winnerCard_ptr;
    bool flag_firstCard=true;
    for(auto card:cardsOnTheTable){
        if(flag_firstCard){
            winnerCard_ptr=&card;
            flag_firstCard=false;
        }else{
            if(card.suit==startSuit&&card.value>winnerCard_ptr->value){
                winnerCard_ptr=&card;
            }
        }
    }
    return winnerCard_ptr->owner;
}

void showChosenCard(Card& card){
    switch (currentPlayerForPlayerCards) {
        case 1:
        setIcon(card, ui_ptr->label_musikCard1Suit);
        setValues(card, ui_ptr->label_musikCard1value_1, ui_ptr->label_musikCard1value_2);
        ui_ptr->groupBox_musikCard1->show();
        ui_ptr->pushButton_musikCard1->hide();
        break;
    case 2:
        setIcon(card, ui_ptr->label_musikCard2Suit);
        setValues(card, ui_ptr->label_musikCard2value_1, ui_ptr->label_musikCard2value_2);
        ui_ptr->groupBox_musikCard2->show();
        ui_ptr->pushButton_musikCard2->hide();
        break;
    case 3:
        setIcon(card, ui_ptr->label_musikCard3Suit);
        setValues(card, ui_ptr->label_musikCard3value_1, ui_ptr->label_musikCard3value_2);
        ui_ptr->groupBox_musikCard3->show();
        ui_ptr->pushButton_musikCard3->hide();
        break;
    default:
        throw std::out_of_range("showChosenCard wrong currentPlayerForCards");
        break;
    }
}

void game3players::on_pushButton_playerCard1_clicked()
{
    playerCardBehaviour(ui_ptr->groupBox_card1);
}

void game3players::on_pushButton_playerCard2_clicked()
{
    playerCardBehaviour(ui_ptr->groupBox_card2);
}

void game3players::on_pushButton_playerCard3_clicked()
{
    playerCardBehaviour(ui_ptr->groupBox_card3);
}

void game3players::on_pushButton_playerCard4_clicked()
{
    playerCardBehaviour(ui_ptr->groupBox_card4);
}

void game3players::on_pushButton_playerCard5_clicked()
{
    playerCardBehaviour(ui_ptr->groupBox_card5);
}

void game3players::on_pushButton_playerCard6_clicked()
{
    playerCardBehaviour(ui_ptr->groupBox_card6);
}

void game3players::on_pushButton_playerCard7_clicked()
{
    playerCardBehaviour(ui_ptr->groupBox_card7);
}

void game3players::on_pushButton_playerCard8_clicked()
{
    playerCardBehaviour(ui_ptr->groupBox_card8);
}

void game3players::on_pushButton_pick_clicked()
{
    //sleep(5);
    scoreFlagChange();
    size_t value = static_cast<size_t>(ui_ptr->label_scoreToAchieveValue->text().toInt());
    if(value>minScoreToAchieve&&value<=maxScoreToAchieve){
        for(auto& p:players){
            if(p.scoreToAchieve!=INT_MAX)p.scoreToAchieve=0;
        }
        while(playersWithMaxScore[currentPlayerForContract-1]==true){
            incrimentCurrentPlayer(currentPlayerForContract);
        }
        players[currentPlayerForContract-1].scoreToAchieve=value;
        playersWithMaxScoreA=0;
        minScoreToAchieve=value;
        while(playersWithMaxScore[currentPlayerForContract-1]!=false){
            playersWithMaxScoreA++;
            incrimentCurrentPlayer(currentPlayerForContract);
            if(playersWithMaxScoreA==2){
                break;
            }
        }
        if(playersWithMaxScoreA<2){
            playersWithMaxScoreA=0;
            while(120+returnScoreBonus(players[currentPlayerForContract-1])<=minScoreToAchieve || players[currentPlayerForContract-1].scoreToAchieve==INT_MAX){
                playersWithMaxScoreA++;
                incrimentCurrentPlayer(currentPlayerForContract);
                if(playersWithMaxScoreA==2){
                    break;
                }
            }
        }
        playerWithContractID=currentPlayerForContract-1;
    }else{
        incrimentCurrentPlayer(currentPlayerForContract);
    }

    if(playersWithMaxScoreA>=2){
        showPlayersCard(players[playerWithContractID]);
        showMusikCards();
        ui_ptr->groupBox_scoreToAchieveControl->hide();
        for(auto& p:players){
            if(p.scoreToAchieve==INT_MAX)p.scoreToAchieve=0;
        }
        ui_ptr->label_player1ScoreToAchieve->setText(QString::number(players[0].scoreToAchieve));
        ui_ptr->label_player2ScoreToAchieve->setText(QString::number(players[1].scoreToAchieve));
        ui_ptr->label_player3ScoreToAchieve->setText(QString::number(players[2].scoreToAchieve));
        playersWithMaxScore.clear();
        playersWithMaxScore.push_back(false);
        playersWithMaxScore.push_back(false);
        playersWithMaxScore.push_back(false);
        currentPlayer = playerWithContractID+1;
        return;
    }
    ui_ptr->label_scoreToAchieveValue->setText(QString::number(minScoreToAchieve));
    maxScoreToAchieve = 120+returnScoreBonus(players[currentPlayerForContract-1]);
    ui_ptr->label_scoreToAchieveMaxValue->setText(QString::number(maxScoreToAchieve));
}

void game3players::on_pushButton_pass_clicked()
{
    //sleep(5);
    scoreFlagChange();
    if(players[currentPlayerForContract-1].scoreToAchieve==0)players[currentPlayerForContract-1].scoreToAchieve = INT_MAX;
    playersWithMaxScore[currentPlayerForContract-1]=1;
    playersWithMaxScoreA=2;
    while(playersWithMaxScore[currentPlayerForContract-1]!=false){
        playersWithMaxScoreA++;
        incrimentCurrentPlayer(currentPlayerForContract);
        if(playersWithMaxScoreA==2){
            break;
        }
    }
    if(playersWithMaxScoreA<2){
        playersWithMaxScoreA=0;
        while(120+returnScoreBonus(players[currentPlayerForContract-1])<=minScoreToAchieve || players[currentPlayerForContract-1].scoreToAchieve==INT_MAX){
            playersWithMaxScoreA++;
            incrimentCurrentPlayer(currentPlayerForContract);
            if(playersWithMaxScoreA==2){
                break;
            }
        }
    }
    if(playersWithMaxScoreA>=2){
        showPlayersCard(players[playerWithContractID]);
        showMusikCards();
        ui_ptr->groupBox_scoreToAchieveControl->hide();
        for(auto& p:players){
            if(p.scoreToAchieve==INT_MAX)p.scoreToAchieve=0;
        }
        ui_ptr->label_player1ScoreToAchieve->setText(QString::number(players[0].scoreToAchieve));
        ui_ptr->label_player2ScoreToAchieve->setText(QString::number(players[1].scoreToAchieve));
        ui_ptr->label_player3ScoreToAchieve->setText(QString::number(players[2].scoreToAchieve));
        playersWithMaxScore.clear();
        playersWithMaxScore.push_back(false);
        playersWithMaxScore.push_back(false);
        playersWithMaxScore.push_back(false);
        currentPlayer = playerWithContractID+1;
        return;
    }
    ui_ptr->label_scoreToAchieveValue->setText(QString::number(minScoreToAchieve));
    maxScoreToAchieve = 120+returnScoreBonus(players[currentPlayerForContract-1]);
    ui_ptr->label_scoreToAchieveMaxValue->setText(QString::number(maxScoreToAchieve));
}

void scoreFlagChange(){
    if(currentPlayerForContract==1){
        flag_fpContract=true;
        incrimentCurrentPlayer(currentPlayerForContract);
    }else if(currentPlayerForContract==2){
        flag_spContract=true;
        incrimentCurrentPlayer(currentPlayerForContract);
    }else{
        flag_tpContract=true;
        incrimentCurrentPlayer(currentPlayerForContract);
    }
}

size_t absValue(size_t& value){
    if(value%10<6){
        value=value/10;
    }else{
        value=(value/10+1)*10;
    }
    return value;
}


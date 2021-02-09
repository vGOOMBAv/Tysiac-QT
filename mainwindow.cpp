#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "carddeck.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug()<<(cardValue::Nine<cardValue::Ten);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    g3p_ptr = new game3players(this);
    g3p_ptr->show();
}

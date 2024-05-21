#include "promotionwindow.h"
#include "ui_promotionwindow.h"

PromotionWindow::PromotionWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PromotionWindow)
{
    ui->setupUi(this);
    prom = 50000;
}

PromotionWindow::~PromotionWindow()
{
    delete ui;
}

void PromotionWindow::on_Queen_clicked()
{
    prom = 50000;
    close();
}

void PromotionWindow::on_Rook_clicked()
{
    prom = 40000;
    close();
}

void PromotionWindow::on_Bishop_clicked()
{
    prom = 30000;
    close();
}

void PromotionWindow::on_Knight_clicked()
{
    prom = 20000;
    close();
}


#include "addpiecewindow.h"
#include "ui_editwindow.h"
#include <iostream>

AddPieceWindow::AddPieceWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditWindow)
{
    ui->setupUi(this);
    this->setFixedSize(QSize(100, 300));
    
    piece = 0;
    kw.load(":/graphics/kingW.png");
    qw.load(":/graphics/queenW.png");
    rw.load(":/graphics/rookW.png");
    bw.load(":/graphics/bishopW.png");
    nw.load(":/graphics/knightW.png");
    pw.load(":/graphics/pawnW.png");

    kb.load(":/graphics/kingB.png");
    qb.load(":/graphics/queenB.png");
    rb.load(":/graphics/rookB.png");
    bb.load(":/graphics/bishopB.png");
    nb.load(":/graphics/knightB.png");
    pb.load(":/graphics/pawnB.png");
    
    pieces[0] = kw;
    pieces[1] = qw;
    pieces[2] = rw;
    pieces[3] = bw;
    pieces[4] = nw;
    pieces[5] = pw;

    pieces[6] = kb;
    pieces[7] = qb;
    pieces[8] = rb;
    pieces[9] = bb;
    pieces[10] = nb;
    pieces[11] = pb;
    
    QPixmap *pixmap = new QPixmap(100, 300);
    pixmap->fill();
    QPainter *painter = new QPainter(pixmap);
    
    for(int i = 0; i < 6; i++)
        painter->drawImage(0, i * 50, pieces[i]);
    for(int i = 0; i < 6; i++)
        painter->drawImage(50, i * 50, pieces[i + 6]);
    ui->label->setPixmap(*pixmap);
}

AddPieceWindow::~AddPieceWindow()
{
    delete ui;
}

void AddPieceWindow::mousePressEvent(QMouseEvent *e)
{
    int x;
    int y;
    x = e->pos().x();
    y = e->pos().y();
    piece = x < 50 ? 6 - (y / 50) : -6 + (y / 50);
    std::cout << piece << std::endl;
}

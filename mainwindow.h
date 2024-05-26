#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <chrono>
#include <ctype.h>
#include "QMainWindow"
#include "QMouseEvent"
#include "QPainter"
#include "QFileDialog"
#include "QFile"
#include "QLabel"
#include "editwindow.h"
#include "promotionwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QImage board;
    QImage processBoard;
    QImage kw;
    QImage qw;
    QImage rw;
    QImage bw;
    QImage nw;
    QImage pw;
    QImage kb;
    QImage qb;
    QImage rb;
    QImage bb;
    QImage nb;
    QImage pb;

    int row;
    int col;
    int aom;
    int problem;

    bool turn;
    bool side;
    bool select;
    bool edit;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void mousePressEvent(QMouseEvent *event);
    void init_board();
    void set_board();
    void clear_board();
    void draw_board();
    bool is_turn(const int r0, const int c0);
    QImage* which_piece(int a);



private slots:
    void on_actionopen_triggered();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_spinBox_valueChanged(int arg1);
    void on_checkBox_stateChanged(int arg1);
    void on_comboBox_currentIndexChanged(int index);

    void on_comboBox_2_activated(int index);
    
    void on_Clear_clicked();
    
private:
    Ui::MainWindow *ui;
    Editwindow *ew;
    QLabel *stat;
};
#endif // MAINWINDOW_H

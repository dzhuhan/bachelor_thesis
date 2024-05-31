#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <chrono>
#include <ctype.h>
#include "QTextCharFormat"
#include "QProgressBar"
#include "QMainWindow"
#include "QMouseEvent"
#include "QPainter"
#include "QFileDialog"
#include "QFile"
#include "QLabel"
#include "global_data.h"
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
    int problem;

    bool turn;
    bool side;
    bool select;
    bool edit;
    
    int jump;
    std::vector<state> st_arr;
    QLabel *stat;
    QProgressBar *prog;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void mousePressEvent(QMouseEvent *event);
    void init_board();
    void set_board();
    void clear_board();
    void draw_board();
    void highlight_move();
    bool is_turn(const int r0, const int c0);
    QImage* which_piece(int a);



private slots:
    void on_actionopen_triggered();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_spinBox_valueChanged(int arg1);
    void on_comboBox_currentIndexChanged(int index);
    void on_comboBox_2_activated(int index);
    void on_edit_clicked(bool checked);
    void on_Clear_clicked();
    void on_previous_clicked();
    void on_next_clicked();
    void on_first_clicked();
    void on_last_clicked();
    
private:
    Ui::MainWindow *ui;
    Editwindow *ew;
    QTextCharFormat *jump_fmt;
    QTextCursor *jump_cursor;
};
#endif // MAINWINDOW_H

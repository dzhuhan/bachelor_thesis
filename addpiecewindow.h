#ifndef ADDPIECEWINDOW_H
#define ADDPIECEWINDOW_H

#include <QDialog>
#include <QPainter>
#include <QMouseEvent>

namespace Ui {
class EditWindow;
}

class AddPieceWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddPieceWindow(QWidget *parent = nullptr);
    ~AddPieceWindow();
    void mousePressEvent(QMouseEvent *event);
    
    int piece;
    QImage pieces[12];
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

private:
    Ui::EditWindow *ui;
};

#endif // ADDPIECEWINDOW_H

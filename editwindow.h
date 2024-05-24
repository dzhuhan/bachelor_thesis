#ifndef EDITWINDOW_H
#define EDITWINDOW_H

#include <QDialog>
#include <QPainter>
#include <QMouseEvent>

namespace Ui {
class EditWindow;
}

class Editwindow : public QDialog
{
    Q_OBJECT

public:
    explicit Editwindow(QWidget *parent = nullptr);
    ~Editwindow();
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

#endif // EDITWINDOW_H

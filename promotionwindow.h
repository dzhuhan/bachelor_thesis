#ifndef PROMOTIONWINDOW_H
#define PROMOTIONWINDOW_H

#include <QDialog>

namespace Ui {
class PromotionWindow;
}

class PromotionWindow : public QDialog
{
    Q_OBJECT

public:
    int prom;
    explicit PromotionWindow(QWidget *parent = nullptr);
    ~PromotionWindow();
    
private slots:
    void on_Queen_clicked();
    void on_Rook_clicked();
    void on_Bishop_clicked();
    void on_Knight_clicked();
    
private:
    Ui::PromotionWindow *ui;
};

#endif // PROMOTIONWINDOW_H

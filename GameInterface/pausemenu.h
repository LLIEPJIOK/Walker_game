
#ifndef PAUSEMENU_H
#define PAUSEMENU_H

#include <QPushButton>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QPaintEvent>
#include <QPainter>

class PauseMenu : public QWidget
{
    Q_OBJECT
public:

    explicit PauseMenu(QWidget* parent = nullptr);
private:
    QPushButton *continue_button;
    QPushButton *main_menu;
    QVBoxLayout *vblay;
signals:
    void continue_button_clicked();
    void main_menu_clicked();
protected:
    void paintEvent(QPaintEvent* event);
};

#endif // PAUSEMENU_H

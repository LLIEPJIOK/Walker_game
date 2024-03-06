#ifndef PAUSEMENU_H
#define PAUSEMENU_H

#include "Menu/load.h"


class PauseMenu : public QMainWindow
{
    Q_OBJECT

public:
    explicit PauseMenu(QMainWindow* parent = nullptr);

private:
    QPushButton *continue_button;
    QPushButton* save_menu;
    QPushButton *main_menu;
    QVBoxLayout *vblay;
    QWidget* pause_menu;

    Load* load;

signals:
    void continue_button_clicked_signal();
    void main_menu_clicked();
    void save_game_signal(QString file_name);

protected:
    void paintEvent(QPaintEvent* event);

private slots:
    void open_save();
    void show_pause();
    void save_game_slot(QString file_name);
    void continue_button_clicked_slot();
};

#endif // PAUSEMENU_H

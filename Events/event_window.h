
#ifndef EVENT_WINDOW_H
#define EVENT_WINDOW_H

#include <QPixmap>
#include <QWindow>
#include <QWidget>
#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QString>
#include "Engine/Player.h"
#include "Event.h"


class Event_window : public QWidget

{
    Q_OBJECT

public:
    Event_window(QWidget *parent, Player* target, Event* event);
    ~Event_window();

private:
    QVBoxLayout* main_layout;
    QLabel *img_label;
    QLabel *info;

    QLabel* requirement_info;
    QPushButton* challenge_your_luck;
    Player* target;
    std::string event_name;

    bool success;

protected:
    void paintEvent(QPaintEvent* event);
public slots:
    void challenge_button_was_clicked();
    void continue_playing();
signals:
    void send_event_choice(std::string _event_name, Player* _target, std::string attribute_choice); // наследие или труп, как вам угодно...
    void event_started();
    void event_ended(); // сигнал, который указывает на конец ивента
};

#endif // EVENT_WINDOW_H

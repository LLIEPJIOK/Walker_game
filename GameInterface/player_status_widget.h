
#ifndef PLAYER_STATUS_WIDGET_H
#define PLAYER_STATUS_WIDGET_H

#include "./Engine/Player.h"
#include "GameInterface/effect_item.h"
class Player_status_widget : public QWidget

{
    Q_OBJECT
    Player* assigned_player;

    QVBoxLayout* main_layout;

    //overview
    QHBoxLayout* overview_lo;
    QVBoxLayout* overview_labels;
    QVBoxLayout* overview_present;

    QPixmap* pl_img;

    QLabel* name_label;
    QLabel* ov_health_label;
    QLabel* ov_atk_label;

    QLabel* name_val;
    QProgressBar* health_bar;
    QLabel* ov_atk_value;

    //advanced chars
    QTabWidget* advanced;

    //effects
    QListWidget* effects;
protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);

public:
    Player_status_widget(QWidget *parent = nullptr);
    Player_status_widget(QWidget *parent, Player* pl);
    ~Player_status_widget();

    QListWidget* get_effects();
public slots:
    void update_all();
};


#endif // PLAYER_STATUS_WIDGET_H

#ifndef PLAYERSMODEL_H
#define PLAYERSMODEL_H

#include "Engine/Player.h"
#include "mapobject.h"

#include <QTimer>
#include <QPixmap>
class PlayersModel : public QObject, public MapObject
{
    Q_OBJECT
    enum animation_status
    {
        Standing = 0,
        Attack,
        Moving,
        StateEnd
    };

public:
    explicit PlayersModel(QObject* parent = nullptr, int width = 0, int height = 0, QString icon = "knight");
    Player* get_connected_player();
    void set_connected_player(Player* player);

    void set_left_direction();
    void set_right_direction();
    void move(QPointF dir);
    void stop();
    void attack();

private:
    Player* connected_player;
    qreal direction{1.};
    animation_status current_animation_status{animation_status::Standing};

    int current_movement_frame{0};
    QTimer timer;

    QVector<QPixmap> movement_frames;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
signals:
    void target_to_attack(PlayersModel* attacked_player);
protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private slots :
    void next_frame();
};

#endif // PLAYERSMODEL_H

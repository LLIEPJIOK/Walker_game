
#ifndef PLAYERSMODEL_H
#define PLAYERSMODEL_H
#include "Engine/Player.h"

#include <QTimer>
#include <QPixmap>
#include "gamemapobject.h"
class PlayersModel :  public GameMapObject
{
    Q_OBJECT
    enum class eAnimateState
    {
        Standing = 0
        ,Attack
        ,Moving
        ,StateEnd
    };
public:
    explicit PlayersModel(QObject* parent = nullptr, int xrpos = 0, int yrpos = 0, QBrush brush = QBrush(Qt::NoBrush), QString icon = "knight");
    Player* get_connected_player();
    void set_connected_plaeyr(Player* player);


    void set_left_direction();
    void set_right_direction();
    void move(QPointF dir);
    void stop();
    void attack();

private:
    Player* connected_player;
    qreal mDx{1.};
    eAnimateState mState{eAnimateState::Standing};

    int mCurrentFrame{0};
    QTimer  mTimer;

    QVector<QPixmap> mvPixmaps;


    void paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
signals:
    void target_to_attack();
protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private slots :
    void next_frame();
};

#endif // PLAYERSMODEL_H

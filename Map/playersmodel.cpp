
#include "playersmodel.h"
#include "QTransform"
PlayersModel::PlayersModel(QObject *parent, int width, int height, QString icon) :
    QObject(parent), GameMapObject(width, height)
{
    connected_player = nullptr;

    QPixmap pixmap_to_load;
    for (int i = 1; i < 6; i++)
    {
        pixmap_to_load.load(":/" + icon + "/Pictures/" + icon + '/' + icon + QString::number(i) + ".png");
        pixmap_to_load = pixmap_to_load.scaled(width, height);
        movement_frames.push_back(pixmap_to_load);
    }
    connect(&timer, &QTimer::timeout, this, &PlayersModel::next_frame);
    timer.start(200);
}

Player *PlayersModel::get_connected_player()
{
    return connected_player;
}

void PlayersModel::set_connected_player(Player *player)
{
    connected_player = player;
}

void PlayersModel::set_left_direction()
{
    if(direction < 0.)
    {
        QTransform transform(this->transform());

        qreal m11 = transform.m11();
        qreal m12 = transform.m12();
        qreal m13 = transform.m13();
        qreal m21 = transform.m21();
        qreal m22 = transform.m22();
        qreal m23 = transform.m23();
        qreal m31 = transform.m31();
        qreal m32 = transform.m32();
        qreal m33 = transform.m33();

        qreal scale = m11;

        m11 = -m11;


        if(m31 > 0)
            m31 = 0;
        else
            m31 = (boundingRect().width() * scale);


        transform.setMatrix(m11, m12, m13, m21, m22, m23, m31, m32, m33);


        setTransform(transform);
        direction = 1.;
    }
}

void PlayersModel::set_right_direction()
{
    if(direction > 0.)
    {
        QTransform transform(this->transform());

        qreal m11 = transform.m11();
        qreal m12 = transform.m12();
        qreal m13 = transform.m13();
        qreal m21 = transform.m21();
        qreal m22 = transform.m22();
        qreal m23 = transform.m23();
        qreal m31 = transform.m31();
        qreal m32 = transform.m32();
        qreal m33 = transform.m33();


        qreal scale = m11;


        m11 = -m11;


        if(m31 > 0)
            m31 = 0;
        else
            m31 = (boundingRect().width() * scale);


        transform.setMatrix(m11, m12, m13, m21, m22, m23, m31, m32, m33);


        setTransform(transform);
        direction = -1.;
    }
}

void PlayersModel::move(QPointF dir)
{
    setPos(pos().x() + dir.x(), pos().y() + dir.y());
}

void PlayersModel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawPixmap(0, 0, movement_frames[current_movement_frame]);
}

void PlayersModel::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    is_hovered = true;

    update();
}

void PlayersModel::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    is_hovered = false;

    update();
}

void PlayersModel::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
        emit target_to_attack();
}

void PlayersModel::next_frame()
{
    current_movement_frame++;
    if(current_movement_frame >= movement_frames.size())
        current_movement_frame = 0;
    update();
}

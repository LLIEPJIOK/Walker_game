
#include "playersmodel.h"
#include "QTransform"
PlayersModel::PlayersModel(QObject *parent, int xrpos, int yrpos, QBrush brush, QString icon) :
    GameMapObject(parent, xrpos, yrpos, brush)
{
    connected_player = nullptr;

    QPixmap l;
    for (int i = 1; i < 6; i++)
    {
        l.load("../Game/Resources/Pictures/" + icon + "/" + icon + QString::number(i) + ".png");
        l = l.scaled(width, height);
        mvPixmaps.push_back(l);
    }
    connect(&mTimer, &QTimer::timeout, this, &PlayersModel::next_frame);
    mTimer.start(200);
}

Player *PlayersModel::get_connected_player()
{
    return connected_player;
}

void PlayersModel::set_connected_plaeyr(Player *player)
{
    connected_player = player;
}

void PlayersModel::set_left_direction()
{
    if(mDx < 0.)
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
        mDx = 1.;
    }
}

void PlayersModel::set_right_direction()
{
    if(mDx > 0.)
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
        mDx = -1.;
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
    painter->drawPixmap(0, 0, mvPixmaps[mCurrentFrame]);
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
    mCurrentFrame++;
    if(mCurrentFrame >= mvPixmaps.size())
        mCurrentFrame = 0;
    update();
}

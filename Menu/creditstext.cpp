#include "creditstext.h"

CreditsText::CreditsText() :
    QGraphicsItem()
{
    size = QApplication::screens().at(0)->size();
    reset();
}

void CreditsText::reset()
{
    setY (size.height() + 100);
}

QRectF CreditsText::boundingRect() const
{
    return QRectF(QPointF(0, 0), size + QSize(0, 1100));
}

void CreditsText::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setFont(QFont("Arial", 20));
    painter->setPen(Qt::white);

    painter->drawText(0, 0, size.width(), 30, Qt::AlignCenter, QObject::tr("Studio"));
    painter->drawText(0, 40, size.width(), 30, Qt::AlignCenter, "Brodilka Gaming");

    painter->drawText(0, 100, size.width(), 30, Qt::AlignCenter, QObject::tr("Director"));
    painter->drawText(0, 140, size.width(), 30, Qt::AlignCenter, QObject::tr("Nikita \"Luni\" Lukyanov"));

    painter->drawText(0, 200, size.width(), 30, Qt::AlignCenter, QObject::tr("Game Designer"));
    painter->drawText(0, 240, size.width(), 30, Qt::AlignCenter, QObject::tr("Anton \"Kapusha\" Sadovski")); // 300 -> 660

    painter->drawText(0, 300, size.width(), 30, Qt::AlignCenter, QObject::tr("Full stack developers"));
    painter->drawText(0, 340, size.width(), 30, Qt::AlignCenter, QObject::tr("Anton \"Kapusha\" Sadovski"));
    painter->drawText(0, 380, size.width(), 30, Qt::AlignCenter, QObject::tr("Nikita \"Luni\" Lukyanov"));
    painter->drawText(0, 420, size.width(), 30, Qt::AlignCenter, QObject::tr("Denis \"LLIEPJIOK\" Lebedzev"));

    painter->drawText(0, 480, size.width(), 30, Qt::AlignCenter,
                      QObject::tr("Developed for tech skillset showcase. Non-commercial project."));
    painter->drawText(0, 520, size.width(), 30, Qt::AlignCenter,
                      QObject::tr("No one was injured during the development process (except for our mental health)."));
    painter->drawText(0, 560, size.width(), 30, Qt::AlignCenter,
                      QObject::tr("All events are not real!"));

    painter->drawText(0, 600, size.width(), 30, Qt::AlignCenter,
                      QObject::tr("Thanks for playing!"));
}


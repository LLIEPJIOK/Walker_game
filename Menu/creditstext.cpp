#include "creditstext.h"

CreditsText::CreditsText(QObject *parent) : QObject(parent), QGraphicsItem()
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

    painter->drawText(0, 0, size.width(), 30, Qt::AlignCenter, "Студия");
    painter->drawText(0, 40, size.width(), 30, Qt::AlignCenter, "Brodilka Gaming");

    painter->drawText(0, 100, size.width(), 30, Qt::AlignCenter, "Руководитель проекта");
    painter->drawText(0, 140, size.width(), 30, Qt::AlignCenter, "Лукьянов Никита");

    painter->drawText(0, 200, size.width(), 30, Qt::AlignCenter, "Автор идеи");
    painter->drawText(0, 240, size.width(), 30, Qt::AlignCenter, "Садовский Антон");

    painter->drawText(0, 300, size.width(), 30, Qt::AlignCenter, "Разработчики интерфейса");
    painter->drawText(0, 340, size.width(), 30, Qt::AlignCenter, "Лебедев Денис");
    painter->drawText(0, 380, size.width(), 30, Qt::AlignCenter, "Лукьянов Никита");

    painter->drawText(0, 440, size.width(), 30, Qt::AlignCenter, "Графика");
    painter->drawText(0, 480, size.width(), 30, Qt::AlignCenter, "Горбач Владимир");

    painter->drawText(0, 540, size.width(), 30, Qt::AlignCenter, "Back-end разработчики");
    painter->drawText(0, 580, size.width(), 30, Qt::AlignCenter, "Лебедев Денис");
    painter->drawText(0, 620, size.width(), 30, Qt::AlignCenter, "Садовский Антон");
    painter->drawText(0, 660, size.width(), 30, Qt::AlignCenter, "Лукьянов Никита");

    painter->drawText(0, 760, size.width(), 30, Qt::AlignCenter,
                      "Весь медиаконтент используется не в коммерческих целях");
    painter->drawText(0, 800, size.width(), 30, Qt::AlignCenter,
                      "При разработке данного приложения никто не пострадал");
    painter->drawText(0, 840, size.width(), 30, Qt::AlignCenter,
                      "Основано не на реальных событиях");
    painter->drawText(0, 880, size.width(), 30, Qt::AlignCenter,
                      "Все персонажи выдуманы");

    painter->drawText(0, 980, size.width(), 30, Qt::AlignCenter,
                      "Спасибо, что вы с нами!");
}


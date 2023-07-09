
#include "infocell.h"
InfoCell::InfoCell(QWidget *parent, QString terrain, QString item, QString event)
    : QFrame{parent}
{
    terrain_type = new QLabel(this);
    item_name = new QLabel(this);
    item_name->setWordWrap(true);
    event_name = new QLabel(this);
    setFixedSize(175, 100);
    setFrameStyle(QFrame::WinPanel | QFrame::Plain);
    QPalette pal;
    pal.setBrush(backgroundRole(), QBrush(Qt::white));
    setPalette(pal);
    setAutoFillBackground(true);

    terrain_type->setText("Поверхность: " + terrain);
    terrain_type->resize(this->width(), 30);

    item_name-> setText("Предмет: " + item);
    item_name->resize(this->width(), 30);
    item_name->setAlignment(Qt::AlignLeft);

    event_name->setText("Событие: " + event);
    event_name->resize(this->width(), 30);

    QVBoxLayout *l = new QVBoxLayout(this);
    l->addWidget(terrain_type);
    l->addWidget(item_name);
    l->addWidget(event_name);
    l->setAlignment(Qt::AlignLeft);
    setLayout(l);
}

InfoCell::~InfoCell()
{
    delete terrain_type;
    delete item_name;
    delete event_name;
}

void InfoCell::set_terrain_type(QString str)
{
    terrain_type->setText("Поверхность: " + str);
}

void InfoCell::set_item_name(QString str)
{
    item_name->setText("Предмет: " + str);
}

void InfoCell::set_event_name(QString str)
{
    event_name->setText("Событие: " + str);
}


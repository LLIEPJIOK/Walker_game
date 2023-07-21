#include "loadslot.h"

// file name pattern: time (in reverse order) + date (in reverse order) + name

LoadSlot::LoadSlot(QString _file_name, QWidget *parent)
    : QWidget(parent)
{
    QPalette pal = palette();
    pal.setColor(QPalette::WindowText, Qt::white);
    setPalette(pal);

    file_name = _file_name;

    painter = new QPainter;

    label_name = new QLabel(file_name.mid(20), this);
    label_name->setFont(QFont("Arial", 12));

    auto time = file_name.mid(6, 2) + file_name.mid(2, 4) + file_name.mid(0, 2);
    auto date = file_name.mid(17, 2) + file_name.mid(13, 4) + file_name.mid(9, 4);

    label_date = new QLabel(date + "\t" + time, this);
    label_date->setFont(QFont("Arial", 12));

    is_hover = 0;
    is_pressed = 0;
}

void LoadSlot::unpress()
{
    is_pressed = 0;
    repaint();
}

void LoadSlot::delete_save()
{
    //QFile::remove(file_name);
    delete this;
}

QString LoadSlot::get_name()
{
    return label_name->text();
}

QString LoadSlot::get_file_name()
{
    return file_name;
}

void LoadSlot::change_name(QString name)
{
    label_name->setText(fontMetrics().elidedText(name, Qt::ElideRight, width() / 2. - 30));
    //перезапись файла
}

void LoadSlot::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    label_name->move(width() / 100., (height() - label_name->height()) / 2.);
    label_name->resize(width() / 2., 30);
    label_date->move(99 * width() / 100. - label_date->width(), (height() - label_name->height()) / 2.);
    painter->begin(this);
    painter->setRenderHint(QPainter::Antialiasing);
    if (is_pressed)
    {
        painter->setPen(QPen(QBrush(QColor(255, 178, 102)), 6));
        painter->setBrush(QColor(255, 178, 102, 20));
        painter->drawRect(0, 0, width(), height());
    }
    else if (is_hover)
        painter->setPen(QPen(QBrush(QColor(255, 178, 102)), 4));
    else
        painter->setPen(QPen(QBrush(QColor(255, 255, 255)), 2));
    painter->drawRect(0, 0, width(), height());
    painter->end();
}

void LoadSlot::enterEvent(QEnterEvent *event)
{
    Q_UNUSED(event);
    if (!isEnabled())
        return;
    is_hover = 1;
    repaint();
}

void LoadSlot::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    is_hover = 0;
    repaint();
}

void LoadSlot::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        is_pressed ^= 1;
        repaint();
        emit pressed(is_pressed);
    }
}

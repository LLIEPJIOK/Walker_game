#include "loadframe.h"

#include <QDate>
#include <QDir>

#define screen_size QApplication::screens().at(0)->size()

LoadFrame::LoadFrame(QWidget *parent)
    : QMainWindow(parent)
{
    //открыть поток в файл с названиями сейвов и в цикле создать объекты
    //закрыть поток

    QDir dir("../Game/Saves");
    auto saves =  dir.entryList();

    vblay = new QVBoxLayout();
    vblay->setAlignment(Qt::AlignTop);
    vblay->setSpacing(10);
    for (int i = 2; i < static_cast<int>(saves.size()); ++i)
    {
        loads.push_front(new LoadSlot(saves[i].mid(0, saves[i].indexOf("."))));
        loads[0]->setFixedSize(3 * screen_size.width() / 5., screen_size.height() / 10.);
        vblay->insertWidget(0, loads[0]);
        connect(loads[0], &LoadSlot::pressed, this, &LoadFrame::unpress);
    }

    vblay->setContentsMargins(3 * width() / 20., 0, 0, 0);

    widget = new QWidget();
    widget->setLayout(vblay);

    scroll = new QScrollArea();
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setWidget(widget);
    scroll->setWidgetResizable(1);
    scroll->setFrameStyle(QFrame::Panel);
    scroll->setLineWidth(3);

    QPalette p = palette();
    p.setBrush(QPalette::Window, QBrush(QColor(0, 0, 0, 0)));
    p.setBrush(QPalette::WindowText, QBrush(QColor(0, 0, 0, 0)));
    setPalette(p);
    scroll->setPalette(p);

    setCentralWidget(scroll);
}

LoadFrame::~LoadFrame()
{
    for (auto& i : loads)
    {
        delete i;
    }
}

QString LoadFrame::add_slot(QString name)
{
    // reverse order of date and time to save it in order from latest to newest
    auto date = QDate::currentDate().toString("yyyy_MM_dd");
    auto time = QTime::currentTime().toString("ss_mm_hh");
    auto file_name = time + "_" + date + "_" + name;
    loads.push_front(new LoadSlot(file_name));
    loads[0]->setFixedSize(3 * screen_size.width() / 5., screen_size.height() / 10.);
    vblay->insertWidget(0, loads[0]);
    return file_name;
}

void LoadFrame::unpress(bool is_pressed)
{
    if (is_pressed)
    {
        foreach(const auto i, loads)
            if (i != sender())
                i->unpress();
    }
    emit pressed(is_pressed, sender());
}

void LoadFrame::delete_load(QObject *ls)
{
    for (QList<LoadSlot*>::const_iterator i = loads.cbegin(); i != loads.cend(); ++i)
    {
        if (*i == ls)
        {
            (*i)->delete_save();
            loads.erase(i);
            repaint();
            return;
        }
    }
}


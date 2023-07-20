#include "loadframe.h"

LoadFrame::LoadFrame(QWidget *parent)
    : QMainWindow(parent)
{
    //открыть поток в файл с названиями сейвов и в цикле создать объекты
    //закрыть поток

    vblay = new QVBoxLayout();
    vblay->setAlignment(Qt::AlignTop);
    vblay->setSpacing(10);
    auto size = QApplication::screens().at(0)->size();
    for (int i = 0; i < 10; ++i)
    {
        loads.push_back(new LoadSlot("", "Имя", "Дата"));
        loads[i]->setFixedSize(3 * size.width() / 5., size.height() / 10.);
        vblay->addWidget(loads[i]);
        connect(loads[i], &LoadSlot::pressed, this, &LoadFrame::unpress);
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
        if (*i == ls)
        {
            (*i)->delete_save();
            loads.erase(i);
            repaint();
            return;
        }
}


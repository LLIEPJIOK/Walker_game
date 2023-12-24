#include "load.h"
#include "general.h"

Load::Load(QString _type, QWidget *parent)
    : QWidget(parent)
{
    type = _type;

    is_available = false;

    state = State::CHANGE_NAME;

    accept_changes_window = nullptr;
    change_name_window = nullptr;
    enter_save_name_window = nullptr;

    label = new QLabel("<U>Сохранения:</U>");
    label->setFont(QFont("Arial", 20));
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("QLabel {color: white;}");

    lf = new LoadFrame();
    auto size = QApplication::screens().at(0)->size();
    lf->setFixedSize(3 * size.width() / 4., 3 * size.height() / 4.);

    hblay = new QHBoxLayout();
    hblay->addWidget(lf);

    QFont btn_font = QFont("Arial", 14, QFont::Normal, 1);
    setStyleSheet("QPushButton:active   {color: white;}"
                  "QPushButton:disabled {color: gray;}"
                  "QPushButton:hover    {color: rgb(255, 178, 102);}");

    btn_prev = new QPushButton("Назад");
    btn_prev->setFlat(1);
    btn_prev->setFont(btn_font);

    btn_delete = new QPushButton("Удалить");
    btn_delete->setFlat(1);
    btn_delete->setFont(btn_font);

    btn_load = new QPushButton("Загрузить");
    btn_load->setFlat(1);
    btn_load->setFont(btn_font);

    btn_change = new QPushButton("Изменить");
    btn_change->setFlat(1);
    btn_change->setFont(btn_font);


    btn_add = new QPushButton("Добавить");
    btn_add->setFlat(1);
    btn_add->setFont(btn_font);

    btn_rewrite = new QPushButton("Перезаписать");
    btn_rewrite->setFlat(1);
    btn_rewrite->setFont(btn_font);

    set_available();

    ld_hblay = new QHBoxLayout();
    ld_hblay->setAlignment(Qt::AlignCenter);
    ld_hblay->setSpacing(50);
    ld_hblay->addWidget(btn_change);
    ld_hblay->addWidget(btn_delete);
    ld_hblay->addWidget(btn_load);

    ld_hblay->addWidget(btn_rewrite);
    ld_hblay->addWidget(btn_add);

    if (_type == "load")
    {
        btn_add->setVisible(false);
        btn_rewrite->setVisible(false);
        ld_hblay->setContentsMargins(QApplication::screens().at(0)->size().width() - 600, 0, 0, 0);
    }
    else
    {
        btn_delete->setVisible(false);
        btn_load->setVisible(false);
        btn_change->setVisible(false);
        ld_hblay->setContentsMargins(QApplication::screens().at(0)->size().width() - 450, 0, 0, 0);
    }

    btn_hblay = new QHBoxLayout();
    btn_hblay->setSpacing(0);
    btn_hblay->addWidget(btn_prev);
    btn_hblay->addLayout(ld_hblay);

    vblay = new QVBoxLayout();
    vblay->setSpacing(20);
    vblay->setAlignment(Qt::AlignJustify);
    vblay->addWidget(label);
    vblay->addLayout(hblay);
    vblay->addLayout(btn_hblay);

    setLayout(vblay);

    connect(lf, &LoadFrame::pressed, this, &Load::check_pressed);

    connect(btn_prev, SIGNAL(clicked()), this, SIGNAL(return_back_signal()));
    connect(btn_delete, SIGNAL(clicked()), this, SLOT(delete_window_slot()));
    connect(this, &Load::delete_load, lf, &LoadFrame::delete_load);
    connect(btn_load, SIGNAL(clicked()), this, SLOT(load_slot()));
    connect(this, &Load::load_game, General::get_general(), &General::load_game);
    connect(btn_change, SIGNAL(clicked()), this, SLOT(open_change_slot()));

    connect(btn_add, SIGNAL(clicked()), this, SLOT(add_slot()));
    connect(btn_rewrite, &QPushButton::clicked, this, &Load::rewrite_slot);
}

Load::~Load()
{
    delete lf;
    delete accept_changes_window;
    delete change_name_window;
}

// сбрасывает выделение на слот
void Load::reset()
{
    if (is_available)
    {
        qobject_cast<LoadSlot*>(chosen)->unpress();
        is_available = 0;
        set_available();
    }
}

void Load::set_available()
{
    btn_load->setEnabled(is_available);
    btn_delete->setEnabled(is_available);
    btn_change->setEnabled(is_available);
    btn_rewrite->setEnabled(is_available);
}

void Load::turn(bool is_turn_on)
{
    lf->setEnabled(is_turn_on);
    btn_prev->setEnabled(is_turn_on);
    if (is_turn_on)
    {
        set_available();
    }
    else
    {
        btn_load->setEnabled(false);
        btn_delete->setEnabled(false);
        btn_change->setEnabled(false);
        btn_rewrite->setEnabled(false);
    }
}

void Load::check_pressed(bool is_pressed, QObject *ls)
{
    is_available = is_pressed;
    chosen = ls;
    set_available();
}

void Load::delete_window_slot()
{
    if (accept_changes_window == nullptr)
    {
        accept_changes_window = new Accept("Вы точно хотите удалить сохранение?", this);
        connect(accept_changes_window, &Accept::accept_signal, this, &Load::delete_slot);
    }

    turn(0);
    accept_changes_window->show();
}

void Load::load_slot()
{
    close();
    emit load_game(qobject_cast<LoadSlot*>(chosen)->get_file_name());
}

void Load::open_change_slot()
{
    if (change_name_window == nullptr)
    {
        change_name_window = new EnterName("Измените название", qobject_cast<LoadSlot*>(chosen)->get_name(), this);
        connect(change_name_window, &EnterName::return_name, this, &Load::get_name_slot);
    }
    else
    {
        change_name_window->set_name_and_open(qobject_cast<LoadSlot*>(chosen)->get_name());
    }

    state = State::CHANGE_NAME;
    turn(0);
    change_name_window->show();
}

void Load::get_name_slot(QString name)
{
    turn(1);
    if (name != "")
    {
        switch (state)
        {
        case State::CHANGE_NAME :
            change_name_window->hide();
            qobject_cast<LoadSlot*>(chosen)->change_name(name);
            break;
        case State::ADD_NEW :
            enter_save_name_window->hide();
            emit save_game(lf->add_slot(name));
            break;
        case State::REWRITE :
            enter_save_name_window->hide();
            delete_slot(true);
            emit save_game(lf->add_slot(name));
            break;
        default:
            throw std::exception("unknown state");
        }
    }
}

void Load::delete_slot(bool is_delete)
{
    if (accept_changes_window != nullptr)
    {
        accept_changes_window->hide();
    }

    turn(1);
    if (is_delete)
    {
        is_available = 0;
        set_available();
        emit delete_load(chosen);
    }
}

void Load::add_slot()
{
    if (enter_save_name_window == nullptr)
    {
        enter_save_name_window = new EnterName("Введите название сохранения", "", this);
        connect(enter_save_name_window, &EnterName::return_name, this, &Load::get_name_slot);
    }

    state = State::ADD_NEW;
    enter_save_name_window->set_name_and_open("");
}

void Load::rewrite_slot()
{
    if (enter_save_name_window == nullptr)
    {
        enter_save_name_window = new EnterName("Введите название сохранения", qobject_cast<LoadSlot*>(chosen)->get_name(), this);
        connect(enter_save_name_window, &EnterName::return_name, this, &Load::get_name_slot);
    }

    state = State::REWRITE;
    enter_save_name_window->set_name_and_open(qobject_cast<LoadSlot*>(chosen)->get_name());
}

void Load::paintEvent(QPaintEvent *event)
{
    QPixmap background(":/backgrounds/Pictures/widget_backgrounds/Background.png");
    background = background.scaled(this->width(), this->height());
    QPainter painter(this);
    painter.drawPixmap(0, 0, background);
    QWidget::paintEvent(event);
}

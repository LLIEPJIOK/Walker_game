
#include "event_window.h"
#include "Event.h"
#include "Engine/translator.h"

Event_window::Event_window(QWidget *parent, Player *_target, Event *event)
    : QWidget(parent)
{
    QFont font ("Arial", 10, QFont::Normal, 1);
    QString style("color: rgb(255, 255, 255)");
    this->setFixedSize(600, 600);

    event_name = event->get_event_name();
    target = _target;

    main_layout = new QVBoxLayout(this);

    QPixmap img(QString::fromStdString(":/events/Pictures/Events/" + event->get_event_name() + ".png")); // Картинки надо сделать с нормальным соотношением, потому что все пикчи - квадратные
    img.scaled(400, 400);
    img_label = new QLabel(this);
    img_label->setPixmap(img);
    img_label->setScaledContents(1);
    img_label->setFixedSize(400, 400);

    main_layout->addWidget(img_label);

    info = new QLabel(Translator::translate(event->get_event_info().c_str()).c_str(), this);
    //info->resize(700, 350);
    info->setWordWrap(true);
    info->setFont(font);
    info->setStyleSheet(style);

    main_layout->addWidget(info);
    QHBoxLayout* buttons = new QHBoxLayout();

    challenge_your_luck = new QPushButton(this);
    challenge_your_luck->setText(Translator::translate(event->get_option().c_str()).c_str());
    challenge_your_luck->setCursor(Qt::PointingHandCursor);
    connect(challenge_your_luck, &QPushButton::clicked, this, &Event_window::challenge_button_was_clicked);
    buttons->addWidget(challenge_your_luck);


    std::string needed_characteristic = event->get_type();
    QString type;
    if(needed_characteristic == "AGIL")
    {
        type = tr("Agility");
    }
    else if (needed_characteristic == "STR")
    {
        type = tr("Strength");
    }
    else
    {
        type = tr("Intelligence");
    }

    requirement_info = new QLabel(this);
    requirement_info->setText(tr("Success rate of the event is based on your") + " " + type + "\n" +
                              tr("Current stat") + ": " + QString::number(target->get_characteristics().at(needed_characteristic)) + "\n");
    requirement_info->setFont(font);
    requirement_info->setStyleSheet(style);
    main_layout->addWidget(requirement_info);
    buttons->setAlignment(Qt::AlignBottom | Qt::AlignRight);

    main_layout->addLayout(buttons);

    main_layout->setAlignment(Qt::AlignCenter);

    setLayout(main_layout);
}

Event_window::~Event_window()
{
    delete main_layout;
    delete img_label;
    delete info;
    delete requirement_info;
    delete challenge_your_luck;
}

void Event_window::paintEvent(QPaintEvent *event)
{ 
    QPainter painter(this);
    painter.setBrush(QBrush(QColor(44, 66, 47)));
    painter.setPen(QPen(QColor(44, 66, 47)));
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawRect(0, 0 , this->width(), this->height());
    QWidget::paintEvent(event);
}

void Event_window::challenge_button_was_clicked()
{
    Event* event = Events::get_Events()->get_events()->at(event_name);

    int need = event->get_requirement(target) - target->get_characteristics().at(event->get_type()) - target->get_characteristics().at("EVENT_ROLL_MOD");

    srand(time(NULL));
    int DQNT = target->get_characteristics()["DQNT"];
    int roll =  rand() % (5 * DQNT + 1) + DQNT + target->get_characteristics().at("EVENT_ROLL_MOD");
    QString img_path;
    QString label;

    if((success = roll >= need))
    {
        img_path = QString::fromStdString(":/events/Pictures/Events/" + event->get_event_name() + "_success.png");
        label = QString::fromStdString(Translator::translate(event->get_success_text().c_str()));
    }
    else
    {
        img_path = QString::fromStdString(":/events/Pictures/Events/" + event->get_event_name() + "_failure.png");
        label = QString::fromStdString(Translator::translate(event->get_failure_text().c_str()));
    }

    QPixmap img(img_path); // Event rework
    img.scaled(550, 250);
    img_label->setPixmap(img);
    img_label->setScaledContents(1);

    info->setText(label);

    challenge_your_luck->setText(tr("Continue"));
    disconnect(challenge_your_luck, &QPushButton::clicked, this, &Event_window::challenge_button_was_clicked);
    connect(challenge_your_luck, &QPushButton::clicked, this, &Event_window::continue_playing);

    main_layout->removeWidget(requirement_info);
    delete requirement_info;
    requirement_info = nullptr;
}

void Event_window::continue_playing()
{
    if(success)
        Events::get_Events()->get_events()->at(event_name)->execute_success(target);
    else
        Events::get_Events()->get_events()->at(event_name)->execute_failure(target);

    emit event_ended();

    delete this;
}

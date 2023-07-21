#include "entername.h"

EnterName::EnterName(QString _str, QString old_name, QWidget *parent)
    : QFrame(parent)
{
    str = _str;

    ac = nullptr;

    auto size = QApplication::screens().at(0)->size();
    setGeometry(size.width() / 2 - 250, size.height() / 2 - 105, 500, 210);

    QPalette pal = palette();
    pal.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(100, 100, 100));

    label = new QLabel(str, this);
    label->setFont(QFont("Arial", 16));
    label->setAlignment(Qt::AlignCenter);
    label->resize(width(), 50);
    label->move(0, 15);
    label->setStyleSheet("QLabel {color: white;}");

    btn_cansel = new QPushButton("Отмена", this);
    btn_cansel->setFlat(1);
    btn_cansel->setFont(QFont("Arial", 14, QFont::Normal, 1));
    btn_cansel->resize(80, 30);
    btn_cansel->move(25, 150);
    btn_cansel->setStyleSheet("QPushButton        {color: white;}"
                              "QPushButton:hover  {color: rgb(255, 178, 102);}");

    btn_save = new QPushButton("Изменить", this);
    btn_save->setFlat(1);
    btn_save->setFont(QFont("Arial", 14, QFont::Normal, 1));
    btn_save->setPalette(pal);
    btn_save->resize(100, 30);
    btn_save->move(width() - 125, 150);
    btn_save->setStyleSheet("QPushButton:active   {color: white;}"
                            "QPushButton:disabled {color: gray;}"
                            "QPushButton:hover    {color: rgb(255, 178, 102);}");

    edit = new QLineEdit(this);
    edit->setText(old_name);
    edit->setFont(QFont("Arial", 14));
    edit->resize(width() - 100, 30);
    edit->move(50, 80);

    check_text(old_name);

    connect(edit, &QLineEdit::textEdited, this, &EnterName::check_text);
    connect(btn_cansel, SIGNAL(clicked()), this, SLOT(return_slot()));
    connect(btn_save, SIGNAL(clicked()), this, SLOT(return_slot()));
}

EnterName::~EnterName()
{
    delete ac;
}

void EnterName::set_name_and_open(QString name)
{
    edit->setText(name);
    check_text(name);
    turn(1);
}

void EnterName::turn(bool is_turn_on)
{
    if (is_turn_on)
    {
        show();
    }
    else
    {
        hide();
    }

    btn_cansel->setEnabled(is_turn_on);
    btn_save->setEnabled(is_turn_on);
    edit->setEnabled(is_turn_on);
}

void EnterName::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 220));
    painter.drawRect(0, 0, width(), height());
}

void EnterName::check_text(const QString& text)
{
    btn_save->setEnabled(text != "");
}

void EnterName::return_slot()
{
    if (sender() == btn_cansel)
    {
        emit return_name("");
    }
    else
    {
        if (ac == nullptr)
        {
            ac = new Accept("Вы точно хотите изменить название?", qobject_cast<QWidget*>(parent()));
            connect(ac, &Accept::accept_signal, this, &EnterName::check_slot);
        }
        ac->show();
        turn(0);
    }
}

void EnterName::check_slot(bool is_yes)
{
    ac->hide();
    if (is_yes)
    {
        emit return_name(edit->text());
    }
    else
    {
        turn(1);
    }
}

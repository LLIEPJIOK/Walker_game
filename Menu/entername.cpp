#include "entername.h"

EnterName::EnterName(QString _str, QString old_name, QWidget *parent)
    : QFrame(parent)
{
    str = _str;

    auto size = QApplication::screens().at(0)->size();
    setGeometry(size.width() / 2 - 250, size.height() / 2 - 125, 500, 250);

    QPalette pal = palette();
    pal.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(100, 100, 100));

    setStyleSheet("background: rgb(210, 210, 210)");
    setAutoFillBackground(1);

    label = new QLabel(str, this);
    label->setFont(QFont("Arial", 16));
    label->setAlignment(Qt::AlignCenter);
    label->resize(width(), 50);
    label->move(0, 30);

    btn_cansel = new QPushButton("Отмена", this);
    btn_cansel->setFlat(1);
    btn_cansel->setFont(QFont("Arial", 14, QFont::Normal, 1));
    btn_cansel->resize(80, 30);
    btn_cansel->move(25, 150);

    btn_save = new QPushButton("Изменить", this);
    btn_save->setFlat(1);
    btn_save->setFont(QFont("Arial", 14, QFont::Normal, 1));
    btn_save->setPalette(pal);
    btn_save->resize(100, 30);
    btn_save->move(width() - 125, 150);

    edit = new QLineEdit(this);
    edit->setText(old_name);
    //edit->selectAll();
    edit->setFont(QFont("Arial", 14));
    edit->resize(width() - 100, 30);
    edit->move(50, 90);

    check_text(old_name);

    connect(edit, &QLineEdit::textEdited, this, &EnterName::check_text);
    connect(btn_cansel, SIGNAL(clicked()), this, SLOT(return_slot()));
    connect(btn_save, SIGNAL(clicked()), this, SLOT(return_slot()));
}

void EnterName::turn(bool is_turn_on)
{
    btn_cansel->setEnabled(is_turn_on);
    btn_save->setEnabled(is_turn_on);
    edit->setEnabled(is_turn_on);
}

void EnterName::check_text(const QString& text)
{
    btn_save->setEnabled(text != "");
}

void EnterName::return_slot()
{
    if (sender() == btn_cansel)
        emit return_name("");
    else
    {
        ac = new Accept("Вы точно хотите изменить название?", qobject_cast<QWidget*>(parent()));
        ac->show();
        connect(ac, &Accept::accept_signal, this, &EnterName::check_slot);
        turn(0);
    }
}

void EnterName::check_slot(bool is_yes)
{
    delete ac;
    turn(1);
    if (is_yes)
        emit return_name(edit->text());
}

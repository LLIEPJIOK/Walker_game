#include "entername.h"

#include <QRegularExpression>
#include <QRegularExpressionValidator>

EnterName::EnterName(QString _str, QString old_name, QWidget *parent)
    : QFrame(parent)
{
    str = _str;

    ac = nullptr;

    auto size = QApplication::screens().at(0)->size();
    setGeometry(size.width() / 2 - 250, size.height() / 2 - 105, 500, 210);

    auto btn_font = QFont("Arial", 14, QFont::Normal, 1);
    setStyleSheet("QPushButton:active   {color: white;}"
                  "QPushButton:disabled {color: gray;}"
                  "QPushButton:hover    {color: rgb(255, 178, 102);}"
                  "QLabel               {color: white;}");

    label = new QLabel(str, this);
    label->setFont(QFont("Arial", 16));
    label->setAlignment(Qt::AlignCenter);
    label->resize(width(), 50);
    label->move(0, 15);

    btn_cansel = new QPushButton("Отмена", this);
    btn_cansel->setFlat(1);
    btn_cansel->setFont(btn_font);
    btn_cansel->resize(90, 30);
    btn_cansel->move(25, 150);

    btn_save = new QPushButton(this);
    btn_save->setFlat(1);
    btn_save->setFont(btn_font);
    btn_save->resize(120, 30);
    btn_save->move(width() - 145, 150);

    if (str == "Введите название сохранения")
    {
        btn_save->setText("Сохранить");
    }
    else
    {
        btn_save->setText("Изменить");
    }

    // разрешённые символы
    QRegularExpression expression("[a-zA-Zа-яА-яўі'ё 0-9]+");

    edit = new QLineEdit(this);
    edit->setValidator(new QRegularExpressionValidator(expression));
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
    turn(0);
    if (sender() == btn_cansel)
    {
        emit return_name("");
    }
    else
    {
        if (str == "Введите название сохранения")
        {
            check_slot(true);
            return;
        }

        if (ac == nullptr)
        {
            ac = new Accept("Вы точно хотите изменить название?", qobject_cast<QWidget*>(parent()));
            connect(ac, &Accept::accept_signal, this, &EnterName::check_slot);
        }

        ac->show();
    }
}

void EnterName::check_slot(bool is_yes)
{
    if (ac != nullptr)
    {
        ac->hide();
    }

    if (is_yes)
    {
        emit return_name(edit->text());
    }
    else
    {
        turn(1);
    }
}

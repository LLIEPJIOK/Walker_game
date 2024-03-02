
#include "joingamewidget.h"

void JoinGameWidget::join()
{
    emit joinTo(field->text().toStdString());
}

JoinGameWidget::JoinGameWidget()
{

}

JoinGameWidget::JoinGameWidget(QWidget *parent) : QWidget(parent)
{
    setStyleSheet("QWidget {background-image: url(:/backgrounds/Pictures/widget_backgrounds/Background.png);}");
    this->setFixedSize(300, 150);
    cont_btn = new QPushButton(this);
    cont_btn->setText(tr("JOIN"));
    cont_btn->setGeometry(0, 75, 150, 75);

    cancel_btn = new QPushButton(this);
    cancel_btn->setText(tr("CANCEL"));
    cancel_btn->setGeometry(150, 75, 150, 75);

    field = new QLineEdit(this);
    field->setGeometry(0, 38, 300, 37);
    field->setText("X.X.X.X");

    label = new QLabel(this);
    label->setGeometry(0, 0, 300, 38);
    label->setText(tr("Enter Host IP Adress"));

    connect(cancel_btn, &QPushButton::clicked, this, &JoinGameWidget::cancel);
    connect(cont_btn, &QPushButton::clicked, this, &JoinGameWidget::join);
}

